/*
 * src/tutorial/intset.c
 *
 ******************************************************************************
  This file contains routines that can be bound to a Postgres backend and
  called by the backend in the process of processing queries.  The calling
  format for these routines is dictated by Postgres architecture.
******************************************************************************/

#include "postgres.h"

#include "fmgr.h"
#include "libpq/pqformat.h"		/* needed for send/recv functions */
#include "access/hash.h"
#include <regex.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ctype.h"

#define TABLE_SIZE 1000

typedef struct LinkedNode {
    int value;
    struct node *next;
} LinkedNode;

typedef struct LinkedNode *LinkedList;

typedef struct HashTable {
    LinkedList *table;
    int size;
} HashTable;

typedef struct HashTable *Table;


LinkedList createNode(int);
static LinkedList createList(void);
LinkedList insertNode(LinkedList, int);
LinkedList deleteNode(LinkedList, int);
void deleteList(LinkedList);
int getAllValue(LinkedList, int*, int);
bool isExist(LinkedList, int);

int hashCode(int);
Table createTable(void);  // create a table, every elements in this table is a LinkedList
void insertValue(Table, int);  // insert the int into the table
void deleteTable(Table);
int getSize(Table);
int* tableToArray(Table);  // allocate memory, should free after use.

// operations
bool contains(Table, int);

/* ************************************** */
PG_MODULE_MAGIC;

typedef struct Intset
{
	int32 length;
	int	set[FLEXIBLE_ARRAY_MEMBER];
} Intset;

static bool is_valid(char *target, bool test_empty)
{	/* check whether the input intset is valid. */
    bool valid;
    char *pattern;
    regex_t complied;
    int cflag;

    if (!test_empty) {
        pattern = "^[ ]*[\\{]{1}[ ]*([0-9]+([ ]*[\\,]{1}[ ]*[0-9]+)*)*[ ]*[\\}]{1}[ ]*$";
    } else {
        pattern = "^[ ]*[\\{]{1}[ ]*[\\}]{1}[ ]*$";
    }

    cflag = REG_NOSUB | REG_EXTENDED;
    regcomp(&complied, pattern, cflag);
    valid = regexec(&complied, target, 0, NULL, 0) ? false : true;
    regfree(&complied);
    return valid;
}



static void merge(int* array, int left, int mid, int right, int* helper) {
    int left_idx;
    int right_idx;

    int i;
    for (i = left; i <= right; i++) {
        helper[i] = array[i];
    }

    left_idx = left;
    right_idx = mid + 1;
    while (left_idx <= mid && right_idx <= right) {
        if (helper[left_idx] < helper[right_idx]) {
            array[left++] = helper[left_idx++];
        } else {
            array[left++] = helper[right_idx++];
        }
    }
    while (left_idx <= mid) {
        array[left++] = helper[left_idx++];
    }
}

static void merge_sort(int* array, int left, int right, int *helper) {
    if (left >= right) {
        return;
    }
    merge_sort(array, left, left + (right - left) / 2, helper);
    merge_sort(array, left + (right - left) / 2 + 1, right, helper);
    merge(array, left, left + (right - left) / 2, right, helper);
}


static bool search(int* array, int left, int right, int target) {
    int mid;
    while (left <= right) {
        mid = left + (right - left) / 2;
        if (array[mid] == target) {
            return true;
        } else if (array[mid] > target) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return false;
}


/*****************************************************************************
 * Input/Output functions
 *****************************************************************************/

PG_FUNCTION_INFO_V1(intset_in);

Datum
intset_in(PG_FUNCTION_ARGS)
{
    char *str;
    Table myTable;
    int length;
    char *tmp;
    int j;
    int number;
    int table_size;
    int *tempArray;
    int *helper;    
    Intset *result;

	str = PG_GETARG_CSTRING(0);

	if (!is_valid(str, false)) {
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for Intset: \"%s\"", str)));
	}

	// initialize hash table
	myTable = createTable();

	// traverse the string. Get all number and add them into table
	length = strlen(str);  // calculate the max situation
	tmp = malloc(length * sizeof(char));
    assert(tmp != NULL);

    j = 0;
    if (is_valid(str, true)) {
        length = 0;
    }


    for (int i = 0; i < length && str[i] != '\0'; i++) {
        if (str[i] == '{' || str[i] == ' ') {
            continue;
        }
        if (isdigit(str[i])) { // if str[i] is a digit, record it
            tmp[j] = str[i];
            j++;
        } else {  // if str[i] is ',' or '}', all digit has been record
            tmp[j] = '\0';
            number = atoi(tmp);
            j = 0;
            insertValue(myTable, number);
        }
    }
    table_size = getSize(myTable);

    tempArray = tableToArray(myTable);
    helper = malloc(table_size * sizeof(int));
    merge_sort(tempArray, 0, table_size - 1, helper);
    
    result = (Intset *) palloc(VARHDRSZ + table_size * sizeof(int));
    SET_VARSIZE(result, VARHDRSZ + table_size * sizeof(int));
    memcpy(result->set, tempArray, table_size * sizeof(int));

    free(helper);
    free(tempArray);
    deleteTable(myTable);
    free(tmp);
	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(intset_out);

Datum
intset_out(PG_FUNCTION_ARGS)
{
    Intset *intset;
    int length;
    char *result;

	intset = (Intset *) PG_GETARG_POINTER(0);
	length = VARSIZE_ANY_EXHDR(intset) / sizeof(int);
	result = "{";

	for (int i = 0; i < length; i++) {
		if (i == 0) {
			result = psprintf("%s%d", result, intset->set[i]);
		} else {
			result = psprintf("%s,%d", result, intset->set[i]);
		}
	}
    result = psprintf("%s}", result);

	PG_RETURN_CSTRING(result);
}

/*****************************************************************************
 * New Operators
 *****************************************************************************/

// i ? S  : S contains i
PG_FUNCTION_INFO_V1(intset_contains);
Datum
intset_contains(PG_FUNCTION_ARGS)
{
    int value;
    Intset *intset;
    int length;
    bool result;

	value = PG_GETARG_INT32(0);
	intset = (Intset *) PG_GETARG_POINTER(1);

    length = VARSIZE_ANY_EXHDR(intset) / sizeof(int);
    result = search(intset->set, 0, length - 1, value);

	PG_RETURN_BOOL(result);
}

// # S : cardinality
PG_FUNCTION_INFO_V1(intset_cardinality);
Datum
intset_cardinality(PG_FUNCTION_ARGS)
{
    Intset *intset;
    int result;

    intset = (Intset *) PG_GETARG_POINTER(0);
    result = VARSIZE_ANY_EXHDR(intset) / sizeof(int);

    PG_RETURN_INT32(result);
}

// A >@ B
PG_FUNCTION_INFO_V1(intset_improper_superset);
Datum
intset_improper_superset(PG_FUNCTION_ARGS)
{
    Intset    *intset_a; 
    Intset    *intset_b;
    int length_a;
    int length_b;
    bool result;
    Table table_a;

    intset_a = (Intset *) PG_GETARG_POINTER(0);
    intset_b = (Intset *) PG_GETARG_POINTER(1);

    length_a = VARSIZE_ANY_EXHDR(intset_a) / sizeof(int);
    length_b = VARSIZE_ANY_EXHDR(intset_b) / sizeof(int);
    if (length_a < length_b) {
        PG_RETURN_BOOL(false);
    }

    result = true;

    table_a = createTable();
    for (int i = 0; i < length_a; i++) {
        insertValue(table_a, intset_a->set[i]);
    }

    for (int i = 0; i < length_b; i++) {
        if (!contains(table_a, intset_b->set[i])) {
            result = false;
            break;
        }
    }

    deleteTable(table_a);
    PG_RETURN_BOOL(result);
}

// A @< B
PG_FUNCTION_INFO_V1(intset_improper_subset);
Datum
intset_improper_subset(PG_FUNCTION_ARGS)
{
    Intset    *intset_a; 
    Intset    *intset_b;
    int length_a;
    int length_b;
    bool result;
    Table table_a;

    intset_b = (Intset *) PG_GETARG_POINTER(0);
    intset_a = (Intset *) PG_GETARG_POINTER(1);

    length_a = VARSIZE_ANY_EXHDR(intset_a) / sizeof(int);
    length_b = VARSIZE_ANY_EXHDR(intset_b) / sizeof(int);
    if (length_a < length_b) {
        PG_RETURN_BOOL(false);
    }

    result = true;

    table_a = createTable();
    for (int i = 0; i < length_a; i++) {
        insertValue(table_a, intset_a->set[i]);
    }

    for (int i = 0; i < length_b; i++) {
        if (!contains(table_a, intset_b->set[i])) {
            result = false;
            break;
        }
    }

    deleteTable(table_a);
    PG_RETURN_BOOL(result);
}

// A = B
PG_FUNCTION_INFO_V1(intset_equal);
Datum
intset_equal(PG_FUNCTION_ARGS)
{
    Intset    *intset_a;
    Intset    *intset_b;
    int length_a;
    int length_b;

    intset_a = (Intset *) PG_GETARG_POINTER(0);
    intset_b = (Intset *) PG_GETARG_POINTER(1);

    length_a = VARSIZE_ANY_EXHDR(intset_a) / sizeof(int);
    length_b = VARSIZE_ANY_EXHDR(intset_b) / sizeof(int);
    if (length_a != length_b) {
        PG_RETURN_BOOL(false);
    }

    for (int i = 0; i < length_a; i++) {
        if(intset_a->set[i] != intset_b->set[i]) {
            PG_RETURN_BOOL(false);
        }
    }

    PG_RETURN_BOOL(true);
}


// A <> B
PG_FUNCTION_INFO_V1(intset_not_equal);
Datum
intset_not_equal(PG_FUNCTION_ARGS)
{
    Intset    *intset_a;
    Intset    *intset_b;
    int length_a;
    int length_b;

    intset_a = (Intset *) PG_GETARG_POINTER(0);
    intset_b = (Intset *) PG_GETARG_POINTER(1);

    length_a = VARSIZE_ANY_EXHDR(intset_a) / sizeof(int);
    length_b = VARSIZE_ANY_EXHDR(intset_b) / sizeof(int);
    if (length_a != length_b) {
        PG_RETURN_BOOL(true);
    }

    for (int i = 0; i < length_a; i++) {
        if(intset_a->set[i] != intset_b->set[i]) {
            PG_RETURN_BOOL(true);
        }
    }

    PG_RETURN_BOOL(false);
}


// A && B
PG_FUNCTION_INFO_V1(intset_and);
Datum
intset_and(PG_FUNCTION_ARGS)
{
    Intset    *intset_a;
    Intset    *intset_b;
    int length_a;
    int length_b;
    int length;
    int *tempArray;
    int i;
    int j;
    int number_of_result;
    Intset *result;

    intset_a = (Intset *) PG_GETARG_POINTER(0);
    intset_b = (Intset *) PG_GETARG_POINTER(1);

    length_a = VARSIZE_ANY_EXHDR(intset_a) / sizeof(int);
    length_b = VARSIZE_ANY_EXHDR(intset_b) / sizeof(int);
    
    length = length_a > length_b ? length_a : length_b;
    tempArray = malloc(length * sizeof(int));
    assert(tempArray != NULL);

    i = 0;
    j = 0;
    number_of_result = 0;
    while (i < length_a && j < length_b) {
        if (intset_a->set[i] == intset_b->set[j]) {
            tempArray[number_of_result++] = intset_a->set[i++];
            j++;
        } else if (intset_a->set[i] < intset_b->set[j]) {
            i++;
        } else {
            j++;
        }
    }

    result = (Intset *) palloc(VARHDRSZ + number_of_result * sizeof(int));
    SET_VARSIZE(result, VARHDRSZ + number_of_result * sizeof(int));
    memcpy(result->set, tempArray, number_of_result * sizeof(int));

    free(tempArray);
    PG_RETURN_POINTER(result);
}

// A || B
PG_FUNCTION_INFO_V1(intset_or);
Datum
intset_or(PG_FUNCTION_ARGS)
{
    Intset    *intset_a;
    Intset    *intset_b;
    int length_a;
    int length_b;
    int length;
    int *tempArray;
    int i;
    int j;
    int number_of_result;
    Intset *result;

    intset_a = (Intset *) PG_GETARG_POINTER(0);
    intset_b = (Intset *) PG_GETARG_POINTER(1);

    length_a = VARSIZE_ANY_EXHDR(intset_a) / sizeof(int);
    length_b = VARSIZE_ANY_EXHDR(intset_b) / sizeof(int);
    length = length_a + length_b;

    tempArray = malloc(length * sizeof(int));
    assert(tempArray != NULL);

    i = 0;
    j = 0;
    number_of_result = 0;
    while (i < length_a && j < length_b) {
        if (intset_a->set[i] == intset_b->set[j]) {
            tempArray[number_of_result++] = intset_a->set[i++];
            j++;
        } else if (intset_a->set[i] < intset_b->set[j]) {
            tempArray[number_of_result++] = intset_a->set[i++];
        } else {
            tempArray[number_of_result++] = intset_b->set[j++];
        }
    }
    while (i < length_a) {
        tempArray[number_of_result++] = intset_a->set[i++];
    }
    while (j < length_b) {
        tempArray[number_of_result++] = intset_b->set[j++];
    }

    result = (Intset *) palloc(VARHDRSZ + number_of_result * sizeof(int));
    SET_VARSIZE(result, VARHDRSZ + number_of_result * sizeof(int));
    memcpy(result->set, tempArray, number_of_result * sizeof(int));

    free(tempArray);
    PG_RETURN_POINTER(result);
}

// A !! B
PG_FUNCTION_INFO_V1(intset_disjunction);
Datum
intset_disjunction(PG_FUNCTION_ARGS)
{
    Intset    *intset_a;
    Intset    *intset_b;
    int length_a;
    int length_b;
    int length;
    int *tempArray;
    int i;
    int j;
    int number_of_result;
    Intset *result;

    intset_a = (Intset *) PG_GETARG_POINTER(0);
    intset_b = (Intset *) PG_GETARG_POINTER(1);

    length_a = VARSIZE_ANY_EXHDR(intset_a) / sizeof(int);
    length_b = VARSIZE_ANY_EXHDR(intset_b) / sizeof(int);
    length = length_a + length_b;

    tempArray = malloc(length * sizeof(int));
    assert(tempArray != NULL);

    i = 0;
    j = 0;
    number_of_result = 0;
    while (i < length_a && j < length_b) {
        if (intset_a->set[i] == intset_b->set[j]) {
            i++;
            j++;
        } else if (intset_a->set[i] < intset_b->set[j]) {
            tempArray[number_of_result++] = intset_a->set[i++];
        } else {
            tempArray[number_of_result++] = intset_b->set[j++];
        }
    }
    while (i < length_a) {
        tempArray[number_of_result++] = intset_a->set[i++];
    }
    while (j < length_b) {
        tempArray[number_of_result++] = intset_b->set[j++];
    }

    result = (Intset *) palloc(VARHDRSZ + number_of_result * sizeof(int));
    SET_VARSIZE(result, VARHDRSZ + number_of_result * sizeof(int));
    memcpy(result->set, tempArray, number_of_result * sizeof(int));

    free(tempArray);
    PG_RETURN_POINTER(result);
}

// A - B
PG_FUNCTION_INFO_V1(intset_difference);
Datum
intset_difference(PG_FUNCTION_ARGS)
{
    Intset    *intset_a;
    Intset    *intset_b;
    int length_a;
    int length_b;
    int length;
    int *tempArray;
    int i;
    int j;
    int number_of_result;
    Intset *result;

    intset_a = (Intset *) PG_GETARG_POINTER(0);
    intset_b = (Intset *) PG_GETARG_POINTER(1);

    length_a = VARSIZE_ANY_EXHDR(intset_a) / sizeof(int);
    length_b = VARSIZE_ANY_EXHDR(intset_b) / sizeof(int);
    length = length_a + length_b;

    tempArray = malloc(length * sizeof(int));
    assert(tempArray != NULL);

    i = 0;
    j = 0;
    number_of_result = 0;
    while (i < length_a && j < length_b) {
        if (intset_a->set[i] == intset_b->set[j]) {
            i++;
            j++;
        } else if (intset_a->set[i] < intset_b->set[j]) {
            tempArray[number_of_result++] = intset_a->set[i++];
        } else {
            j++;
        }
    }
    while (i < length_a) {
        tempArray[number_of_result++] = intset_a->set[i++];
    }

    result = (Intset *) palloc(VARHDRSZ + number_of_result * sizeof(int));
    SET_VARSIZE(result, VARHDRSZ + number_of_result * sizeof(int));
    memcpy(result->set, tempArray, number_of_result * sizeof(int));

    free(tempArray);
    PG_RETURN_POINTER(result);
}




/************************** 
 *  methods of hashTable  *
 **************************/
LinkedList createNode(int value) {
    LinkedList newNode = malloc(sizeof(LinkedList));
    assert(newNode != NULL);
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

LinkedList createList(void) {
    return NULL;
}

LinkedList insertNode(LinkedList list, int value){
    LinkedList newHead;
    if (list == NULL) {
        return createNode(value);
    }
    newHead = createNode(value);
    newHead->next = (struct node *) list;
    return newHead;
}

LinkedList deleteNode(LinkedList list, int value) {
    LinkedList dummyNode;
    LinkedList prev;
    LinkedList curr;
    LinkedList newHead;

    if (list == NULL) {
        return list;
    }
    // create dummyNode
    dummyNode = malloc(sizeof(LinkedList));
    assert(dummyNode != NULL);
    dummyNode->value = 0;
    dummyNode->next = (struct node *) list;

    prev = dummyNode;
    curr = list;
    while (curr != NULL) {
        LinkedList next = (LinkedList) curr->next;
        if (curr->value == value) {
            prev->next = (struct node *) next;
            free(curr);
            break;
        } else {
            prev = curr;
            curr = next;
        }
    }
    newHead = (LinkedList) dummyNode->next;
    free(dummyNode);
    return newHead;
}

void deleteList(LinkedList list) {
    while (list != NULL) {
        LinkedList delNode = list;
        list = (LinkedList) list->next;
        free(delNode);
    }
}

int getAllValue(LinkedList head, int *array, int start) {
    while (head != NULL) {
        array[start++] = head->value;
        head = (LinkedList) head->next;
    }
    return start;
}

bool isExist(LinkedList list, int value) {
    while (list != NULL) {
        if (list->value == value) {
            return true;
        }
        list = (LinkedList) list->next;
    }
    return false;
}

/* *********** HashTable ************* */
int hashCode(int value) {
    return value % TABLE_SIZE;
}

Table createTable(void) {
    Table newTable = malloc(sizeof(struct HashTable));
    assert(newTable != NULL);
    newTable->table = malloc(TABLE_SIZE * sizeof(LinkedList));
    assert(newTable->table != NULL);
    for (int i = 0; i < TABLE_SIZE; i++) {
        newTable->table[i] = createList();
    }
    newTable->size = 0;
    return newTable;
}

void insertValue(Table hashTable, int value) {
    int idx = hashCode(value); // calculate the hashValue and use it as index
    if (!contains(hashTable, value)) {
        hashTable->table[idx] = insertNode(hashTable->table[idx], value);
        hashTable->size++;
    }
}

void deleteTable(Table hashTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        deleteList(hashTable->table[i]);
    }
    free(hashTable->table);
    free(hashTable);
}

int getSize(Table hashTable) {
    return hashTable->size;
}

int* tableToArray(Table hashTable) {
    int k;
    int *array;

    array = malloc(hashTable->size * sizeof(int));
    assert(array != NULL);
    k = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashTable->table[i] != NULL) {
            k = getAllValue(hashTable->table[i], array, k);
        }
    }
    return array;
}

bool contains(Table hashTable, int value) {
    int idx = hashCode(value);
    if (isExist(hashTable->table[idx], value)) {
        return true;
    }
    return false;
}
