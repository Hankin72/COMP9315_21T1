/*
 * src/tutorial/intset.c
 *
 ******************************************************************************
  This file contains routines that can be bound to a Postgres backend and
  called by the backend in the process of processing queries.  The calling
  format for these routines is dictated by Postgres architecture.
******************************************************************************/

#include "postgres.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "fmgr.h"
#include "libpq/pqformat.h"		/* needed for send/recv functions */
#include "../include/utils/builtins.h"
#define HASH_NUMBER 1024

PG_MODULE_MAGIC;

typedef struct {
	int32 length;
	int data[FLEXIBLE_ARRAY_MEMBER];
} Intset;

/* string process function*/
typedef enum {
    CONVERT_SUCCESS,
    CONVERT_OVERFLOW,
    CONVERT_UNDERFOW,
    CONVERT_INVONVERTIBLE
} convert_result;
convert_result convert_str2int(int *out, char *s);
convert_result get_next_number(char *s, int start, int end, int *out);

/*       hash set function     */
typedef struct _hash_node{
    int value;
    int exist;
    struct _hash_node *next;
} hash_node;

int hash_function(int x);
hash_node *init_hash_set();
hash_node *found_value_or_not(hash_node *hash_int_set, int value);
hash_node *insert_value(hash_node *hash_int_set, int x, int *number_of_value);
void value_print(hash_node *hash_int_set, int x);

/* change hash int set to int array */
int *hash2arry(hash_node *hash_int_set, int number_of_value);

/* quick sort function */
int partition(int arr[], int low, int high);
void quick_sort(int arr[], int start, int end);

/* binary search function */
int binarySearch(int arr[], int l, int r, int x);

/*****************************************************************************
 * Input/Output functions
 *****************************************************************************/

/* intset in function */
PG_FUNCTION_INFO_V1(intset_in);

Datum
intset_in(PG_FUNCTION_ARGS)
{
	// hash_set init
    int number_of_value = 0;
    hash_node *hash_int_set = init_hash_set();

	char *s = PG_GETARG_CSTRING(0);
    int i;
    int string_length = strlen(s);

    
	int empty_flag = 0;         /* check input string is empty or not */
    int start = 0;              /* the position start to convert to int */
    int next_start = 0;         /* next position to convert */
    int assign_flag = 0;        /* check reach the end of string */
    int end;                    /* the position end to convert to int */
	int result;                 /* convert result */

	int *array_int;             /* the final int result */

    if (s[0] != '{' || s[string_length - 1] != '}') {
        ereport(ERROR,
				(errcode(ERRCODE_FDW_INVALID_STRING_FORMAT),
				 errmsg("invalid input syntax for intset: \"%s\"",
						s)));
    }
	/* string is empty */
    if (string_length == 2) {
		Intset *t = (Intset *) palloc(VARHDRSZ);
		SET_VARSIZE(t, VARHDRSZ);
        PG_RETURN_POINTER(t);
    }
    /* find the next converting position */
    while (next_start < string_length - 1) {
        start = next_start + 1;
        assign_flag = 0;
        for (i = start; i < string_length - 1; i++) {
            if (s[i] == ',') {
                next_start = i;
                assign_flag = 1;
                break;
            }
        }
        /* check reach the end of string or not */
        if (!assign_flag) {
            if (start == 1) empty_flag = 1;
            next_start = string_length - 1;
        }
        /* ignore the spaces after integer */
        end = next_start - 1;
        while (end >= start && s[end] == ' ') {
            end--;
        }
        /* empty string or occur invalid space */
        if (end < start) {
            if (!empty_flag) {
                ereport(ERROR,
				(errcode(ERRCODE_FDW_INVALID_STRING_FORMAT),
				 errmsg("invalid input syntax for intset: \"%s\"", s)));
            } else {
                Intset *t = (Intset *) palloc(VARHDRSZ);
				SET_VARSIZE(t, VARHDRSZ);
        		PG_RETURN_POINTER(t);
            }
            
        }
        /* check the string is valid or not */
        if (get_next_number(s, start, end, &result) == CONVERT_SUCCESS) {
            insert_value(hash_int_set, result, &number_of_value);
        } else {
            ereport(ERROR,
				(errcode(ERRCODE_FDW_INVALID_STRING_FORMAT),
				 errmsg("invalid convert input syntax for intset: \"%s\"",
						s)));
        }
    }
    
    array_int = hash2arry(hash_int_set, number_of_value);
    quick_sort(array_int, 0, number_of_value - 1);
	Intset *t = (Intset *) palloc(VARHDRSZ + number_of_value * sizeof(int));
	SET_VARSIZE(t, VARHDRSZ + number_of_value * sizeof(int));
	memcpy(t -> data, array_int, number_of_value * sizeof(int));
    free(array_int);
    PG_RETURN_POINTER(t);
}



/* intset out function */
PG_FUNCTION_INFO_V1(intset_out);

Datum
intset_out(PG_FUNCTION_ARGS)
{

	Intset *intset = (Intset *) PG_GETARG_POINTER(0);
	int number_of_value = (intset -> length) / 16  - 1;
    char *result = "{";
	int i;
	if (number_of_value == 0) {
		result = "{}";
		PG_RETURN_CSTRING(result);
	}
    
	for (i = 0; i < number_of_value; i++) {
		if (i == 0) {
			result = psprintf("%s%d", result, intset -> data[i]);
		} else {
			result = psprintf("%s,%d", result, intset -> data[i]);
		}
	}
	result = psprintf("%s}", result);
	PG_RETURN_CSTRING(result);
}

/*****************************************************************************
 * New Operators
 *
 * A practical Intset datatype would provide much more than this, of course.
 *****************************************************************************/

/* i <@ S intSet S contains the integer i */
PG_FUNCTION_INFO_V1(intset_contain_integer);

Datum
intset_contain_integer(PG_FUNCTION_ARGS)
{
    int a = (int) PG_GETARG_POINTER(0);
    Intset *intset = (Intset *) PG_GETARG_POINTER(1);
    int number_of_value = (intset -> length) / 16  - 1;
    /* no element in data */
    if (number_of_value == 0) {
        PG_RETURN_BOOL(0);
    } 
    int r = binarySearch(intset -> data, 0, number_of_value - 1, a);
    PG_RETURN_BOOL((r != -1));
}

/* @ S number of distinct elements in S, |S| */
PG_FUNCTION_INFO_V1(intset_distinct_number);

Datum
intset_distinct_number(PG_FUNCTION_ARGS)
{
    Intset *intset = (Intset *) PG_GETARG_POINTER(0);
    PG_RETURN_INT32((intset -> length) / 16  - 1);
}

/* A @> B does intSet A contain only values in intSet B */
PG_FUNCTION_INFO_V1(intset_check_subset);
Datum
intset_check_subset(PG_FUNCTION_ARGS) {
    Intset *A = (Intset *) PG_GETARG_POINTER(0);
    Intset *B = (Intset *) PG_GETARG_POINTER(1);
    int A_number = (A -> length) / 16  - 1;
    int B_number = (B -> length) / 16  - 1;
    int i, r;
    if (A_number == 0 || (A_number > B_number)) {
        PG_RETURN_BOOL(0);
    }
    for (i = 0; i < A_number; i++) {
        r = binarySearch(B -> data, 0, B_number - 1, A->data[i]);
        if (r == -1) PG_RETURN_BOOL(0);
    }
    PG_RETURN_BOOL(1);
}

/* A = B intSets A and B are equal */
PG_FUNCTION_INFO_V1(intset_check_equal);
Datum
intset_check_equal(PG_FUNCTION_ARGS) {
    Intset *A = (Intset *) PG_GETARG_POINTER(0);
    Intset *B = (Intset *) PG_GETARG_POINTER(1);
    int A_number = (A -> length) / 16  - 1;
    int B_number = (B -> length) / 16  - 1;
    int i;
    if (A_number != B_number) PG_RETURN_BOOL(0);
    if (A_number == 0) PG_RETURN_BOOL(1);
    for (i = 0; i < A_number; i++) {
        if (A -> data[i] != B -> data[i]) PG_RETURN_BOOL(0);
    }
    PG_RETURN_BOOL(1);
}

/* A && B takes the set intersection */
PG_FUNCTION_INFO_V1(intset_set_intersection);
Datum
intset_set_intersection(PG_FUNCTION_ARGS) {
    Intset *A = (Intset *) PG_GETARG_POINTER(0);
    Intset *B = (Intset *) PG_GETARG_POINTER(1);
    int A_number = (A -> length) / 16  - 1;
    int B_number = (B -> length) / 16  - 1;
    int i = 0;
    int j = 0;
    int *x = malloc((A_number + B_number) * sizeof(int));
    int total_n = 0;
    while (i < A_number && j < B_number) {
        if (A -> data[i] == B -> data[j]) {
            x[total_n] = A -> data[i];
            i++; j++; total_n++;
        } else if (A -> data[i] < B -> data[j]) {
            i++;
        } else {
            j++;
        }
    }

    Intset *t = (Intset *) palloc(VARHDRSZ + total_n * sizeof(int));
	SET_VARSIZE(t, VARHDRSZ + total_n * sizeof(int));
	memcpy(t -> data, x, total_n * sizeof(int));
    free(x);
    PG_RETURN_POINTER(t);
}

/* A || B takes the set union */
PG_FUNCTION_INFO_V1(intset_set_union);
Datum
intset_set_union(PG_FUNCTION_ARGS) {
    Intset *A = (Intset *) PG_GETARG_POINTER(0);
    Intset *B = (Intset *) PG_GETARG_POINTER(1);
    int A_number = (A -> length) / 16  - 1;
    int B_number = (B -> length) / 16  - 1;
    int i = 0;
    int j = 0;
    int *x = malloc((A_number + B_number) * sizeof(int));
    int total_n = 0;

    while (i < A_number && j < B_number) {
        if (A -> data[i] == B -> data[j]) {
            x[total_n] = A -> data[i];
            i++; j++; total_n++;
        } else if (A -> data[i] < B -> data[j]) {
            x[total_n] = A -> data[i];
            total_n++;

            i++;
        } else {
            x[total_n] = B -> data[j];
            total_n++; j++;
        }
    }
    while (i < A_number) {
        x[total_n] = A -> data[i];
        total_n++; i++;
    }
    while (j < B_number) {
        x[total_n] = B -> data[j];
        total_n++; j++;
    }
    
    Intset *t = (Intset *) palloc(VARHDRSZ + total_n * sizeof(int));
	SET_VARSIZE(t, VARHDRSZ + total_n * sizeof(int));
	memcpy(t -> data, x, total_n * sizeof(int));
    free(x);
    PG_RETURN_POINTER(t);
}


/* A !! B takes the set disjunction */
PG_FUNCTION_INFO_V1(intset_set_disjunction);
Datum
intset_set_disjunction(PG_FUNCTION_ARGS) {
    Intset *A = (Intset *) PG_GETARG_POINTER(0);
    Intset *B = (Intset *) PG_GETARG_POINTER(1);
    int A_number = (A -> length) / 16  - 1;
    int B_number = (B -> length) / 16  - 1;
    int i = 0;
    int j = 0;
    int *x = malloc((A_number + B_number) * sizeof(int));
    int total_n = 0;
    while (i < A_number && j < B_number) {
        if (A -> data[i] == B -> data[j]) {
            i++; j++;
        } else if (A -> data[i] < B -> data[j]) {
            x[total_n] = A -> data[i];
            i++; total_n++;
        } else {
            x[total_n] = B -> data[j];
            j++; total_n++;
        }
    }
    while (i < A_number) {
        x[total_n] = A -> data[i];
        i++; total_n++;
    }
    while (j < B_number) {
        x[total_n] = B -> data[j];
        j++; total_n++;
    }

    Intset *t = (Intset *) palloc(VARHDRSZ + total_n * sizeof(int));
	SET_VARSIZE(t, VARHDRSZ + total_n * sizeof(int));
	memcpy(t -> data, x, total_n * sizeof(int));
    free(x);
    PG_RETURN_POINTER(t);
}

/* A - B takes the set difference */
PG_FUNCTION_INFO_V1(intset_set_difference);
Datum
intset_set_difference(PG_FUNCTION_ARGS) {
    Intset *A = (Intset *) PG_GETARG_POINTER(0);
    Intset *B = (Intset *) PG_GETARG_POINTER(1);
    int A_number = (A -> length) / 16  - 1;
    int B_number = (B -> length) / 16  - 1;
    int i = 0;
    int j = 0;
    int *x = malloc((A_number + B_number) * sizeof(int));
    int total_n = 0;
    while (i < A_number && j < B_number) {
        if (A -> data[i] == B -> data[j]) {
            i++; j++;
        } else if (A -> data[i] < B -> data[j]) {
            x[total_n] = A -> data[i];
            i++; total_n++;
        } else {
            j++;
        }
    }
    while (i < A_number) {
        x[total_n] = A -> data[i];
        i++; total_n++;
    }

    Intset *t = (Intset *) palloc(VARHDRSZ + total_n * sizeof(int));
	SET_VARSIZE(t, VARHDRSZ + total_n * sizeof(int));
	memcpy(t -> data, x, total_n * sizeof(int));
    free(x);
    PG_RETURN_POINTER(t);
}



/* 		string process  	*/
convert_result convert_str2int(int *out, char *s) {
    char *errstr;
    long l = strtol(s, &errstr, 10);
    if (l > INT32_MAX || (errno == ERANGE && l == INT32_MAX))
        return CONVERT_OVERFLOW;
    if (l < INT32_MIN || (errno == ERANGE && l == INT32_MIN))
        return CONVERT_UNDERFOW;
    if (*errstr != '\0')
        return CONVERT_INVONVERTIBLE;
    *out = (int)l;
    return CONVERT_SUCCESS;
}

convert_result get_next_number(char *s, int start, int end, int *out) {
    char *sub_s = malloc((end - start + 2) * sizeof(char));
    memcpy(sub_s, &s[start], end - start + 1);
    sub_s[ end - start + 1] = '\0';
    convert_result r = convert_str2int(out, sub_s);
    free(sub_s);
    return r;
}
/*       end      */



/*       hash set function     */

int hash_function(int x) {
    if (x < 0) return x % HASH_NUMBER + HASH_NUMBER;
    return x % HASH_NUMBER;
}

hash_node *init_hash_set() {
    hash_node *x = malloc(HASH_NUMBER * sizeof(hash_node));
    int i;
    for (i = 0; i < HASH_NUMBER; i++) {
        x[i].value = 0;
        x[i].exist = 0;
        x[i].next = NULL;
    }
    return x;
}

hash_node *found_value_or_not(hash_node *hash_int_set, int value) {
    while (hash_int_set -> next) {
        if (hash_int_set -> value == value) {
            
            return hash_int_set;
        }
        hash_int_set = hash_int_set -> next;
    }
    return hash_int_set;
}

hash_node *insert_value(hash_node *hash_int_set, int x, int *number_of_value) {
    int value = hash_function(x);
    if (!hash_int_set[value].exist) {
        hash_int_set[value].value = x;
        hash_int_set[value].exist = 1;
        *number_of_value = *number_of_value + 1;
    } else {
        hash_node *get_result = found_value_or_not(&hash_int_set[value], x);
        if (get_result -> value == x) {
            return hash_int_set;
        } else {
            hash_node *new_node = malloc(sizeof(hash_node));
            new_node -> value = x;
            new_node -> exist = 1;
            new_node -> next = NULL;
            get_result -> next = new_node;
            *number_of_value = *number_of_value + 1;
            
        }
    }
    return hash_int_set;
}
void value_print(hash_node *hash_int_set, int x) {
    int value = hash_function(x);
    hash_node *temp = &hash_int_set[value];
    while (temp) {
        printf("%d\n", temp -> value);
        temp = temp -> next;
    }
}
/*      end       */

/* change hash to int array */
int *hash2arry(hash_node *hash_int_set, int number_of_value) {
    int *result = calloc(number_of_value, sizeof(int));
    int i;
    int total_number = 0;
    for (i = 0; i < HASH_NUMBER; i++) {
        if (hash_int_set[i].exist) {
            result[total_number] = hash_int_set[i].value;
            total_number++;
            hash_node *temp = hash_int_set[i].next;
            while (temp) {
                result[total_number] = temp -> value;
                total_number++;
                hash_node *last = temp;
                temp = temp -> next;
                free(last);
            }
        }
    }
    free(hash_int_set);
    return result;
}


/* quick sort function */
int partition(int arr[], int low, int high){
    int key;
    key = arr[low];
    while(low<high){
        while(low <high && arr[high]>= key )
            high--;
        if(low<high)
            arr[low++] = arr[high];
        while( low<high && arr[low]<=key )
            low++;
        if(low<high)
            arr[high--] = arr[low];
    }
    arr[low] = key;
    return low;
}
void quick_sort(int arr[], int start, int end){
    int pos;
    if (start<end){
        pos = partition(arr, start, end);
        quick_sort(arr,start,pos-1);
        quick_sort(arr,pos+1,end);
    }
    return;
}

// /* binary search */

int binarySearch(int arr[], int l, int r, int x) {
   if (r >= l)
   {
        int mid = l + (r - l)/2;
        if (arr[mid] == x)  
            return mid;

        if (arr[mid] > x) 
            return binarySearch(arr, l, mid-1, x);

        return binarySearch(arr, mid+1, r, x);
   }
   return -1;
}