#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <regex.h>
#include <stdbool.h>

#include "postgres.h"

#include "fmgr.h"
#include "libpq/pqformat.h"        /* needed for send/recv functions */


#include "utils/hashutils.h"
#include "utils/builtins.h"
//#include "access/hash.h"



PG_MODULE_MAGIC;

typedef struct intSet {
    // 固定写法？？？？
    int allocate_len;
    int intset[1];
} intSet;

//determine whether the inset is valid


// Sort the inset, by selection sort
static int SelectionSort(int *arr, int size) {
    int i, j, k, tmp;
    for (i = 0; i < size - 1; i++) {
        k = i;
        for (j = i + 1; j < size; j++) {
            if (arr[j] < arr[k]) {
                k = j;
            }
        }
        tmp = arr[k];
        arr[k] = arr[i];
        arr[i] = tmp;
    }
}


static int split_String(char str[], char delims[]) {
    char *result = NULL;
    result = strtok(str, delims);
    while (result != NULL) {
        printf("result is \"%s\"\n", result);
        result = strtok(NULL, delims);
    }

}

/*****************************************************************************
 * Input/Output functions
 *****************************************************************************/

PG_FUNCTION_INFO_V1(intset_in);

Datum
intset_in(PG_FUNCTION_ARGS) {

    char *str = PG_GETARG_CSTRING(0);
    intSet *result;
    elog(NOTICE, "===================stt:%s ======= ", str);

    char delims[] = "{, }";
    char *temp_result = NULL;
    int allocate_len = 100;
    int *array_intset;
    int index = 0;

    int m, max;
    int i, index_element_num;
    int arrayflag[max + 1];

    int *temp1 = (int *) palloc(sizeof(index_element_num + 1));


    // Assign an address to the array
    array_intset = (int *) malloc(sizeof(int) * allocate_len);

    // cut the current string
    temp_result = strtok(str, delims);
    while (temp_result != NULL) {
        if (index >= allocate_len) {
            allocate_len = allocate_len + allocate_len;
            array_intset = (int *) realloc(array_intset, sizeof(int) * allocate_len);
        }
        // string to number
        array_intset[index] = atoi(temp_result);

        temp_result = strtok(NULL, delims);
        index++;
    }

    // Find the maximum value in the current array_set
    max = array_intset[0];

    for (m = 1; m < index; m++) {
        if (max < array_intset[m])
            max = array_intset[m];
    }

    // delete the duplicated elements(number)
    for (i = 0; i <= max; i++) {
        arrayflag[i] = false;
    }
    //delete Algorithm
    for (i = 0; i < index; i++) {
        arrayflag[array_intset[i]] = array_intset[i];
    }
    //Take out the effective number
    for (i = 0, index_element_num = 0; i <= max; i++) {
        if (arrayflag[i] != false) {
            array_intset[index_element_num++] = arrayflag[i];
        }
    }
    // sorting the array_intset
    SelectionSort(array_intset, index_element_num);

    result = (intSet *) palloc(VARHDRSZ + temp1);
    SET_VARSIZE(result, VARHDRSZ + temp1)

    //assign the value of array_intset to the result
    // the fist one inset[0] of result means: the number of all valid elements
    // and the valid number is start at inset[1]
    result->intset[0] = index_element_num;

    for (int j = 0; j < index_element_num; j++) {
        result->intset[j + 1] = array_intset[j];
    }

    free(array_intset);
    PG_RETURN_POINTER(result);
}


PG_FUNCTION_INFO_V1(intset_out);

Datum
intset_out(PG_FUNCTION_ARGS) {
    intSet *intset_elements = (intSet *) PG_GETARG_POINTER(0);
    char *result_temp;
    char *result;
    int index_count;
    int allocate_len = 100;
    char string_limit[32];

    result_temp = (char *) malloc(sizeof(char) * allocate_len);
    index_count = intset_elements->intset[0];

    strcpy(result_temp, "{");
    if (index_count>0){
        // stitching the first number
        // pg_ltoa: number to string
        pg_ltoa(intset_elements->intset[1], string_limit);
        strcat(result_temp, string_limit);
        for (int i=1; i<index_count; i++){
            strcat(result_temp, ",");
            pg_ltoa(intset_elements->intset[i+1], string_limit);
            if (strlen(result_temp)+strlen(string_limit) +2 >= allocate_len){
                allocate_len = allocate_len allocate_len;
                result_temp = (char*)realloc(result_temp,sizeof(char)*allocate_len);
            }
            strcat(result_temp, string_limit);
        }
    }
    if (strlen(result_temp)+strlen(string_limit) +2 >= allocate_len){
        allocate_len = allocate_len allocate_len;
        result_temp = (char*)realloc(result_temp,sizeof(char)*allocate_len);

    }
    strcat(result_temp, "}");
    result = psprintf("%s", result_temp);

    free(result_temp);
    PG_RETURN_CSTRING(result);
}


PG_FUNCTION_INFO_V1(intset_cardinality);

Datum
intset_cardinality(PG_FUNCTION_ARGS) {
    intSet *a = (intSet *) PG_GETARG_POINTER(0);

    int result;

    PG_RETURN_INT32(a->intset[0]);
}


PG_FUNCTION_INFO_V1(intset_equal);

Datum
intset_equal(PG_FUNCTION_ARGS) {
    intSet *a = (intSet *) PG_GETARG_POINTER(0);
    intSet *b = (intSet *) PG_GETARG_POINTER(1);

    PG_RETURN_BOOL(complex_abs_cmp_internal(a, b) == 0);
}





