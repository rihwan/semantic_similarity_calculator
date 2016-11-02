#ifndef __SAFE_WORDNET_DEFINES_H__
#define __SAFE_WORDNET_DEFINES_H__

#ifndef UNREF_PARAM
#define UNREF_PARAM(p) (p)
#endif

#define EXCFILE	"%s.exc"

#define NUMPREPS 15

#define DATAFILE        "data.%s"
#define INDEXFILE       "index.%s"
#define SENSEIDXFILE    "index.sense"
//#define KEYIDXFILE      "index.key"
#define REVKEYIDXFILE   "index.key.rev"
#define VRBSENTFILE     "sents.vrb"
#define VRBIDXFILE      "sentidx.vrb"
#define CNTLISTFILE     "cntlist.rev"

/* Various buffer sizes */

#define SEARCHBUF   ((long)(200*(long)1024))
#define LINEBUF     (15*1024) /* 15K buffer to read index & data files */
#define SMLINEBUF   (3*1024) /* small buffer for output lines */
#define WORDBUF     (256)	/* buffer for one word or collocation */

#define ALLSENSES   0	/* pass to findtheinfo() if want all senses */
#define MAXID       15	/* maximum id number in lexicographer file */
#define MAXDEPTH    20	/* maximum tree depth - used to find cycles */
#define MAXSENSE    75	/* maximum number of senses in database */
#define MAX_FORMS   5	/* max # of different 'forms' word can have */
#define MAXFNUM     44	/* maximum number of lexicographer files */

/* Pointer type and search type counts */

/* Pointers */

#define ANTPTR           1	/* ! */
#define HYPERPTR         2	/* @ */
#define HYPOPTR          3	/* ~ */
#define ENTAILPTR        4	/* * */
#define SIMPTR           5	/* & */

#define ISMEMBERPTR      6	/* #m */
#define ISSTUFFPTR       7	/* #s */
#define ISPARTPTR        8	/* #p */

#define HASMEMBERPTR     9	/* %m */
#define HASSTUFFPTR     10	/* %s */
#define HASPARTPTR      11	/* %p */

#define MERONYM         12	/* % (not valid in lexicographer file) */
#define HOLONYM         13	/* # (not valid in lexicographer file) */
#define CAUSETO         14	/* > */
#define PPLPTR	        15	/* < */
#define SEEALSOPTR	    16	/* ^ */
#define PERTPTR		    17	/* \ */
#define ATTRIBUTE	    18	/* = */
#define VERBGROUP	    19	/* $ */
#define DERIVATION      20	/* + */
#define CLASSIFICATION  21	/* ; */
#define CLASS           22	/* - */

#define LASTTYPE	CLASS

/* Misc searches */

#define SYNS            (LASTTYPE + 1)
#define FREQ            (LASTTYPE + 2)
#define FRAMES          (LASTTYPE + 3)
#define COORDS          (LASTTYPE + 4)
#define RELATIVES       (LASTTYPE + 5)
#define HMERONYM        (LASTTYPE + 6)
#define HHOLONYM        (LASTTYPE + 7)
#define WNGREP          (LASTTYPE + 8)
#define OVERVIEW        (LASTTYPE + 9)

#define MAXSEARCH       OVERVIEW

#define CLASSIF_START    (MAXSEARCH + 1)

#define CLASSIF_CATEGORY (CLASSIF_START)        /* ;c */
#define CLASSIF_USAGE    (CLASSIF_START + 1)    /* ;u */
#define CLASSIF_REGIONAL (CLASSIF_START + 2)    /* ;r */

#define CLASSIF_END      CLASSIF_REGIONAL

#define CLASS_START      (CLASSIF_END + 1)

#define CLASS_CATEGORY   (CLASS_START)          /* -c */
#define CLASS_USAGE      (CLASS_START + 1)      /* -u */
#define CLASS_REGIONAL   (CLASS_START + 2)      /* -r */

#define CLASS_END        CLASS_REGIONAL

#define INSTANCE         (CLASS_END + 1)        /* @i */
#define INSTANCES        (CLASS_END + 2)        /* ~i */

#define MAXPTR          INSTANCES

/* WordNet part of speech stuff */

#define NUMPARTS	4	/* number of parts of speech */
#define NUMFRAMES	35	/* number of verb frames */

/* Generic names for part of speech */

#define NOUN        1
#define VERB        2
#define ADJ         3
#define ADV         4
#define SATELLITE   5   /* not really a part of speech */
#define ADJSAT      SATELLITE

#define ALL_POS     0   /* passed to in_wn() to check all POS */

#define bit(n) ((unsigned int)((unsigned int)1<<((unsigned int)n)))

/* Adjective markers */

#define PADJ        1   /* (p) */
#define NPADJ       2   /* (a) */
#define IPADJ       3   /* (ip) */

#define UNKNOWN_MARKER      0
#define ATTRIBUTIVE         NPADJ
#define PREDICATIVE         PADJ
#define IMMED_POSTNOMINAL   IPADJ

/* For adjectives, indicates synset type */
#define DONT_KNOW       0
#define DIRECT_ANT      1   /* direct antonyms (cluster head) */
#define INDIRECT_ANT    2   /* indrect antonyms (similar) */
#define PERTAINYM       3   /* no antonyms or similars (pertainyms) */

/* Flags for printsynset() */
#define ALLWORDS        0   /* print all words */
#define SKIP_ANTS       0   /* skip printing antonyms in printsynset() */
#define PRINT_ANTS      1   /* print antonyms in printsynset() */
#define SKIP_MARKER     0   /* skip printing adjective marker */
#define PRINT_MARKER    1   /* print adjective marker */

/* Trace types used by printspaces() to determine print sytle */
#define TRACEP      1   /* traceptrs */
#define TRACEC      2   /* tracecoords() */
#define TRACEI      3   /* traceinherit() */

#define DEFON   1
#define DEFOFF  0

#endif//__SAFE_WORDNET_DEFINES_H__