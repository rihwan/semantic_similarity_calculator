/*
WordNet Release 3.0 This software and database is being provided to you, the LICENSEE, by Princeton University under the following license. By obtaining, using and/or copying this software and database, you agree that you have read, understood, and will comply with these terms and conditions.: Permission to use, copy, modify and distribute this software and database and its documentation for any purpose and without fee or royalty is hereby granted, provided that you agree to comply with the following copyright notice and statements, including the disclaimer, and that the same appear on ALL copies of the software, database and documentation, including modifications that you make for internal use or for distribution. WordNet 3.0 Copyright 2006 by Princeton University. All rights reserved. THIS SOFTWARE AND DATABASE IS PROVIDED "AS IS" AND PRINCETON UNIVERSITY MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED. BY WAY OF EXAMPLE, BUT NOT LIMITATION, PRINCETON UNIVERSITY MAKES NO REPRESENTATIONS OR WARRANTIES OF MERCHANT- ABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE, DATABASE OR DOCUMENTATION WILL NOT INFRINGE ANY THIRD PARTY PATENTS, COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS. The name of Princeton University or Princeton may not be used in advertising or publicity pertaining to distribution of the software and/or database. Title to copyright in this software, database and any associated documentation shall at all times remain with Princeton University and LICENSEE agrees to preserve same.
*/

// This is modified version of original wordnet source from https://wordnet.princeton.edu/
// Modified by Innfarn Yoo (rihwan@gmail.com)
#ifndef __SAFE_WORDNET_H__
#define __SAFE_WORDNET_H__

#include "memory_file_helper.h"
#include "safe_wordnet_structs.h"

// Thread Safe WordNet
class SafeWordNet
{
public:
    static const int KEY_LEN = 1024;
    static const int LINE_LEN = 1024 * 25;

protected:
    int OpenDB;

    int morphinit_done;
    int morphinit_openerr;

    RFILE* exc_fps[NUMPARTS + 1];
    RFILE* datafps[NUMPARTS + 1];
    RFILE* indexfps[NUMPARTS + 1];

    RFILE *sensefp;
    RFILE *cntlistfp;
    RFILE *keyindexfp;
    RFILE *revkeyindexfp;
    RFILE *vsentfilefp;
    RFILE *vidxfilefp;

    SearchResults wnresults;

    char morphstr_searchstr[WORDBUF];
    char morphstr_str[WORDBUF];
    int morphstr_svcnt;
    int morphstr_svprep;

    char morphword_retval[WORDBUF];
    char exc_lookup_line[WORDBUF];
    char morphprep_retval[WORDBUF];

    char getindex_strings[MAX_FORMS][WORDBUF];
    IndexPtr getindex_offsets[MAX_FORMS];
    int getindex_offset;

    char global_searchbuffer[SEARCHBUF];

    int global_prflag, global_sense, global_prlexid;
    int global_overflag;
    int global_lastholomero;

    char parse_synset_line[LINEBUF];

    int global_fnflag;
    int global_dflag;
    int global_saflag;
    int global_fileinfoflag;
    int global_frflag;
    int global_abortsearch;
    int global_offsetflag;
    int global_wnsnsflag;

    char printant_retbuf[SMLINEBUF];

    int wninit_done;
    int wninit_openerr;

private:
    SafeWordNet(const SafeWordNet&);
    SafeWordNet(SafeWordNet&&);
    SafeWordNet& operator=(const SafeWordNet&);
    SafeWordNet& operator=(SafeWordNet&&);

    /* General error message handler - can be defined by interface.
       Default function provided in library returns -1 */
    static int default_display_message(const char*);
    int (*display_message)(const char*);

    int morph_do_init();
    int do_init();

    int cntwords(const char *s, char separator) const;
    void deadjify(std::string& word, int& adj_marker) const;

    void strtolower(char* str) const;
    void strtolower(std::string& s) const;

    void trim_space(char* key) const;

public:
    SafeWordNet();
    ~SafeWordNet();

    int wninit();
    int re_wninit();

    int getOpenDB() const { return OpenDB; }

    int morphinit();
    int re_morphinit();

    void closefps();

    std::string read_index(long offset, RFILE *fp);
    std::string bin_search(const char* searchkey, RFILE* fp, long* last_bin_search_offset_ptr);
    const char* morphstr(const char *origstr, int pos);
    const char *morphword(const char *word, int pos);

    int strend(const char *str1, const char *str2);
    std::string wordbase(const char *word, int ender);
    int hasprep(const char *s, int wdcnt);
    const char* exc_lookup(const char *word, int pos);
    const char* morphprep(const char *s);

    unsigned int is_defined(const char* searchstr, int dbase);
    IndexPtr getindex(const char *searchstr, int dbase);

    std::string strsubst(const char* str, char from, char to);

    void WNOverview(char *searchstr, int pos);
    void printbuffer(char *str);
    IndexPtr parse_index(long offset, int dbase, const char* input_line);
    IndexPtr index_lookup(const char *word, int dbase);
    int getptrtype(const char *ptrstr);

    void free_index(IndexPtr idx);
    void free_synset(SynsetPtr synptr);
    void free_syns(SynsetPtr synptr);

    SynsetPtr read_synset(int dbase, long boffset, const char *word);
    SynsetPtr parse_synset(RFILE *fp, int dbase, const char *word);
    int getsstype(const char *s);
    int getpos(const char *s);
    int getsearchsense(SynsetPtr synptr, int whichword);
    unsigned int GetKeyForOffset(const char *loc);

    void printsynset(char *head, SynsetPtr synptr, char *tail, int definition, int wdnum, int antflag, int markerflag);
    void catword(char *buf, SynsetPtr synptr, int wdnum, int adjmarker, int antflag);
    char *printant(int dbase, SynsetPtr synptr, int wdnum, const char *templ, char *tail);
    int GetTagcnt(IndexPtr idx, int sense);
    void WNSnsToStr(IndexPtr idx, int sense, std::string& ret);

    unsigned int in_wn(const char *word, int pos);
    int HasHoloMero(IndexPtr index, int ptrtyp);
    int HasPtr(SynsetPtr synptr, int ptrtyp);

    SynsetPtr findtheinfo_ds2(const char *searchstr, int dbase, int ptrtyp, int whichsense, int depth2);
    SynsetPtr traceptrs_ds(SynsetPtr synptr, int ptrtyp, int dbase, int depth);
    int depthcheck(int depth, SynsetPtr synptr);
};

#endif//__SAFE_WORDNET_H__