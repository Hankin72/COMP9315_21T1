// tsig.c ... functions on Tuple Signatures (tsig's)
// part of signature indexed files
// Written by John Shepherd, March 2019

#include <unistd.h>
#include <string.h>
#include "defs.h"
#include "tsig.h"
#include "reln.h"
#include "hash.h"
#include "bits.h"

///  COMP9315_21T1  Haojin Guo z5216214



// make a tuple signature

Bits codeword(){
    return 0;

}


Bits makeTupleSig(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL);
	///TODO









	return NULL;
}

// find "matching" pages using tuple signatures

void findPagesUsingTupSigs(Query q)
{
	assert(q != NULL);
	///TODO

	setAllBits(q->pages); // remove this

	// The printf below is primarily for debugging
	// Remove it before submitting this function
	printf("Matched Pages:"); showBits(q->pages); putchar('\n');
}
