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

///  COMP9315_21T1_ass02  Haojin Guo z5216214

// make a tuple signature
Bits codeword_simc(char *attribute_value, int m, int k) {
    int nbits = 0;
    Bits code_word = newBits(m);
    srandom(hash_any(attribute_value, strlen(attribute_value)));
    while (nbits < k) {
        int i = random() % m;
        if (bitIsSet(code_word, i) == FALSE) {
            setBit(code_word, i);
            nbits++;
        }
    }
    return code_word;
}


Bits codeword_catc(char *attribute_value, int m, int m_catc, int k) {
    int nbits = 0;
    Bits code_word = newBits(m);
    srandom(hash_any(attribute_value, strlen(attribute_value)));
    while (nbits < k) {
        int i = random() % m_catc;
        if (bitIsSet(code_word, i) == FALSE) {
            setBit(code_word, i);
            nbits++;
        }
    }
    return code_word;
}


Bits makeTupleSig(Reln r, Tuple t) {
    assert(r != NULL && t != NULL);
    ///TODO
    // tsigBits(r) = r->  (REL)->params.tm :  width of tuple signature (#bits)
    // make
    int t_m = tsigBits(r);    // width of tuple signature (#bits)
    int t_k = codeBits(r);  // bits set per attribute
    int n = nAttrs(r);   //  number of attributes  // (REL)->params.nattrs

    char sig_type = sigType(r);
    Bits t_sig = newBits(t_m);
    // get tuple values from relation, and put it in  array and return it
    char **tuple_value = tupleVals(r, t);

    int m_u1 = t_m / n + t_m % n;  //CATC for attribute1: length of attri1;
    int m_u1_k = m_u1 / 2;

    int m_ur = t_m / n;     // CATC for attribute1: length of attri;
    int m_ur_k = m_ur / 2;

    int i = 0;
    while (i < n) {
        if (strcmp(tuple_value[i], "?\0") != 0) {
            if (sig_type == 's') {
                Bits cw_simc = codeword_simc(tuple_value[i], t_m, t_k);
                orBits(t_sig, cw_simc);
                freeBits(cw_simc);
            } else {
                if (sig_type == 'c') {
                    if (i == 0) {
                        Bits cw_catc_1 = codeword_catc(tuple_value[i], t_m, m_u1, m_u1_k);
                        shiftBits(cw_catc_1, 0);
                        orBits(t_sig, cw_catc_1);
                        freeBits(cw_catc_1);
                    } else {
                        Bits cw_catc_rest = codeword_catc(tuple_value[i], t_m, m_ur, m_ur_k);
                        shiftBits(cw_catc_rest, ((i - 1) * m_ur + m_u1));
                        orBits(t_sig, cw_catc_rest);
                        freeBits(cw_catc_rest);
                    }
                }
            }
        }
        i++;
    }
    freeVals(tuple_value, n);
    return t_sig;
}


// find "matching" pages using tuple signatures

void findPagesUsingTupSigs(Query q) {
    assert(q != NULL);

    ///TODO
    unsetAllBits(q->pages); // remove this
    // get relation
    Reln r = q->rel;
    Bits q_page_sig = makeTupleSig(r, q->qstring);
    Count npages = nTsigPages(r);  // (REL)->params.tsigNpages // number of tsig pages
    File t_file = tsigFile(r);
    Count tm = tsigBits(r);  // m;  width of tuple signature (#bits)
    Count max_t_sig = maxTsigsPP(r);  // max tuple signatures per page // (REL)->params.tsigPP
    Count max_t_size_page = maxTupsPP(r);  // tupPP;   // max tuples per page
    int page_id = 0;
    while (page_id < npages) {
        Page curr_page = getPage(t_file, page_id);
        for (int tuple_id = 0; tuple_id < pageNitems(curr_page); tuple_id++) {
            // make a new tuple signature
            Bits t_sig = newBits(tm);
            getBits(curr_page, tuple_id, t_sig);
            // to get the corresponding data
            if (isSubset(q_page_sig, t_sig)) {
                // calculate the result
                // record the curr number of signature
                Offset curr_tid = page_id * max_t_sig + tuple_id;
                PageID position = curr_tid / max_t_size_page;
                setBit(q->pages, position);
            }
            freeBits(t_sig);
            // every signature
            q->nsigs++;
        }
        q->nsigpages++;
        page_id++;
        free(curr_page);
    }

    // The printf below is primarily for debugging
//    Remove it before submitting this function
//    printf("Matched Pages:");
//    showBits(q->pages);
//    putchar('\n');

}




