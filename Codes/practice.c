//
// Created by 郭昊瑾 on 2021/3/16.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <regex.h>
#include<math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))


int main() {
    printf("hello world\n");

    int pm=6;
    int index = 0;

    int m = 32;
    int n = 4;

    int k;

    for (int i = 0; i < n;i++){
        k = rand() % m;

        printf("%d------\n", k);

    }

    return 0;


}
