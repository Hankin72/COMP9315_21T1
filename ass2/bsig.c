// bsig.c ... functions on Tuple Signatures (bsig's)
// part of signature indexed files
// Written by John Shepherd, March 2019

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "bsig.h"
#include "psig.h"

///  COMP9315_21T1_ass02  Haojin Guo z5216214

void findPagesUsingBitSlices(Query q)
{
	assert(q != NULL);
	///TODO

    Reln r = q->rel;
    Bits query_sig = makePageSig(r, q->qstring);

    setAllBits(q->pages);

    Count pm = psigBits(r);  // width of page signature (#bits)
    Count bc = maxBsigsPP(r);  // max bit-slices per page ; (REL)->params.bsigPP
    Count bm = bsigBits(r);   // width of bit-slice (=maxpages)

    // number of page signatures (psigs) : #define nPsigs(REL)   (REL)->params.npsigs
    PageID b_slice_page_id = -1;
    Page b_slice_page;
    for (int page_id =0; page_id < pm; page_id++){
        File b_file = bsigFile(r);
        Bits check_b_sig = newBits(bm);
        if (bitIsSet(query_sig, page_id) == TRUE){
            // get the position of bit slice
            int b_slice = page_id/ bc;
            int b_position_offset = page_id %bc;

            if (b_slice_page == NULL || b_slice != b_slice_page_id ){

                b_slice_page_id = b_slice;
                // get i'th bit slice from bsigFile
                b_slice_page = getPage(b_file, b_slice_page_id);

                q->nsigpages ++;  // how many signature pages read
            }
            getBits(b_slice_page, b_position_offset, check_b_sig);
            //  nPsigs(REL)      (REL)->params.npsigs //// number of page signatures (psigs)
            // bsigNpages; // number of bsig pages
            for (int i=0; i < nPsigs(r); i++ ){
                if (bitIsSet(check_b_sig, i) == FALSE){
                    unsetBit(q->pages,i );
                }
            }


            q->nsigs++;   // how many signatures read
        }
        freeBits(check_b_sig);
    }
    free(b_slice_page);
}






