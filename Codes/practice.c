//
// Created by 郭昊瑾 on 2021/3/16.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <regex.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))


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
    return *arr;
}


// function used to compile the regrex and execute it
static bool regexMatch(char * str, char * regexPattern) {
    regex_t reg_check;
    int output = false;
    int cflags;
    cflags = REG_EXTENDED ;

    // compile the regex
    if(regcomp(&reg_check, regexPattern, cflags)){
        return false;
    }
    // execute the regex
    if(regexec(&reg_check, str, 0, NULL, 0)) {
        output = false;
    }else{
        output = true;
    }
    // free the regex
    regfree(&reg_check);
    return output;
}




static int intset_2_cstring(char *result, int *a, int allocate_len){
    int index;
    char left[10]="{";
    char right[10] ="}";
    char comma[10] =",";
    char temp_str[32];
    index = a[0];

    if (index== 0){
        strcat(result, left);
        strcat(result, right);

    }
    if (index > 0) {
        strcat(result, left);
        sprintf(temp_str, "%d", a[1]);
        strcat(result, temp_str);
        for (int i = 1; i < index; i++) {
            strcat(result, comma);
            sprintf(temp_str, "%d", a[i + 1]);
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

int main() {


    char a[1024]="{0}";

    int b[]={0};


    int allocate_len = 100;
    int *array_intset;
    int arrayflag[100];
    int max;
    int m, index_element_num;
    int i;
    int index = 0;


    const char delims[10] = "{ ,}";
    char *temp_result = NULL;

    array_intset = (int *) malloc(sizeof(int) * allocate_len);
    temp_result = strtok(a, delims);
    while (temp_result != NULL) {
        if (index >= allocate_len) {
            allocate_len = 2 * allocate_len;
            array_intset = (int *) realloc(array_intset, sizeof(int) * allocate_len);
        }
        // string to number
        array_intset[index] = atoi(temp_result);
        temp_result = strtok(NULL, delims);
        index++;
    }

//    printf("%d\n",index);
//    printf("%d\n",array_intset[0]);
////    printf("%d\n",array_intset[0]);

    if (index >1 ){
        max = array_intset[0];
        for (m = 1; m < index; m++) {
            if (max < array_intset[m]) {
                max = array_intset[m];
            }
        }

        // delete the duplicated elements(number)
        for ( i = 0; i <= max; i++) {
            arrayflag[i] = false;
        }
        //delete Algorithm
        for ( i = 0; i < index; i++) {
            arrayflag[array_intset[i]] = array_intset[i];
        }
        //Take out the effective number
        for (i = 0, index_element_num = 0; i <= max; i++) {
            if (arrayflag[i] != false) {
                array_intset[index_element_num++] = arrayflag[i];
            }
        }
        index = index_element_num;
        SelectionSort(array_intset,index);
    }


    int result[100];

    result[0]= index;

    for (int j = 0; j < index+1 ; j++) {
        result[j + 1] = array_intset[j];
    }
    printf("----------");
    printf("%d\n",index);

    printf("%d\n",result[0]);
    printf("%d\n",result[1]);



    char output[100];
    int abc[50] = {1, 0};
    intset_2_cstring(output,result, allocate_len);
    printf("%s\n", output);

///////////////////////////

    char *str = "{0} ";
    char *str2 ="  {100000 , 00199 ,1,2, ,    1000,0}   ";

    int match_result ;

    char *re =  "([ ])*\\{([ ])*\\}([ ])*|([ ])*\\{([ ])*((([ ])*([0-9]*)([ ])*)\\,([ ])*)*(([ ])*(([ ])*([0-9]*)([ ])*))\\}([ ])*";

    match_result = regexMatch(str,re);
    if (match_result == false){
        printf("false\n");
    }else{
        printf("true\n");
    }


    free(array_intset);

    return 0;

}