// bits.h ... interface to functions on bit-strings
// part of signature indexed files
// See bits.c for details of functions
// Written by John Shepherd, March 2019

#ifndef BITS_H
#define BITS_H 1

typedef struct _BitsRep *Bits;

#include "defs.h"
#include "page.h"


Bits newBits(int);
void freeBits(Bits);

// check if the bit at position is 1
Bool bitIsSet(Bits, int);

// check whether one Bits b1 is a subset of Bits b2
Bool isSubset(Bits, Bits);

// set the bit at position to 1
void setBit(Bits, int);

// set all bits to 1
void setAllBits(Bits);

// set the bit at position to 0
void unsetBit(Bits, int);

// set all bits to 0
void unsetAllBits(Bits);


void andBits(Bits, Bits);
void orBits(Bits, Bits);


// left-shift ... b1 = b1 << n
// negative n gives right shift

void shiftBits(Bits, int);
void getBits(Page, Offset, Bits);


void putBits(Page, Offset, Bits);


void showBits(Bits);


#endif
