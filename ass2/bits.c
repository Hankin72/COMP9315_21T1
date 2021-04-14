// bits.c ... functions on bit-strings
// part of signature indexed files
// Bit-strings are arbitrarily long byte arrays
// Least significant bits (LSB) are in array[0]
// Most significant bits (MSB) are in array[nbytes-1]

// Written by John Shepherd, March 2019

#include <assert.h>
#include "defs.h"
#include "bits.h"
#include "page.h"

///  COMP9315_21T1  Haojin Guo z5216214


typedef struct _BitsRep {
    Count nbits;          // how many bits
    Count nbytes;          // how many bytes in array
    Byte bitstring[1];  // array of bytes to hold bits
    // actual array size is nbytes
} BitsRep;

// create a new Bits object
Bits newBits(int nbits) {
    Count nbytes = iceil(nbits, 8);
    Bits new = malloc(2 * sizeof(Count) + nbytes);
    new->nbits = nbits;
    new->nbytes = nbytes;
    memset(&(new->bitstring[0]), 0, nbytes);
    return new;
}

// release memory associated with a Bits object
void freeBits(Bits b) {
    ///TODO
    free(b);
}

// check if the bit at position is 1
Bool bitIsSet(Bits b, int position) {
    assert(b != NULL);
    assert(0 <= position && position < b->nbits);
    ///TODO
    // left shift = plus + 2^n
    // find the position of the array(which bytes)
    // find the offset position in certain byte
    int array_position = position / 8;
    int offset_position = position % 8;
    if ((b->bitstring[array_position] & (1 << offset_position))) {
        return TRUE;
    }
    return FALSE; // remove this
}

// check whether one Bits b1 is a subset of Bits b2
Bool isSubset(Bits b1, Bits b2) {
    assert(b1 != NULL && b2 != NULL);
    assert(b1->nbytes == b2->nbytes);
    ///TODO
    for (int byte_index = 0; byte_index < b1->nbytes; byte_index++) {
        b2->bitstring[byte_index] &= b1->bitstring[byte_index];
        if ((b2->bitstring[byte_index]) == (b1->bitstring[byte_index])) {
            return TRUE;
        }
    }
    return FALSE; // remove this
}

// set the bit at position to 1
void setBit(Bits b, int position) {
    assert(b != NULL);
    assert(0 <= position && position < b->nbits);
    ///TODO
    int array_position;
    int offset_position;
    // find the position of the array(which bytes)
    // find the offset position in certain byte
    array_position = position / 8;
    offset_position = position % 8;
    // set the position offset to 1, eg: 00010000 , then do or operation
    b->bitstring[array_position] |= (1 << offset_position);
}

// set all bits to 1
void setAllBits(Bits b) {
    assert(b != NULL);
    ///TODO
    // ch &= 0xff; /* 或者 ch &= 0377; */
    //oxff的二进制形式是11111111，八进制形式是0377。
    for (int byte_index = 0; byte_index < b->nbytes; byte_index++) {
        b->bitstring[byte_index] |= 0377;
    }
}

// set the bit at position to 0
void unsetBit(Bits b, int position) {
    assert(b != NULL);
    assert(0 <= position && position < b->nbits);
    ///TODO
    int array_position;
    int offset_position;
    array_position = position / 8;
    offset_position = position % 8;
    b->bitstring[array_position] &= (~(1 << offset_position));
}

// set all bits to 0
void unsetAllBits(Bits b) {
    assert(b != NULL);
    ///TODO
    for (int byte_index = 0; byte_index < b->nbytes; byte_index++) {
        b->bitstring[byte_index] &= 0000;
    }
}

// bitwise AND ... b1 = b1 & b2
void andBits(Bits b1, Bits b2) {
    assert(b1 != NULL && b2 != NULL);
    assert(b1->nbytes == b2->nbytes);
    ///TODO
    for (int byte_index = 0; byte_index < b1->nbytes; byte_index++) {
        b1->bitstring[byte_index] &= b2->bitstring[byte_index];
    }
}

// bitwise OR ... b1 = b1 | b2
void orBits(Bits b1, Bits b2) {
    assert(b1 != NULL && b2 != NULL);
    assert(b1->nbytes == b2->nbytes);
    ///TODO
    for (int byte_index = 0; byte_index < b1->nbytes; byte_index++) {
        b1->bitstring[byte_index] |= b2->bitstring[byte_index];
    }
}

// left-shift ... b1 = b1 << n
// negative n gives right shift
void shiftBits(Bits b, int n) {
    /// TODO
    for (int byte_index = 0; byte_index < b->nbytes; byte_index++) {
        if (n >= 0) {
            b->bitstring[byte_index] = b->bitstring[byte_index] << n;
        } else {
            b->bitstring[byte_index] = b->bitstring[byte_index] >> abs(n);
        }
    }
}

// get a bit-string (of length b->nbytes)
// from specified position in Page buffer
// and place it in a BitsRep structure
void getBits(Page p, Offset pos, Bits b) {
    ///TODO
    Byte *temp = addrInPage(p, pos, b->nbytes);
    memcpy(b->bitstring, temp, b->nbytes);
}

// copy the bit-string array in a BitsRep
// structure to specified position in Page buffer
void putBits(Page p, Offset pos, Bits b) {
    ///TODO
    Byte *temp = addrInPage(p, pos, b->nbytes);
    memcpy(temp, b->bitstring, b->nbytes);
}

// show Bits on stdout
// display in order MSB to LSB
// do not append '\n'
void showBits(Bits b) {
    assert(b != NULL);
    //printf("(%d,%d)",b->nbits,b->nbytes);
    for (int i = b->nbytes - 1; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
            Byte mask = (1 << j);
            if (b->bitstring[i] & mask)
                putchar('1');
            else
                putchar('0');
        }
    }
}
