// reln.c ... functions on Relations
// part of signature indexed files
// Written by John Shepherd, March 2019

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "defs.h"
#include "reln.h"
#include "page.h"
#include "tuple.h"
#include "tsig.h"
#include "bits.h"
#include "hash.h"
#include "psig.h"

///  COMP9315_21T1_ass02  Haojin Guo z5216214, update

// open a file with a specified suffix
// - always open for both reading and writing


File openFile(char *name, char *suffix) {
    char fname[MAXFILENAME];
    sprintf(fname, "%s.%s", name, suffix);
    File f = open(fname, O_RDWR | O_CREAT, 0644);
    assert(f >= 0);
    return f;
}

// create a new relation (five files)
// data file has one empty data page

Status newRelation(char *name, Count nattrs, float pF, char sigtype,
                   Count tk, Count tm, Count pm, Count bm) {

    Reln r = malloc(sizeof(RelnRep));
    RelnParams *p = &(r->params);

    assert(r != NULL);
    p->nattrs = nattrs;
    p->pF = pF,
            p->sigtype = sigtype;
    p->tupsize = 28 + 7 * (nattrs - 2);
    Count available = (PAGESIZE - sizeof(Count));
    p->tupPP = available / p->tupsize;
    p->tk = tk;
    if (tm % 8 > 0) tm += 8 - (tm % 8); // round up to byte size
    p->tm = tm;
    p->tsigSize = tm / 8;
    p->tsigPP = available / (tm / 8);
    if (pm % 8 > 0) pm += 8 - (pm % 8); // round up to byte size
    p->pm = pm;
    p->psigSize = pm / 8;
    p->psigPP = available / (pm / 8);
    if (p->psigPP < 2) {
        free(r);
        return -1;
    }
    if (bm % 8 > 0) bm += 8 - (bm % 8); // round up to byte size
    p->bm = bm;
    p->bsigSize = bm / 8;
    p->bsigPP = available / (bm / 8);
    if (p->bsigPP < 2) {
        free(r);
        return -1;
    }
    r->infof = openFile(name, "info");
    r->dataf = openFile(name, "data");
    r->tsigf = openFile(name, "tsig");
    r->psigf = openFile(name, "psig");
    r->bsigf = openFile(name, "bsig");
    addPage(r->dataf);
    p->npages = 1;
    p->ntups = 0;

    addPage(r->tsigf);
    p->tsigNpages = 1;
    p->ntsigs = 0;

    addPage(r->psigf);
    p->psigNpages = 1;
    p->npsigs = 0;


    addPage(r->bsigf);
    p->bsigNpages = 1;
    p->nbsigs = 0; // replace this


    ///Create a file containing "pm" all-zeroes bit-strings,
    /// each of which has length "bm" bits
    ///TODO
    Count pm_ = psigBits(r);  // get pm:  // width of page signature (#bits)
    Count bm_ = bsigBits(r);  // width of bit-slice (=maxpages)
    Count bc = maxBsigsPP(r); //Maximum capacity per page for tuples
    File bit_files = bsigFile(r);

    for (int index = 0; index < pm_; index++) {
        PageID pid = p->bsigNpages - 1;
        Page curBitPage = getPage(bit_files, pid);
        // if it full
        if (pageNitems(curBitPage) == bc) {
            addPage(bit_files);
            p->bsigNpages++;  // / number of bsig pages
            pid++;
            free(curBitPage);
            curBitPage = newPage();
            if (curBitPage == NULL) {
                return NO_PAGE;
            }
        }
        // Create new item
        Bits bit_sig = newBits(bm_);
        putBits(curBitPage, pageNitems(curBitPage), bit_sig);
        addOneItem(curBitPage);
        p->nbsigs++;  // number of bit-sliced sigs (bsigs)
        // write into the file
        putPage(bit_files, pid, curBitPage);
        free(bit_sig);
    }
    closeRelation(r);
    return 0;
}


// check whether a relation already exists

Bool existsRelation(char *name) {
    char fname[MAXFILENAME];
    sprintf(fname, "%s.info", name);
    File f = open(fname, O_RDONLY);
    if (f < 0)
        return FALSE;
    else {
        close(f);
        return TRUE;
    }
}

// set up a relation descriptor from relation name
// open files, reads information from rel.info

Reln openRelation(char *name) {
    Reln r = malloc(sizeof(RelnRep));
    assert(r != NULL);
    r->infof = openFile(name, "info");
    r->dataf = openFile(name, "data");
    r->tsigf = openFile(name, "tsig");
    r->psigf = openFile(name, "psig");
    r->bsigf = openFile(name, "bsig");
    read(r->infof, &(r->params), sizeof(RelnParams));
    return r;
}

// release files and descriptor for an open relation
// copy latest information to .info file
// note: we don't write ChoiceVector since it doesn't change

void closeRelation(Reln r) {
    // make sure updated global data is put in info file
    lseek(r->infof, 0, SEEK_SET);
    int n = write(r->infof, &(r->params), sizeof(RelnParams));
    assert(n == sizeof(RelnParams));
    close(r->infof);
    close(r->dataf);
    close(r->tsigf);
    close(r->psigf);
    close(r->bsigf);
    free(r);
}

// insert a new tuple into a relation
// returns page where inserted
// returns NO_PAGE if insert fails completely

PageID addToRelation(Reln r, Tuple t) {
    assert(r != NULL && t != NULL && strlen(t) == tupSize(r));
    Page p;
    PageID pid;
    RelnParams *rp = &(r->params);

    // add tuple to last page
    pid = rp->npages - 1;
    p = getPage(r->dataf, pid);

//    Bool flag = FALSE;
    // check if room on last page; if not add new page
    if (pageNitems(p) == rp->tupPP) {
        addPage(r->dataf);
        rp->npages++;
        pid++;
        free(p);
        //  added a page? need check below
        p = newPage();
        if (p == NULL) return NO_PAGE;
//        flag = TRUE;
    }
    addTupleToPage(r, p, t);
    rp->ntups++;  //written to disk in closeRelation()
    putPage(r->dataf, pid, p);


    /// compute tuple signature and add to tsigf
    ///TODO
    // // number of tsig pages. get curr tuple page
    Bits tuple_sig = makeTupleSig(r, t);
    PageID tuple_sig_page_id = rp->tsigNpages - 1;  // nTsigPages(REL)  (REL)->params.tsigNpages // number of tsig pages
    File tuple_file = tsigFile(r); // r-> tsigf
    Page tuple_sig_page = getPage(tuple_file, tuple_sig_page_id);
    // Count tuple_items = pageNitems(tuple_sig_page);  // the number of tuples in tuple_sig_page,,
    Count max_tuple_sig = maxTsigsPP(r); //(REL)->params.tsigPP
    // // check if room on last page; if not add new page
    if (pageNitems(tuple_sig_page) == max_tuple_sig) {
        addPage(tuple_file);
        // number of tsig pages
        rp->tsigNpages++;
        tuple_sig_page_id++;
        free(tuple_sig_page);
        tuple_sig_page = newPage();
        if (tuple_sig_page == NULL) {
            return NO_PAGE;
        }
    }
    putBits(tuple_sig_page, pageNitems(tuple_sig_page), tuple_sig);
    // add one item
    addOneItem(tuple_sig_page);
    // number of tuple signatures (tsigs)
    rp->ntsigs++;
    // write into file
    putPage(tuple_file, tuple_sig_page_id, tuple_sig_page);
    freeBits(tuple_sig);

    /// compute page signature and add to psigf
    ///TODO
    Bits page_sig = makePageSig(r, t);
    PageID page_sig_pid = nPsigPages(r) - 1;  // nPsigPages(r)
    File page_file = psigFile(r);  //(REL)->psigf
    Page page_sig_page = getPage(page_file, page_sig_pid);
    Count num_page_sig = nPsigs(r);   //number of page signatures (psigs) (REL)->params.npsigs
    Count num_data_page = nPages(r);       // (REL)->params.npages  // number of data pages
    if (num_page_sig != num_data_page) {
        // check if the page is full
        if (pageNitems(page_sig_page) == maxPsigsPP(r)) {
            addPage(page_file);
            rp->psigNpages++;
            page_sig_pid++;
            free(page_sig_page);
            page_sig_page = newPage();
            if (page_sig_page == NULL) {
                return NO_PAGE;
            }
        }
        // create new page
        putBits(page_sig_page, pageNitems(page_sig_page), page_sig);
        addOneItem(page_sig_page);
        rp->npsigs++;      // number of page signatures (psigs)
        putPage(page_file, page_sig_pid, page_sig_page);
    } else {
        Count _pm = psigBits(r);  // // width of page signature (#bits)

        Bits P_page_sig = newBits(_pm);

        Count position = pageNitems(page_sig_page) - 1;
        getBits(page_sig_page, position, P_page_sig);
        orBits(page_sig, P_page_sig);
        // merge together
        putBits(page_sig_page, position, page_sig);
        freeBits(P_page_sig);
        putPage(page_file, page_sig_pid, page_sig_page);
    }
    freeBits(page_sig);



    ///use page signature to update bit-slices
    ///TODO

    Bits psig_b = makePageSig(r, t);
    Count bc = maxBsigsPP(r);
    Count _pm = psigBits(r);
    Count _bm = bsigBits(r);
    File bslice_file = bsigFile(r);
    PageID b_pid = nPsigs(r) - 1;

    for (int index = 0; index < _pm; index++) {

        if (!bitIsSet(psig_b, index)) {
            continue;

        } else {
            Bits temp_bslice_sig = newBits(_bm);
            int bit_page_id = index / bc;  // curr page id
            int bit_page_offset = index % bc;  // curr page offset
            Page curr_page = getPage(bslice_file, bit_page_id);
            getBits(curr_page, bit_page_offset, temp_bslice_sig);
            // write in current bit-sliced index, get the bits and then set 1;

            setBit(temp_bslice_sig, b_pid);

            // write bit_sliced signature into the page
            putBits(curr_page, bit_page_offset, temp_bslice_sig);
            // write the page into file
            putPage(bslice_file, bit_page_id, curr_page);
            free(temp_bslice_sig);
        }

    }
    freeBits(psig_b);
    return nPages(r) - 1;
}

// displays info about open Reln (for debugging)

void relationStats(Reln r) {
    RelnParams *p = &(r->params);
    printf("Global Info:\n");
    printf("Dynamic:\n");
    printf("  #items:  tuples: %d  tsigs: %d  psigs: %d  bsigs: %d\n",
           p->ntups, p->ntsigs, p->npsigs, p->nbsigs);
    printf("  #pages:  tuples: %d  tsigs: %d  psigs: %d  bsigs: %d\n",
           p->npages, p->tsigNpages, p->psigNpages, p->bsigNpages);
    printf("Static:\n");
    printf("  tups   #attrs: %d  size: %d bytes  max/page: %d\n",
           p->nattrs, p->tupsize, p->tupPP);
    printf("  sigs   %s",
           p->sigtype == 'c' ? "catc" : "simc");
    if (p->sigtype == 's')
        printf("  bits/attr: %d", p->tk);
    printf("\n");
    printf("  tsigs  size: %d bits (%d bytes)  max/page: %d\n",
           p->tm, p->tsigSize, p->tsigPP);
    printf("  psigs  size: %d bits (%d bytes)  max/page: %d\n",
           p->pm, p->psigSize, p->psigPP);
    printf("  bsigs  size: %d bits (%d bytes)  max/page: %d\n",
           p->bm, p->bsigSize, p->bsigPP);
}
