// Test Bits ADT

#include <stdio.h>
#include "defs.h"
#include "reln.h"
#include "tuple.h"
#include "bits.h"

int main(int argc, char **argv)
{
	Bits b = newBits(60);
	printf("t=0: "); showBits(b); printf("\n");

	setBit(b, 5);
	printf("t=1: "); showBits(b); printf("\n");

	setBit(b, 0);
	setBit(b, 50);
	setBit(b, 59);
	printf("t=2: "); showBits(b); printf("\n");
	if (bitIsSet(b,5)) printf("Bit 5 is set\n");
	if (bitIsSet(b,10)) printf("Bit 10 is set\n");
	printf("\n");

	setAllBits(b);
	printf("t=3: "); showBits(b); printf("\n");


	unsetBit(b, 40);
	printf("t=4: "); showBits(b); printf("\n");

	if (bitIsSet(b,20)) printf("Bit 20 is set\n");
	if (bitIsSet(b,40)) printf("Bit 40 is set\n");
	if (bitIsSet(b,50)) printf("Bit 50 is set\n");

    unsetAllBits(b);
    printf("t=5: "); showBits(b); printf("\n");

	setBit(b, 59);
    printf("t=6: "); showBits(b); printf("\n\n");



    Bits b1 = newBits(8);
    Bits b2 = newBits(8);

    setBit(b1, 0);
    setBit(b1, 1);
    setBit(b1, 0);
    setBit(b1, 5);
    printf("t=7: "); showBits(b1); printf("\n\n");
    setBit(b2, 0);
    setBit(b2, 1);
    setBit(b2, 2);
    setBit(b2, 3);
    setBit(b2, 4);
    printf("t=8:b2===== "); showBits(b2); printf("\n\n");


//    if (isSubset(b1,b2)){
//        printf("TRUE\n");
//    } else{
//        printf("FALSE\n");
//    }


    printf("t=10=b2 shift==: "); shiftBits(b2, 1); showBits(b2); printf("\n\n");



//    printf("t=8: "); andBits(b1,b2); showBits(b1); printf("\n\n");
    freeBits(b);
    freeBits(b1);
    freeBits(b2);
	return 0;
}
