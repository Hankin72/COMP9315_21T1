
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
#include "access/hash.h"


PG_MODULE_MAGIC;

typedef struct intSet {
    // 固定写法？？？？
    int allocate_len;
    int intset[1];
} intSet;


// Sort the inset, by selection sort
static int Sorting_element(int *array, int size) {
    int i, j, k, temp;
    for (i = 0; i < size - 1; i++) {
        k = i;
        for (j = i + 1; j < size; j++) {
            if (array[j] < array[k]) {
                k = j;
            }
        }
        temp = array[k];
        array[k] = array[i];
        array[i] = temp;
    }
    return (*array);
}

//determine whether the inset is valid by regex expression
// function used to compile the regrex and execute it
static int regex_check_str(char *str, char *regexPattern) {
    regex_t reg_check;
    int output = false;
    int cflags;

    cflags = REG_EXTENDED;
    // compile the regex
    if (regcomp(&reg_check, regexPattern, cflags)) {
        return false;
    }
    // execute the regex
    if (regexec(&reg_check, str, 0, NULL, 0)) {
        output = false;
    } else {
        output = true;
    }
    // free the regex
    regfree(&reg_check);
    return output;
}

static int valid_str(char *str) {
    int match_result;
    char *re = "\\s*\\{(\\s*([0-9]+\\d*)\\s*,)*(\\s*([0-9]+\\d*))\\s*\\}\\s*|\\s*\\{\\s*\\}\\s*";
    int result = false;
    match_result = regex_check_str(str, re);
    if (match_result == false) {
        return false;
    } else {
        result = true;
    }
    return result;
}


static int intset_2_cstring(char *result, intSet *intset_elements, int allocate_len){
    int index;
    char left[10]="{";
    char right[10] ="}";
    char comma[10] =",";
    char temp_str[32];

    index = intset_elements->intset[0];

    if (index==0){
        sprintf(result, "%s%s", left,right);
    }
    if (index > 0) {
        sprintf(result, "%s", left);
        sprintf(temp_str, "%d", intset_elements->intset[1]);
        strcat(result, temp_str);
        for (int i = 1; i < index; i++) {
            strcat(result, comma);
            sprintf(temp_str, "%d", intset_elements->intset[i + 1]);
            if (strlen(result) > allocate_len - 34) {
                allocate_len = allocate_len + allocate_len;
                result = (char *) realloc(result, sizeof(char) * allocate_len);
            }
            strcat(result, temp_str);
        }
        if (strlen(result) > allocate_len - 34) {
            allocate_len = allocate_len + allocate_len;
            result = (char *) realloc(result, sizeof(char) * allocate_len);
        }
        strcat(result, right);
    }
    return *result;

}

/*****************************************************************************
 * Input functions
 *****************************************************************************/

PG_FUNCTION_INFO_V1(intset_in);

Datum
intset_in(PG_FUNCTION_ARGS) {

    char *str = PG_GETARG_CSTRING(0);
    intSet *result;

    elog(NOTICE, "===================stt:%s ======= ", str);

    int allocate_len = 100;
    int *array_intset;
    int index = 0;
    int m, max;
    int i, index_element_num;
    int *arrayflag;
    const char delims[4] = "{, }";
    char *temp_result = NULL;
    if (!valid_str(str)) {
        ereport(ERROR,
        (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
         errmsg("invalid input syntax for type %s: \"%s\"",
                 "intSet", str)));
    }

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
        if (max < array_intset[m]) {
            max = array_intset[m];
        }
    }
    arrayflag = (int *) malloc(sizeof(int) * (max + 1));
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
    Sorting_element(array_intset, index_element_num);
    result = (intSet *) palloc(VARHDRSZ + (sizeof(int) * (index_element_num + 1)));
    SET_VARSIZE(result, VARHDRSZ + (sizeof(int) * (index_element_num + 1)));
    //assign the value of array_intset to the result
    // the fist one inset[0] of result means: the number of all valid elements
    // and the valid number is start at inset[1]
    result->intset[0] = index_element_num;
    for (int j = 0; j < index_element_num; j++) {
        result->intset[j + 1] = array_intset[j];
    }
//    result_inSet = psprintf("%d", result);
    free(arrayflag);
    free(array_intset);
//    free(result);
    PG_RETURN_POINTER(result);
}

/*****************************************************************************
 * Output functions
 *****************************************************************************/

PG_FUNCTION_INFO_V1(intset_out);

Datum
intset_out(PG_FUNCTION_ARGS) {
    intSet *intset_elements = (intSet *) PG_GETARG_POINTER(0);
    char *result;
    char *output;
    int allocate_len = 100;
    result =(char*) malloc(sizeof(char)*allocate_len);
    intset_2_cstring(result, intset_elements , allocate_len);
    output = psprintf("%s", result);
    free(result);
    PG_RETURN_CSTRING(output);

}



/*****************************************************************************
 * Contain: the intSet type: contains, a belongs to S
 *****************************************************************************/

PG_FUNCTION_INFO_V1(intset_contains);

Datum
intset_contains(PG_FUNCTION_ARGS) {
    int one_element = PG_GETARG_INT32(0);
    intSet *intset_elements = (intSet *) PG_GETARG_POINTER(1);

    int output_bool;
    int intset_elements_count;
    intset_elements_count = intset_elements->intset[0];

    if (intset_elements_count >= 1) {
        for (int i = 0; i < intset_elements_count; i++) {
            if (one_element == intset_elements->intset[i]) {
                output_bool = 1;
            } else {
                output_bool = 0;
            }
        }
    } else {
        output_bool = 0;
    }
    PG_RETURN_BOOL(output_bool);
}


/*****************************************************************************
 * Superset: left_contains_right-  A>=B
 *****************************************************************************/
PG_FUNCTION_INFO_V1(intset_supset);

Datum
intset_supset(PG_FUNCTION_ARGS) {
    intSet *intset_elements_1 = (intSet *) PG_GETARG_POINTER(0);
    intSet *intset_elements_2 = (intSet *) PG_GETARG_POINTER(1);
    int count_left;
    int count_right;
    int output;
    int *temp;
    int allocate_len = 100;
    int index, t;
    count_left = intset_elements_1->intset[0];
    count_right = intset_elements_2->intset[0];
    temp = (int *) malloc(sizeof(int) * allocate_len);
    index = 0;
    for (int i = 0; i < count_left; i++) {
        for (int j = 0; j < count_right; j++) {
            t = intset_elements_1->intset[i + 1] ^ intset_elements_2->intset[j + 1];
            if (t == 0) {
                if (index >= allocate_len) {
                    allocate_len = allocate_len * 2;
                    temp = (int *) realloc(temp, sizeof(int) * allocate_len);
                }
                temp[index] = intset_elements_1->intset[i + 1];
                index++;
            }
        }
    }
    // len(A) < len(B) ---false:0
    if (index != count_right) {
        output = 0;
    } else {
        if (index==0 && count_right ==0){
            output=1;
        }
        // len(A) >= len(B) ---check
        for (int i = 0; i < index; i++) {

            for (int j = 0; j < count_right; j++) {
                if (temp[i] != intset_elements_2->intset[j + 1]) {
                    output = 0;
                } else {
                    output = 1;
                }
            }
        }
    }
    free(temp);
    PG_RETURN_BOOL(output);
}


/*****************************************************************************
 *  Subset: left is contained by right; A <= B
 *****************************************************************************/

PG_FUNCTION_INFO_V1(intset_subset);

Datum
intset_subset(PG_FUNCTION_ARGS) {
    intSet *intset_elements_1 = (intSet *) PG_GETARG_POINTER(0);
    intSet *intset_elements_2 = (intSet *) PG_GETARG_POINTER(1);
    int count_left;
    int count_right;
    int output;
    int *temp;
    int allocate_len = 100;
    int index, t;
    count_left = intset_elements_1->intset[0];
    count_right = intset_elements_2->intset[0];
    temp = (int *) malloc(sizeof(int) * allocate_len);
    index = 0;
    for (int i = 0; i < count_left; i++) {
        for (int j = 0; j < count_right; j++) {
            t = intset_elements_1->intset[i + 1] ^ intset_elements_2->intset[j + 1];
            if (t == 0) {
                if (index >= allocate_len) {
                    allocate_len = allocate_len * 2;
                    temp = (int *) realloc(temp, sizeof(int) * allocate_len);
                }
                temp[index] = intset_elements_1->intset[i + 1];
                index++;
            }
        }
    }
    // len(A) < len(B) ---false:0
    if (index != count_left) {
        output = 0;
    } else {
        if (index==0 && count_left ==0 ){
            output =1;
        }
        // len(A) >= len(B) ---check
        for (int i = 0; i < index; i++) {

            for (int j = 0; j < count_right; j++) {
                if (temp[i] != intset_elements_2->intset[j + 1]) {
                    output = 0;
                } else {
                    output = 1;
                }
            }
        }
    }
    free(temp);
    PG_RETURN_BOOL(output);
}

/*****************************************************************************
 * # the intSet type: cardinality
 *****************************************************************************/

PG_FUNCTION_INFO_V1(intset_num_elements);

Datum
intset_num_elements(PG_FUNCTION_ARGS) {
    intSet *intset_elements = (intSet *) PG_GETARG_POINTER(0);
    int result;
    result = intset_elements->intset[0];
    PG_RETURN_INT32(result);
}

/*****************************************************************************
 * the intSet type: equal
 *****************************************************************************/
PG_FUNCTION_INFO_V1(intset_equal);

Datum
intset_equal(PG_FUNCTION_ARGS) {
    intSet *intset_elements_1 = (intSet *) PG_GETARG_POINTER(0);
    intSet *intset_elements_2 = (intSet *) PG_GETARG_POINTER(1);
    int count_left;
    int count_right;
    int i;
    int output_bool;
    // equal = true =1
    // not equal = false =0
    count_left = intset_elements_1->intset[0];
    count_right = intset_elements_2->intset[0];
    if (count_left == count_right) {
        for (i = 0; i < count_left; i++) {
            if (intset_elements_1->intset[i + 1] != intset_elements_2->intset[i + 1]) {
                output_bool = 0;
            } else {
                output_bool = 1;
            }
        }
    } else {
        output_bool = 0;
    }
    PG_RETURN_BOOL(output_bool);
}


/*****************************************************************************
 * the intSet type: not equal
 *****************************************************************************/
PG_FUNCTION_INFO_V1(intset_not_equal);

Datum

intset_not_equal(PG_FUNCTION_ARGS) {
    intSet *intset_elements_1 = (intSet *) PG_GETARG_POINTER(0);
    intSet *intset_elements_2 = (intSet *) PG_GETARG_POINTER(1);
    int count_left;
    int count_right;
    int i;
    int output_bool;
    // not equal = 1 = true
    // equal = 0 = false
    count_left = intset_elements_1->intset[0];
    count_right = intset_elements_2->intset[0];

    if (count_left != count_right) {
        // not equal = false
        output_bool = 1;
    } else {
        // case2: two insets have the same length
        for (i = 0; i < count_left; i++) {

            if (intset_elements_1->intset[i + 1] != intset_elements_2->intset[i + 1]) {
                output_bool = 1;

            } else {
                output_bool = 0;
            }
        }
    }
    PG_RETURN_BOOL(output_bool);
}


/*****************************************************************************
 * Set intersection  ----A and B
 *****************************************************************************/
PG_FUNCTION_INFO_V1(intset_intersection);

Datum
intset_intersection(PG_FUNCTION_ARGS) {
    intSet *intset_elements_1 = (intSet *) PG_GETARG_POINTER(0);
    intSet *intset_elements_2 = (intSet *) PG_GETARG_POINTER(1);
    intSet *output;
    int count_left;
    int count_right;
    int *diff_inset_temp;
    int allocate_len = 100;
    int index;
    int t;
    diff_inset_temp = (int *) malloc(sizeof(int) * allocate_len);
    count_left = intset_elements_1->intset[0];
    count_right = intset_elements_2->intset[0];
    index = 0;
    for (int i = 0; i < count_left; i++) {
        for (int j = 0; j < count_right; j++) {
            t = intset_elements_1->intset[i + 1] ^ intset_elements_2->intset[j + 1];
            if (t == 0) {
                if (index >= allocate_len) {
                    allocate_len = allocate_len * 2;
                    diff_inset_temp = (int *) realloc(diff_inset_temp, sizeof(int) * allocate_len);
                }
                diff_inset_temp[index] = intset_elements_1->intset[i + 1];
                index++;
            }
        }
    }
    // sorting if necessary
    Sorting_element(diff_inset_temp, index);
    // allocate address for tem_result_diff
    output = (intSet *) palloc(VARHDRSZ + (sizeof(int) * (index + 1)));
    SET_VARSIZE(output, VARHDRSZ + (sizeof(int) * (index + 1)));

    output->intset[0] = index;

    for (int i = 0; i < index; i++) {
        output->intset[i + 1] = diff_inset_temp[i];
    }
    free(diff_inset_temp);
    PG_RETURN_POINTER(output);
}

/*****************************************************************************
 * Set union====  or --- A+B
 *****************************************************************************/
PG_FUNCTION_INFO_V1(intset_union);

Datum
intset_union(PG_FUNCTION_ARGS) {
    intSet *intset_elements_1 = (intSet *) PG_GETARG_POINTER(0);
    intSet *intset_elements_2 = (intSet *) PG_GETARG_POINTER(1);
    intSet *output;
    int count_left;
    int count_right;
    int *temp_output;
    int allocate_len = 100;
    int index, max;
    int *arrayflag;
    int i, index_element_num;
    count_left = intset_elements_1->intset[0];
    count_right = intset_elements_2->intset[0];
    temp_output = (int *) malloc(sizeof(int) * allocate_len);
    // putting the set A and B to together
    index = 0;
    for (int i = 0; i < count_left; i++) {
        if (index >= allocate_len) {
            allocate_len = allocate_len * 2;
            temp_output = (int *) realloc(temp_output, sizeof(int) * allocate_len);
        }
        temp_output[index] = intset_elements_1->intset[i + 1];
        index++;
    }
    for (int j = 0; j < count_right; j++) {
        if (index >= allocate_len) {
            allocate_len = allocate_len * 2;
            temp_output = (int *) realloc(temp_output, sizeof(int) * allocate_len);
        }
        temp_output[index] = intset_elements_2->intset[j + 1];
        index++;
    }
    // Find the maximum value in the current array_set
    max = temp_output[0];
    for (int m = 1; m < index; m++) {
        if (max < temp_output[m])
            max = temp_output[m];
    }
    arrayflag = (int *) malloc(sizeof(int) * (max + 1));
    // delete the duplicated elements(number)
    for (i = 0; i <= max; i++) {
        arrayflag[i] = false;
    }
    //delete Algorithm
    for (i = 0; i < index; i++) {
        arrayflag[temp_output[i]] = temp_output[i];
    }
    //Take out the effective number
    for (i = 0, index_element_num = 0; i <= max; i++) {
        if (arrayflag[i] != false) {
            temp_output[index_element_num++] = arrayflag[i];
        }
    }
    Sorting_element(temp_output, index_element_num);
    output = (intSet *) palloc(VARHDRSZ + (sizeof(int) * (index_element_num + 1)));
    SET_VARSIZE(output, VARHDRSZ + (sizeof(int) * (index_element_num + 1)));

    output->intset[0] = index_element_num;

    for (int i = 0; i < index_element_num; i++) {
        output->intset[i + 1] = temp_output[i];
    }
    free(temp_output);
    free(arrayflag);
    PG_RETURN_POINTER(output);
}

/*****************************************************************************
 * Set disjunction====  (A-B) + (B-A)
 *****************************************************************************/
PG_FUNCTION_INFO_V1(intset_disjunction);

Datum
intset_disjunction(PG_FUNCTION_ARGS) {
    intSet *intset_elements_1 = (intSet *) PG_GETARG_POINTER(0);
    intSet *intset_elements_2 = (intSet *) PG_GETARG_POINTER(1);

    intSet *output;
    int count_left;
    int count_right;
    int i, j;
    int flag2, index2 = 0;
    int flag, index = 0;
    int *temp;
    int *temp2;
    int *temp_total;
    int allocate_len = 100;
    int index_total = 0;

    count_left = intset_elements_1->intset[0];
    count_right = intset_elements_2->intset[0];

    temp2 = (int *) malloc(sizeof(int) * allocate_len);

    for (i = 0; i < count_right; i++) {
        flag2 = 1;
        for (j = 0; j < count_left; j++) {
            if (intset_elements_2->intset[i + 1] == intset_elements_1->intset[j + 1])
                flag2 = 0;
        }
        if (flag2) {
            temp2[index2] = intset_elements_2->intset[i + 1];
            index2++;
        }
    }

    temp = (int *) malloc(sizeof(int) * allocate_len);
    for (i = 0; i < count_left; i++) {
        flag = 1;
        for (j = 0; j < count_right; j++) {
            if (intset_elements_1->intset[i + 1] == intset_elements_2->intset[j + 1]) {
                flag = 0;
            }
        }
        if (flag) {
            if (index >= allocate_len) {
                allocate_len = allocate_len + allocate_len;
                temp = (int *) realloc(temp, sizeof(int) * allocate_len);
            }
            temp[index] = intset_elements_1->intset[i + 1];
            index++;
        }
    }
    // index_total
    temp_total = (int *) malloc(sizeof(int) * allocate_len);
    for (int m = 0; m < index; m++) {
        if (index_total >= allocate_len) {
            allocate_len = allocate_len * 1.5;
            temp_total = (int *) realloc(temp_total, sizeof(int) * allocate_len);
        }
        temp_total[index_total] = temp[m];
        index_total++;
    }
    for (int n = 0; n < index2; n++) {
        if (index_total >= allocate_len) {
            allocate_len = allocate_len * 1.5;
            temp_total = (int *) realloc(temp_total, sizeof(int) * allocate_len);
        }
        temp_total[index_total] = temp2[n];
        index_total++;
    }
    Sorting_element(temp_total, index_total);

    output = (intSet *) palloc(VARHDRSZ + (sizeof(int) * (index_total + 1)));
    SET_VARSIZE(output, VARHDRSZ + (sizeof(int) * (index_total + 1)));
    output->intset[0] = index_total;
    for (int i = 0; i < index_total; i++) {
        output->intset[i + 1] = temp_total[i];
    }
    free(temp);
    free(temp2);
    free(temp_total);
    PG_RETURN_POINTER(output);
}

/*****************************************************************************
 * Set difference====  (A-B)
 *****************************************************************************/

PG_FUNCTION_INFO_V1(intset_diff);

Datum
intset_diff(PG_FUNCTION_ARGS) {
    intSet *intset_elements_1 = (intSet *) PG_GETARG_POINTER(0);
    intSet *intset_elements_2 = (intSet *) PG_GETARG_POINTER(1);
    intSet *output;
    int count_left;
    int count_right;
    int flag;
    int index = 0;
    int *temp;
    int allocate_len = 100;
    int i, j;
    count_left = intset_elements_1->intset[0];
    count_right = intset_elements_2->intset[0];

    temp = (int *) malloc(sizeof(int) * allocate_len);

    for (i = 0; i < count_left; i++) {
        flag = 1;
        for (j = 0; j < count_right; j++) {
            if (intset_elements_1->intset[i + 1] == intset_elements_2->intset[j + 1]) {
                flag = 0;
            }
        }
        if (flag) {
            if (index >= allocate_len) {
                allocate_len = allocate_len + allocate_len;
                temp = (int *) realloc(temp, sizeof(int) * allocate_len);
            }
            temp[index] = intset_elements_1->intset[i + 1];
            index++;
        }
    }
    Sorting_element(temp, index);
    output = (intSet *) palloc(VARHDRSZ + (sizeof(int) * (index + 1)));
    SET_VARSIZE(output, VARHDRSZ + (sizeof(int) * (index + 1)));
    output->intset[0] = index;
    for (int i = 0; i < index; i++) {
        output->intset[i + 1] = temp[i];
    }
    free(temp);
    PG_RETURN_POINTER(output);
}
