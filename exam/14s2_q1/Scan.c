// Scan.c ... scanning operations

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "Globals.h"
#include "Scan.h"
#include "Page.h"

// startScan: start a scan on an open file
// - returns a pointer to a newly created Scan object
// - if Scan object can't be created, return NULL
// - if Scan object is created, initialise it appropriately
//   - which includes making a copy of the file descriptor
Scan *startScan(int file)
{
	// TODO
	return NULL; // replace this line
}

// nextTuple: get the tuple during a scan
// - if no more tuples, return NONE
// - if finds an invalid tuple offset, return NONE
// - otherwise advance to next tuple
//   - may have to move to next page
//   - may have to skip empty pages
//   - may eventually discover "no more tuples"
// - copy next tuple string into buf and return OK
int nextTuple(Scan *s, char *buf)
{
	// TODO
	return NONE; // replace this line
}

// closeScan: clean up after a scan finishes
void closeScan(Scan *s)
{
	if (s != NULL) free(s);
}
