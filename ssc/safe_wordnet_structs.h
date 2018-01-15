/*
WordNet Release 3.0 This software and database is being provided to you, the LICENSEE, by Princeton University under the following license.
By obtaining, using and/or copying this software and database, you agree that you have read, understood, and will comply with these terms
and conditions.: Permission to use, copy, modify and distribute this software and database and its documentation for any purpose and without
fee or royalty is hereby granted, provided that you agree to comply with the following copyright notice and statements, including the
disclaimer, and that the same appear on ALL copies of the software, database and documentation, including modifications that you make
for internal use or for distribution. WordNet 3.0 Copyright 2006 by Princeton University. All rights reserved.

THIS SOFTWARE AND DATABASE IS PROVIDED "AS IS" AND PRINCETON UNIVERSITY MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
BY WAY OF EXAMPLE, BUT NOT LIMITATION, PRINCETON UNIVERSITY MAKES NO REPRESENTATIONS OR WARRANTIES OF MERCHANT- ABILITY OR FITNESS
FOR ANY PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE, DATABASE OR DOCUMENTATION WILL NOT INFRINGE ANY THIRD PARTY
PATENTS, COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS.

The name of Princeton University or Princeton may not be used in advertising or publicity pertaining
to distribution of the software and/or database. Title to copyright in this software, database and
any associated documentation shall at all times remain with Princeton University and LICENSEE
agrees to preserve same.
*/

// This is modified version of original wordnet source from https://wordnet.princeton.edu/
// Modified by Innfarn Yoo (rihwan@gmail.com)
#ifndef __SAFE_WORDNET_STRUCTS_H__
#define __SAFE_WORDNET_STRUCTS_H__

#include "safe_wordnet_defines.h"

/* Structure for index file entry */
struct Index{
    long idxoffset;		/* byte offset of entry in index file */
    char *wd;			/* word string */
    char *pos;			/* part of speech */
    int sense_cnt;		/* sense (collins) count */
    int off_cnt;		/* number of offsets */
    int tagged_cnt;		/* number senses that are tagged */
    unsigned long *offset;	/* offsets of synsets containing word */
    int ptruse_cnt;		/* number of pointers used */
    int *ptruse;		/* pointers used */
};

typedef Index *IndexPtr;

/* Structure for data file synset */
struct Synset {
    long hereiam;		/* current file position */
    int sstype;			/* type of ADJ synset */
    int fnum;			/* file number that synset comes from */
    char *pos;			/* part of speech */
    int wcount;			/* number of words in synset */
    char **words;		/* words in synset */
    int *lexid;			/* unique id in lexicographer file */
    int *wnsns;			/* sense number in wordnet */
    int whichword;		/* which word in synset we're looking for */
    int ptrcount;		/* number of pointers */
    int *ptrtyp;		/* pointer types */
    long *ptroff;		/* pointer offsets */
    int *ppos;			/* pointer part of speech */
    int *pto;			/* pointer 'to' fields */
    int *pfrm;			/* pointer 'from' fields */
    int fcount;			/* number of verb frames */
    int *frmid;			/* frame numbers */
    int *frmto;			/* frame 'to' fields */
    char *defn;			/* synset gloss (definition) */
    unsigned int key;   /* unique synset key */

    /* these fields are used if a data structure is returned
    instead of a text buffer */

    Synset *nextss;		/* ptr to next synset containing searchword */
    Synset *nextform;	/* ptr to list of synsets for alternate spelling of wordform */
    int searchtype;		    /* type of search performed */
    Synset *ptrlist;		/* ptr to synset list result of search */
    char *headword;		    /* if pos is "s", this is cluster head word */
    short headsense;		/* sense number of headword */

    // TODO move this into a inherited struct
    int depth; /* added by Xin, depth of the synset in a graph*/
};

typedef Synset *SynsetPtr;

struct SnsIndex {
    char *sensekey;		    /* sense key */
    char *word;			    /* word string */
    long loc;			    /* synset offset */
    int wnsense;		    /* WordNet sense number */
    int tag_cnt;		    /* number of semantic tags to sense */
    struct si *nextsi;		/* ptr to next sense index entry */
};

typedef SnsIndex *SnsIndexPtr;

struct SearchResults {
    int SenseCount[MAX_FORMS];	/* number of senses word form has */
    int OutSenseCount[MAX_FORMS]; /* number of senses printed for word form */
    int numforms;		/* number of word forms searchword has */
    int printcnt;		/* number of senses printed by search */
    char *searchbuf;		/* buffer containing formatted results */
    SynsetPtr searchds;		/* data structure containing search results */
};

typedef SearchResults *SearchResultsPtr;

struct Preposition {
    const char *str;
    int strlen;
};

#endif//__SAFE_WORDNET_STRUCTS_H__