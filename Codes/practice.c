//
// Created by 郭昊瑾 on 2021/3/16.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))


void bubble_sort(int arr[], int len) {
    int i, j, temp;
    for (i = 0; i < len - 1; i++)
        for (j = 0; j < len - 1 - i; j++)
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
}


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

//int main() {
////    int arr[] = { 22, 34, 3, 32, 82, 55, 89, 50, 37, 5, 64, 35, 9, 70 };
////
////    int len = (int) sizeof(arr) / sizeof(*arr);
////    SelectionSort(arr, len);
////    int i;
////    for (i = 0; i < len; i++)
////        printf("%d ", arr[i]);
////    printf("\n");
////
////

int arraydiff(int *A, int max, int len) {
    int arrayflag[max + 1];
    int i, j;
    //初始化标志数组
    for (i = 0; i <= max; i++) {
        arrayflag[i] = false;
    }
    //剔除算法
    for (i = 0; i < len; i++) {
        arrayflag[A[i]] = A[i];
    }
    //取出有效数
    for (i = 0, j = 0; i <= max; i++) {
        if (arrayflag[i] != false) {
            A[j++] = arrayflag[i];
        }
    }
    return j;
}

int cmp(const void *a, const void *b) //排序规则
{
    return *(int *) a - *(int *) b;
}


static int split_String(char str[], char delims[]) {

    char *result = NULL;

    int allocate_len = 100;
    int *array;
    int index = 0;


    array = (int *) malloc(sizeof(int) * allocate_len);
    result = strtok(str, delims);

    while (result != NULL) {
        if (index == allocate_len) {
            allocate_len = allocate_len * 2;
            array = (int *) realloc(array, sizeof(int) * allocate_len);
        }
        array[index] = atoi(result);
        result = strtok(NULL, delims);
        index++;
    }

    int m, max;
    max = array[0];

    for (m = 1; m < index; m++) {
        if (max < array[m])
            max = array[m];
    }

    printf("最大元素为 %d\n", max);


    int *arrayflag;
    int i, index_element_num;
    //初始化标志数组

    arrayflag = malloc(sizeof(int) * (max + 1));

    for (i = 0; i <= max; i++) {
        arrayflag[i] = false;
    }
    //剔除算法
    for (i = 0; i < index; i++) {
        arrayflag[array[i]] = array[i];
    }
    //取出有效数
    for (i = 0, index_element_num = 0; i <= max; i++) {
        if (arrayflag[i] != false) {
            array[index_element_num++] = arrayflag[i];
        }
    }
    printf("%d\n", index_element_num);


    SelectionSort(array, index_element_num);
    for (int i = 0; i < index_element_num; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    int aa;
    aa = sizeof(array) / sizeof(array[0]);
    printf("数组array元素个数为：%d\n", aa);


    free(arrayflag);
    free(array);

}


int main() {

    int a[10] = {9,1, 2, 3, 4, 5, 6, 7, 8, 9};
    int b[7] = {6,1, 2, 3,11,22,13};


    int count_left;
    int count_right;
    int i,j;
    int flag2,index2 = 0;
    int flag,index = 0;
    int *temp;
    int *temp2;
    int *temp_total;
    int allocate_len=100;
    int index_total=0;
    count_left =a[0];
    count_right=b[0];
    temp2= (int *)malloc(sizeof(int) * allocate_len);
    for(i = 0;i < count_right;i++)
    {
        flag2 = 1;
        for(j = 0;j < count_left;j++)
        {
            if(b[i+1] == a[j+1])
                flag2 = 0;
        }
        if(flag2)
        {
            temp2[index2] = b[i+1];
            index2++;
        }
    }

    temp = (int *)malloc(sizeof(int) * allocate_len);
    for(i = 0; i < count_left; i++) {
        flag = 1;
        for (j = 0; j < count_right; j++) {
            if (a[i + 1] == b[j + 1]) {
                flag = 0;
            }
        }
        if (flag) {
            if (index >= allocate_len) {
                allocate_len = allocate_len + allocate_len;
                temp = (int *) realloc(temp, sizeof(int) * allocate_len);
            }
            temp[index] = a[i + 1];
            index++;
        }
    }

    // index_total
    temp_total = (int *)malloc(sizeof(int) * allocate_len);
    for (int m=0; m<index; m++){
        if(temp_total>=allocate_len){
            allocate_len = allocate_len * 1.5 ;
            temp_total =(int*)realloc(temp_total, sizeof(int)*allocate_len);
        }
        temp_total[index_total] = temp[m];
        index_total++;
    }
    for(int n=0; n<index2; n++){
        if(temp_total>=allocate_len){
            allocate_len = allocate_len * 1.5 ;
            temp_total =(int*)realloc(temp_total, sizeof(int)*allocate_len);
        }
        temp_total[index_total] = temp2[n];
        index_total++;
    }
    SelectionSort(temp_total, index_total);

    printf("-----%d\n",index_total);
    for(i = 0;i < index_total;i++)
        printf("%d ",temp_total[i]);
    printf("\n");


    free(temp);
    free(temp2);
    free(temp_total);


    return 0;

}