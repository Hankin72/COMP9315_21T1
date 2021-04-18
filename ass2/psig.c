// psig.c ... functions on page signatures (psig's)
// part of signature indexed files
// Written by John Shepherd, March 2019

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "psig.h"
#include "hash.h"

///  COMP9315_21T1_ass02  Haojin Guo z5216214


Bits psig_codeword_s(char *attr_value, int m, int k) {
    Bits code_word = newBits(m);
    int nbits=0;
    srandom(hash_any(attr_value, strlen(attr_value)));
    while(nbits < k ) {
        int i = random() % m;
        if (bitIsSet(code_word, i) == FALSE){
            setBit(code_word, i);
            nbits++;
        }
    }
    return code_word;
}


Bits psig_codeword_c(char *attr_value, int m, int m_catc, int k) {

    Bits code_word = newBits(m);
    int nbits=0;
    srandom(hash_any(attr_value, strlen(attr_value)));
    while(nbits < k ) {
        int i = random() % m_catc;
        if (bitIsSet(code_word, i) == FALSE){
            setBit(code_word, i);
            nbits++;
        }
    }
    return code_word;
}



Bits makePageSig(Reln r, Tuple t) {
    assert(r != NULL && t != NULL);
    ///TODO
    Count p_m = psigBits(r);  // // width of page signature (#bits)
    Count p_k = codeBits(r);  // bits set per attribute
    Count n = nAttrs(r); // number of attributes
    Count c = maxTupsPP(r);
    Bits p_sig = newBits(p_m);

    int m_c1 = p_m/n + p_m % n;
    int k_c1 = m_c1/2/c;

    int m_cr = p_m/n;
    int k_cr = m_cr/2/c;

    char sig_type = sigType(r);
    char **attribute_value = tupleVals(r, t);
    for (int index=0; index <n ;index++) {
        if (strcmp(attribute_value[index], "?\0") != 0) {
            if (sig_type == 's') {
                Bits cw_s = psig_codeword_s(attribute_value[index], p_m, p_k);
                orBits(p_sig, cw_s);
                freeBits(cw_s);
            } else {
                if (sig_type == 'c'){
//
//                    Bits cw_c1 =  psig_codeword_s(attribute_value[index], p_m,  p_k);
//                    orBits(p_sig, cw_c1);
//                    freeBits(cw_c1);

                    if (index ==0){
                        Bits cw_c1 =  psig_codeword_c(attribute_value[index], p_m, m_c1,  k_c1);
                        orBits(p_sig, cw_c1);
                        freeBits(cw_c1);
                    }else{
                        Bits cw_cr =  psig_codeword_c(attribute_value[index], p_m, m_cr,k_cr );
                        shiftBits(cw_cr, ((index - 1) * m_cr + m_c1));
                        orBits(p_sig, cw_cr);
                        freeBits(cw_cr);
                    }
                }
            }
        }
    }
    freeVals(attribute_value, n);
    return p_sig;
}


void findPagesUsingPageSigs(Query q) {
    assert(q != NULL);
    ///TODO
    unsetAllBits(q->pages); // remove this
    // get relation
    Reln r = q->rel;
    Bits q_page_sig = makePageSig(r, q->qstring);

    Count npages = nPsigPages(r); // nPsigPages(REL)  (REL)->params.psigNpages // number of psig pages
    File page_file = psigFile(r);

    Count pm = psigBits(r);   //    (REL)->params.pm, // width of page signature (#bits)
    Count max_tuple_sig_pp = maxPsigsPP(r);     // max tuple signatures per page maxPsigsPP(REL)  (REL)->params.psigPP

    int page_id =0 ;
    while(page_id < npages ){
        Page curr_page  = getPage(page_file, page_id);
        for (int p_tuple_id =0; p_tuple_id< pageNitems(curr_page); p_tuple_id++){
            Bits temp_psig = newBits(pm);
            getBits(curr_page, p_tuple_id, temp_psig);
            // to get the detail data
            if (isSubset(q_page_sig, temp_psig)){
                // record the result
                Offset curr_id = page_id * max_tuple_sig_pp + p_tuple_id;
                setBit(q->pages, curr_id);
            }
            freeBits(temp_psig);

            q->nsigs++;   // // how many signatures read
        }
        q->nsigpages++;  // // how many signature pages read
        page_id++;
        free(curr_page);
    }
}

