// Test Bits ADT

#include <stdio.h>
#include "defs.h"
#include "reln.h"
#include "tuple.h"
#include "bits.h"

int main(int argc, char **argv)
{



    Bits b1= newBits(10);
    setBit(b1, 1);
    setBit(b1, 9);

    printf("b1=====7: "); showBits(b1); printf("\n\n");

    Bits b2 = newBits(10);


    setBit(b2, 1);
    setBit(b2, 9);
    setBit(b2, 2);
    setBit(b2, 3);

//    int i;
//    if (isSubset(b1,b2)){
//        i =1 ;
//    }else{
//        i=0;
//    }
//    printf("%d\n",i);
//

    printf("b2=====7: "); showBits(b2); printf("\n\n");


    printf("t=b2=====7: "); orBits(b1,b2); showBits(b1); printf("\n\n");


    printf("t=8:b2===shift===="); shiftBits(b2, 5); showBits(b2); printf("\n\n");

    freeBits(b2);
	return 0;
}
