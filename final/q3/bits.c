// bits.c ... functions on bit-strings

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "bits.h"
#include "util.h"

#define BITS_PER_WORD 32

typedef unsigned int Count;  // non-negative integer
typedef unsigned int Word;   // unsigned 32-bit integer value

typedef struct _BitsRep {
	Count  nbits;		  // how many bits

	Count  nwords;		  // how many Words in array
	Word   bitstring[1];  // array of Words to hold bits
	                      // actual array size is nwords
} BitsRep;

// create a new Bits object

Bits newBits(int nbits)
{
	Count nwords = iceil(nbits,BITS_PER_WORD);
	Bits new = malloc(2*sizeof(Count) + nwords*sizeof(Word));
	assert(new != NULL);
	new->nbits = nbits;
	new->nwords = nwords;
	memset(&(new->bitstring[0]), 0x00, nwords*sizeof(Word));
	return new;
}

// release memory associated with a Bits object

void freeBits(Bits b)
{
	free(b);
}


// XXX put any helper functions here XXX


// set the bit at position to 1

void setBit(Bits b, int position)
{
	assert(b != NULL);
	assert(0 <= position && position < b->nbits);
	// XXX complete this function XXX
	///TODO
    int array_position;
    int offset_position;
    // find the position of the array(which bytes)
    // find the offset position in certain byte
    array_position = position / 32;
    offset_position = position % 32;
    // set the position offset to 1, eg: 00010000 , then do or operation
    b->bitstring[array_position] |= (1 << offset_position);

}

// set all bits to 1
// including possibly some bits beyond the top-end of the bit-string

void setAllBits(Bits b)
{
	assert(b != NULL);
	memset(&(b->bitstring[0]), 0xFF, b->nwords*sizeof(Word));
}

// set the bit at position to 0

void unsetBit(Bits b, int position)
{
	assert(b != NULL);
	assert(0 <= position && position < b->nbits);

	// XXX complete this function XXX
	/// TODO
    int array_position;
    int offset_position;
    array_position = position / 32;
    offset_position = position % 32;
    b->bitstring[array_position] &= (~(1 << offset_position));

}

// set all bits to 0

void unsetAllBits(Bits b)
{
	assert(b != NULL);
	memset(&(b->bitstring[0]), 0x00, b->nwords*sizeof(Word));
}

// show Bits on stdout
// display in order MSB to LSB
// do not append '\n'

void showBits(Bits b){
	assert(b != NULL);
    ///TODO
	// XXX complete this function XXX
//    printf("(%d,%d)",b->nbits,b->nwords);
    int n_words;
//    int bit_string;
    int n_bits;

    n_bits =b->nbits;
    n_words = b->nwords;
//    bit_string =b->bitstring;

    int bit_pos_i;

    bit_pos_i = (n_bits-1) % 32;

    while(bit_pos_i >=0){
        printf("%d",b->bitstring[n_words-1] >>  bit_pos_i & 1 );
        bit_pos_i --;
    }

    int w_pos_j;
    w_pos_j = n_words-2;
    while(w_pos_j >= 0){
        for (int m=31; m >= 0; m--){
            printf("%d",b->bitstring[w_pos_j] >> m &1);
        }
        w_pos_j --;
    }

}
