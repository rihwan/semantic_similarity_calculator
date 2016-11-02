#include "stdafx.h"
#include "safe_wordnet.h"

static const char* sufx[] = {
    /* Noun suffixes */
    "s", "ses", "xes", "zes", "ches", "shes", "men", "ies",
    /* Verb suffixes */
    "s", "ies", "es", "es", "ed", "ed", "ing", "ing",
    /* Adjective suffixes */
    "er", "est", "er", "est"
};

static const char* addr[] = {
    /* Noun endings */
    "", "s", "x", "z", "ch", "sh", "man", "y",
    /* Verb endings */
    "", "y", "e", "", "e", "", "e", "",
    /* Adjective endings */
    "", "", "e", "e"
};

static const Preposition prepositions[NUMPREPS] = {
    "to", 2,
    "at", 2,
    "of", 2,
    "on", 2,
    "off", 3,
    "in", 2,
    "out", 3,
    "up", 2,
    "down", 4,
    "from", 4,
    "with", 4,
    "into", 4,
    "for", 3,
    "about", 5,
    "between", 7,
};

static const char *ptrtyp[] = {
    "",				/* 0 not used */
    "!",			/* 1 ANTPTR */
    "@",			/* 2 HYPERPTR */
    "~",			/* 3 HYPOPTR */
    "*",			/* 4 ENTAILPTR */
    "&",			/* 5 SIMPTR */
    "#m",			/* 6 ISMEMBERPTR */
    "#s",			/* 7 ISSTUFFPTR */
    "#p",			/* 8 ISPARTPTR */
    "%m",			/* 9 HASMEMBERPTR */
    "%s",			/* 10 HASSTUFFPTR */
    "%p",			/* 11 HASPARTPTR */
    "%",			/* 12 MERONYM */
    "#",			/* 13 HOLONYM */
    ">",			/* 14 CAUSETO */
    "<",			/* 15 PPLPTR */
    "^",			/* 16 SEEALSO */
    "\\",			/* 17 PERTPTR */
    "=",			/* 18 ATTRIBUTE */
    "$",			/* 19 VERBGROUP */
    "+",		        /* 20 NOMINALIZATIONS */
    ";",			/* 21 CLASSIFICATION */
    "-",			/* 22 CLASS */
    /* additional searches, but not pointers.  */
    "",				/* SYNS */
    "",				/* FREQ */
    "+",			/* FRAMES */
    "",				/* COORDS */
    "",				/* RELATIVES */
    "",				/* HMERONYM */
    "",				/* HHOLONYM */
    "",				/* WNGREP */
    "",				/* OVERVIEW */
    ";c",			/* CLASSIF_CATEGORY */
    ";u",			/* CLASSIF_USAGE */
    ";r",			/* CLASSIF_REGIONAL */
    "-c",			/* CLASS_CATEGORY */
    "-u",			/* CLASS_USAGE */
    "-r",			/* CLASS_REGIONAL */
    "@i",			/* INSTANCE */
    "~i",			/* INSTANCES */
    nullptr,
};

/* Lexicographer file names and numbers */

static const char *lexfiles[] = {
    "adj.all",			/* 0 */
    "adj.pert",			/* 1 */
    "adv.all",			/* 2 */
    "noun.Tops",		/* 3 */
    "noun.act",			/* 4 */
    "noun.animal",		/* 5 */
    "noun.artifact",		/* 6 */
    "noun.attribute",		/* 7 */
    "noun.body",		/* 8 */
    "noun.cognition",		/* 9 */
    "noun.communication",	/* 10 */
    "noun.event",		/* 11 */
    "noun.feeling",		/* 12 */
    "noun.food",		/* 13 */
    "noun.group",		/* 14 */
    "noun.location",		/* 15 */
    "noun.motive",		/* 16 */
    "noun.object",		/* 17 */
    "noun.person",		/* 18 */
    "noun.phenomenon",		/* 19 */
    "noun.plant",		/* 20 */
    "noun.possession",		/* 21 */
    "noun.process",		/* 22 */
    "noun.quantity",		/* 23 */
    "noun.relation",		/* 24 */
    "noun.shape",		/* 25 */
    "noun.state",		/* 26 */
    "noun.substance",		/* 27 */
    "noun.time",		/* 28 */
    "verb.body",		/* 29 */
    "verb.change",		/* 30 */
    "verb.cognition",		/* 31 */
    "verb.communication",	/* 32 */
    "verb.competition",		/* 33 */
    "verb.consumption",		/* 34 */
    "verb.contact",		/* 35 */
    "verb.creation",		/* 36 */
    "verb.emotion",		/* 37 */
    "verb.motion",		/* 38 */
    "verb.perception",		/* 39 */
    "verb.possession",		/* 40 */
    "verb.social",		/* 41 */
    "verb.stative",		/* 42 */
    "verb.weather",		/* 43 */
    "adj.ppl",			/* 44 */
};

static const char vs[] = " (vs. %s)";
static const char *markers[] = {
    "",			/* UNKNOWN_MARKER */
    "(predicate)",		/* PREDICATIVE */
    "(prenominal)",		/* ATTRIBUTIVE */
    "(postnominal)",	/* IMMED_POSTNOMINAL */
};

static const int offsets[NUMPARTS] = { 0, 0, 8, 16 };
static const int cnts[NUMPARTS] = { 0, 8, 8, 4 };

const char* const partnames[] = { "", "noun", "verb", "adj", "adv", nullptr };

/* add char for satellites to end */
const char partchars[] = " nvara";

const char* const adjclass[] = { "", "(p)", "(a)", "(ip)" };

SafeWordNet::SafeWordNet()
{
    OpenDB = 0; /* if non-zero, database file are open */
    morphinit_done = 0;
    morphinit_openerr = 0;

    display_message = SafeWordNet::default_display_message;

    for (int i = 0; i < NUMPARTS + 1; ++i) {
        exc_fps[i] = nullptr;
        datafps[i] = nullptr;
        indexfps[i] = nullptr;
    }

    sensefp = nullptr;
    cntlistfp = nullptr;
    keyindexfp = nullptr;
    revkeyindexfp = nullptr;
    vsentfilefp = nullptr;
    vidxfilefp = nullptr;

    memset(morphstr_searchstr, 0, sizeof(morphstr_searchstr));
    memset(morphstr_str, 0, sizeof(morphstr_str));
    morphstr_svcnt = 0;
    morphstr_svprep = 0;

    memset(morphword_retval, 0, sizeof(morphword_retval));
    memset(exc_lookup_line, 0, sizeof(exc_lookup_line));
    memset(morphprep_retval, 0, sizeof(morphprep_retval));

    memset(getindex_strings, 0, sizeof(getindex_strings));
    memset(getindex_offsets, 0, sizeof(getindex_offsets));
    getindex_offset = 0;

    memset(global_searchbuffer, 0, sizeof(global_searchbuffer));

    global_prflag = 0;
    global_sense = 0;
    global_prlexid = 0;
    global_overflag = 0;
    global_lastholomero = 0;

    memset(parse_synset_line, 0, sizeof(parse_synset_line));

    global_fnflag = 0;          /* if set, print lex filename after sense */
    global_dflag = 1;           /* if set, print definitional glosses */
    global_saflag = 1;          /* if set, print SEE ALSO pointers */
    global_fileinfoflag = 0;    /* if set, print lex file info on synsets */
    global_frflag = 0;          /* if set, print verb frames */
    global_abortsearch = 0;     /* if set, stop search algorithm */
    global_offsetflag = 0;      /* if set, print byte offset of each synset */
    global_wnsnsflag = 0;       /* if set, print WN sense # for each word */

    memset(printant_retbuf, 0, sizeof(printant_retbuf));

    wninit_done = 0;
    wninit_openerr = 0;
}

SafeWordNet::~SafeWordNet()
{
    closefps();
}

int SafeWordNet::default_display_message(const char *msg)
{
    printf(msg);
    return -1;
}

/* Open exception list files */
// Innfarn added
int SafeWordNet::morphinit()
{
    if (!morphinit_done) {
        if (OpenDB) {       /* make sure WN database files are open */
            morphinit_openerr = morph_do_init();
            if (!morphinit_openerr) {
                morphinit_done = 1;
            }
        }
        else
            morphinit_openerr = -1;
    }

    return morphinit_openerr;
}

int SafeWordNet::morph_do_init()
{
    int openerr = 0;
    char fname[256];

    for (int i = 1; i <= NUMPARTS; i++) {
        _snprintf_s(fname, 256, EXCFILE, partnames[i]);
        if ((exc_fps[i] = rfopen(fname, "r")) == nullptr) {
            char msgbuf[256];
            _snprintf_s(msgbuf, 256, "WordNet library error: Can't open exception file(%s)\n\n", fname);
            display_message(msgbuf);
            openerr = -1;
        }
    }

    return openerr;
}

int SafeWordNet::re_morphinit(void)
{
    for (int i = 1; i <= NUMPARTS; i++) {
        if (exc_fps[i]) {
            rfclose(exc_fps[i]);
            exc_fps[i] = nullptr;
        }
    }

    return (OpenDB ? morph_do_init() : -1);
}

/* Try to find baseform (lemma) of word or collocation in POS.
   Works like strtok() - first call is with string, subsequent calls
   with nullptr argument return additional baseforms for original string. */
const char* SafeWordNet::morphstr(const char *origstr, int pos)
{
    char word[WORDBUF];
    int cnt, st_idx = 0, end_idx;
    int prep;
    char *end_idx1, *end_idx2;
    char *append;

    if (pos == SATELLITE)
        pos = ADJ;

    /* First time through for this string */
    if (origstr != nullptr) {
        /* Assume string hasn't had spaces substitued with '_' */
        std::string s = strsubst(origstr, ' ', '_');
        strtolower(s);
        strcpy(morphstr_str, s.c_str());

        morphstr_searchstr[0] = '\0';
        cnt = cntwords(morphstr_str, '_');
        morphstr_svprep = 0;

        /* first try exception list */
        const char* tmp = exc_lookup(morphstr_str, pos);
        if (tmp && strcmp(tmp, morphstr_str)) {
            morphstr_svcnt = 1;      /* force next time to pass nullptr */
            return tmp;
        }

        /* Then try simply morph on original string */
        if (pos != VERB) {
            tmp = morphword(morphstr_str, pos);
            if (tmp && strcmp(tmp, morphstr_str)) {
                return tmp;
            }
        }

        if (pos == VERB && cnt > 1) {
            /* assume we have a verb followed by a preposition */
            prep = hasprep(morphstr_str, cnt);
            if (prep) {
                morphstr_svprep = prep;
                return morphprep(morphstr_str);
            }
        }

        morphstr_svcnt = cnt = cntwords(morphstr_str, '-');
        while (origstr && --cnt) {
            end_idx1 = strchr(morphstr_str + st_idx, '_');
            end_idx2 = strchr(morphstr_str + st_idx, '-');
            if (end_idx1 && end_idx2) {
                if (end_idx1 < end_idx2) {
                    end_idx = (int)(end_idx1 - morphstr_str);
                    append = "_";
                }
                else {
                    end_idx = (int)(end_idx2 - morphstr_str);
                    append = "-";
                }
            }
            else {
                if (end_idx1) {
                    end_idx = (int)(end_idx1 - morphstr_str);
                    append = "_";
                }
                else {
                    end_idx = (int)(end_idx2 - morphstr_str);
                    append = "-";
                }
            }

            if (end_idx < 0)
                return nullptr;      /* shouldn't do this */

            strncpy(word, morphstr_str + st_idx, end_idx - st_idx);
            word[end_idx - st_idx] = '\0';

            tmp = morphword(word, pos);
            if (tmp)
                strcat(morphstr_searchstr, tmp);
            else
                strcat(morphstr_searchstr, word);

            strcat(morphstr_searchstr, append);
            st_idx = end_idx + 1;
        }

        tmp = morphword(strcpy(word, morphstr_str + st_idx), pos);
        if (tmp)
            strcat(morphstr_searchstr, tmp);
        else
            strcat(morphstr_searchstr, word);

        if (strcmp(morphstr_searchstr, morphstr_str) && is_defined(morphstr_searchstr, pos))
            return morphstr_searchstr;
        else
            return nullptr;
    }
    else {
        /* subsequent call on string */

        if (morphstr_svprep) {
            /* if verb has preposition, no more morphs */
            morphstr_svprep = 0;
            return nullptr;
        }
        else if (morphstr_svcnt == 1)
            return exc_lookup(nullptr, pos);
        else {
            morphstr_svcnt = 1;
            const char* tmp = exc_lookup(morphstr_str, pos);
            if (tmp && strcmp(tmp, morphstr_str))
                return tmp;
            else
                return nullptr;
        }
    }
}

/* General purpose binary search function to search for key as first
   item on line in open file.  Item is delimited by space. */
std::string SafeWordNet::read_index(long offset, RFILE *fp)
{
    rfseek(fp, offset, SEEK_SET);

    const char* s = nullptr;
    const char* e = nullptr;
    rfgets_start_end(fp, s, e);

    assert(s);
    assert(e);

    const int size = static_cast<int>(e - s);

    std::string line = "";
    while (s != e) {
        line.push_back(*s++);
    }
    assert(static_cast<int>(line.size()) == size);

    return line;
}

void SafeWordNet::trim_space(char* key) const
{
    int len = static_cast<int>(strlen(key));
    int count = 0;
    for (int i = 0; i < len; ++i) {
        if (key[i] == ' ' || key[i] == '\t' || key[i] == '\r' || key[i] == '\n')
            count += 1;
        else
            break;
    }

    if (count > 0 && len < 256) {
        char temp[256];
        strcpy(temp, key + count);
        strcpy(key, temp);
    }

    len = static_cast<int>(strlen(key));
    for (int i = len - 1; i >= 0; --i) {
        if (key[i] == ' ' || key[i] == '\t' || key[i] == '\r' || key[i] == '\n')
            key[i] = '\0';
        else
            break;
    }
}

std::string SafeWordNet::bin_search(const char *searchkey, RFILE *fp, long* last_bin_search_offset_ptr)
{
    char key[KEY_LEN];

    std::unique_ptr<char> line_buffer(new char[LINE_LEN]);
    char* linep = line_buffer.get();
    linep[0] = '\0';

    long top = 0;
    long bot = get_file_size(fp);
    long mid = (bot - top) / 2;
    long diff = 1;

    int comparison_result;
    do {
        rfseek(fp, mid - 1, SEEK_SET);

        if (mid != 1) {
            goto_next_line(fp);
        }

        if (last_bin_search_offset_ptr) {
            (*last_bin_search_offset_ptr) = rftell(fp);
        }

        rfgets(linep, LINE_LEN, fp);
        const int length = (int)(strchr(linep, ' ') - linep);
        strncpy(key, linep, length);
        key[length] = '\0';
        trim_space(key);

        comparison_result = strcmp(key, searchkey);
        if (comparison_result < 0) {
            top = mid;
            diff = (bot - top) / 2;
            mid = top + diff;
        }
        else if (comparison_result > 0) {
            bot = mid;
            diff = (bot - top) / 2;
            mid = top + diff;
        }
        else {
            break;
        }

    } while (comparison_result != 0 && diff != 0);

    //printf("line_buffer: %s\n", line_buffer.get());

    if (!comparison_result) {
        return std::string(line_buffer.get());
    }
    else {
        return std::string();
    }
}

/* Try to find baseform (lemma) of individual word in POS */
const char* SafeWordNet::morphword(const char *word, int pos)
{
    int offset, cnt;
    int i;
    char tmpbuf[WORDBUF], *end;

    morphword_retval[0] = '\0';
    tmpbuf[0] = '\0';
    end = "";

    if (word == nullptr)
        return nullptr;

    /* first look for word on exception list */
    const char* tmp = exc_lookup(word, pos);
    if (tmp != nullptr)
        return tmp;        /* found it in exception list */

    if (pos == ADV) {       /* only use exception list for adverbs */
        return nullptr;
    }

    if (pos == NOUN) {
        if (strend(word, "ful")) {
            cnt = (int)(strrchr(word, 'f') - word);
            strncat(tmpbuf, word, cnt);
            end = "ful";
        }
        else {
            /* check for noun ending with 'ss' or short words */
            if (strend(word, "ss") || (strlen(word) <= 2)) {
                return nullptr;
            }
        }
    }

    /* If not in exception list, try applying rules from tables */

    if (tmpbuf[0] == '\0') {
        strcpy(tmpbuf, word);
    }

    offset = offsets[pos];
    cnt = cnts[pos];

    for (i = 0; i < cnt; i++) {
        std::string base = wordbase(tmpbuf, (i + offset));
        assert(sizeof(morphword_retval) > base.size());

        strcpy(morphword_retval, base.c_str());
        if (strcmp(morphword_retval, tmpbuf) && is_defined(morphword_retval, pos)) {
            strcat(morphword_retval, end);
            return morphword_retval;
        }
    }

    return nullptr;
}

int SafeWordNet::strend(const char* str1, const char* str2)
{
    if (strlen(str2) >= strlen(str1)) {
        return 0;
    } else {
        const char* pt1 = str1;
        pt1 = strchr(str1, 0);
        pt1 = pt1 - strlen(str2);
        return (!strcmp(pt1, str2));
    }
}

std::string SafeWordNet::wordbase(const char *word, int ender)
{
    char copy[WORDBUF] = { 0, };
    strcpy(copy, word);

    if (strend(copy, sufx[ender])) {
        char* pt1 = strchr(copy, '\0');
        pt1 -= strlen(sufx[ender]);
        *pt1 = '\0';
        strcat(copy, addr[ender]);
    }

    return std::string(copy);
}

int SafeWordNet::hasprep(const char *s, int wdcnt)
{
    /* Find a preposition in the verb string and return its corresponding word number. */
    for (int wdnum = 2; wdnum <= wdcnt; wdnum++) {
        s = strchr(s, '_');

        s++;

        for (int i = 0; i < NUMPREPS; i++) {
            if (!strncmp(s, prepositions[i].str, prepositions[i].strlen) &&
                (s[prepositions[i].strlen] == '_' || s[prepositions[i].strlen] == '\0')) {
                return wdnum;
            }
        }
    }

    return 0;
}

const char* SafeWordNet::exc_lookup(const char *word, int pos)
{
    char *beglp = nullptr;
    char *endlp = nullptr;

    if (exc_fps[pos] == nullptr)
        return nullptr;

    std::string excline;

    /* first time through load line from exception file */
    if (word != nullptr) {
        excline = bin_search(word, exc_fps[pos], nullptr);

        if (!excline.empty()) {
            strcpy(exc_lookup_line, excline.c_str());
            endlp = strchr(exc_lookup_line, ' ');
        } else {
            endlp = nullptr;
        }
    }

    if (endlp && *(endlp + 1) != ' ') {
        beglp = endlp + 1;

        while (*beglp && *beglp == ' ') {
            beglp++;
        }

        endlp = beglp;
        while (*endlp && *endlp != ' ' && *endlp != '\n') {
            endlp++;
        }

        if (endlp != beglp) {
            *endlp = '\0';
            return beglp;
        }
    }

    return nullptr;
}

const char* SafeWordNet::morphprep(const char *s)
{
    const char *lastwd = nullptr;
    int i, offset, cnt;
    char word[WORDBUF], end[WORDBUF];

    /* Assume that the verb is the first word in the phrase.  Strip it
    off, check for validity, then try various morphs with the
    rest of the phrase tacked on, trying to find a match. */
    const char* rest = strchr(s, '_');
    const char* last = strrchr(s, '_');
    if (rest != last) {     /* more than 2 words */
        lastwd = morphword(last + 1, NOUN);
        if (lastwd) {
            strncpy(end, rest, last - rest + 1);
            end[last - rest + 1] = '\0';
            strcat(end, lastwd);
        }
    }

    strncpy(word, s, rest - s);
    word[rest - s] = '\0';
    for (i = 0, cnt = (int)strlen(word); i < cnt; i++) {
        if (!isalnum(word[i])) {
            return nullptr;
        }
    }

    offset = offsets[VERB];
    cnt = cnts[VERB];

    /* First try to find the verb in the exception list */
    const char* exc_word = exc_lookup(word, VERB);
    if (exc_word && strcmp(exc_word, word)) {
        _snprintf_s(morphprep_retval, WORDBUF, "%s%s", exc_word, rest);
        if (is_defined(morphprep_retval, VERB)) {
            return morphprep_retval;
        }
        else if (lastwd) {
            _snprintf_s(morphprep_retval, WORDBUF, "%s%s", exc_word, end);
            if (is_defined(morphprep_retval, VERB))
                return morphprep_retval;
        }
    }

    for (i = 0; i < cnt; i++) {
        std::string w_base = wordbase(word, (i + offset));
        exc_word = w_base.c_str();

        if (exc_word && strcmp(word, exc_word)) { /* ending is different */
            _snprintf_s(morphprep_retval, WORDBUF, "%s%s", exc_word, rest);
            if (is_defined(morphprep_retval, VERB)) {
                return morphprep_retval;
            }
            else if (lastwd) {
                _snprintf_s(morphprep_retval, WORDBUF, "%s%s", exc_word, end);
                if (is_defined(morphprep_retval, VERB)) {
                    return morphprep_retval;
                }
            }
        }
    }

    _snprintf_s(morphprep_retval, WORDBUF, "%s%s", word, rest);
    if (strcmp(s, morphprep_retval)) {
        return morphprep_retval;
    }

    if (lastwd) {
        _snprintf_s(morphprep_retval, WORDBUF, "%s%s", word, end);
        if (strcmp(s, morphprep_retval)) {
            return morphprep_retval;
        }
    }

    return nullptr;
}

unsigned int SafeWordNet::is_defined(const char *searchstr, int dbase)
{
    IndexPtr index;
    int i;
    unsigned long retval = 0;

    wnresults.numforms = wnresults.printcnt = 0;
    wnresults.searchbuf = nullptr;
    wnresults.searchds = nullptr;

    while ((index = getindex(searchstr, dbase)) != nullptr) {
        searchstr = nullptr;	/* clear out for next getindex() call */

        wnresults.SenseCount[wnresults.numforms] = index->off_cnt;

        /* set bits that must be true for all words */

        retval |= bit(SIMPTR) | bit(FREQ) | bit(SYNS) | bit(WNGREP) | bit(OVERVIEW);

        /* go through list of pointer characters and set appropriate bits */

        for (i = 0; i < index->ptruse_cnt; i++) {

            if (index->ptruse[i] <= LASTTYPE) {
                retval |= bit(index->ptruse[i]);
            }
            else if (index->ptruse[i] == INSTANCE) {
                retval |= bit(HYPERPTR);
            }
            else if (index->ptruse[i] == INSTANCES) {
                retval |= bit(HYPOPTR);
            }

            if (index->ptruse[i] == SIMPTR) {
                retval |= bit(ANTPTR);
            }
#ifdef FOOP

            if (index->ptruse[i] >= CLASSIF_START &&
                index->ptruse[i] <= CLASSIF_END) {
                retval |= bit(CLASSIFICATION);
            }


            if (index->ptruse[i] >= CLASS_START &&
                index->ptruse[i] <= CLASS_END) {
                retval |= bit(CLASS);
            }
#endif

            if (index->ptruse[i] >= ISMEMBERPTR &&
                index->ptruse[i] <= ISPARTPTR)
                retval |= bit(HOLONYM);
            else if (index->ptruse[i] >= HASMEMBERPTR &&
                index->ptruse[i] <= HASPARTPTR)
                retval |= bit(MERONYM);

        }

        if (dbase == NOUN) {

            /* check for inherited holonyms and meronyms */

            if (HasHoloMero(index, HMERONYM))
                retval |= bit(HMERONYM);
            if (HasHoloMero(index, HHOLONYM))
                retval |= bit(HHOLONYM);

            /* if synset has hypernyms, enable coordinate search */

            if (retval & bit(HYPERPTR))
                retval |= bit(COORDS);
        }
        else if (dbase == VERB) {

            /* if synset has hypernyms, enable coordinate search */
            if (retval & bit(HYPERPTR))
                retval |= bit(COORDS);

            /* enable grouping of related synsets and verb frames */

            retval |= bit(RELATIVES) | bit(FRAMES);
        }

        free_index(index);
        wnresults.numforms++;
    }
    return retval;
}

/* 'smart' search of index file.  Find word in index file, trying different
   techniques - replace hyphens with underscores, replace underscores with
   hyphens, strip hyphens and underscores, strip periods. */
IndexPtr SafeWordNet::getindex(const char *searchstr, int dbase)
{
    //printf("getindex: %s, %d\n", searchstr, dbase);

    int i, j, k;
    char c;

    /* This works like strrok(): if passed with a non-null string,
       prepare vector of search strings and offsets.  If string
       is null, look at current list of offsets and return next
       one, or nullptr if no more alternatives for this word. */
    if (searchstr != nullptr) {
        std::string search_string = searchstr;

        getindex_offset = 0;
        strtolower(search_string);

        for (i = 0; i < MAX_FORMS; i++) {
            strcpy(getindex_strings[i], search_string.c_str());
            getindex_offsets[i] = 0;
        }

        std::string s = strsubst(getindex_strings[1], '_', '-');
        strcpy(getindex_strings[1], s.c_str());

        s = strsubst(getindex_strings[2], '-', '_');
        strcpy(getindex_strings[2], s.c_str());

        /* remove all spaces and hyphens from last search string, then all periods */
        for (i = j = k = 0; (c = search_string[i]) != '\0'; i++) {
            if (c != '_' && c != '-')
                getindex_strings[3][j++] = c;
            if (c != '.')
                getindex_strings[4][k++] = c;
        }
        getindex_strings[3][j] = '\0';
        getindex_strings[4][k] = '\0';

        /* Get offset of first entry.  Then eliminate duplicates and get offsets of unique strings. */
        if (getindex_strings[0][0] != 0)
            getindex_offsets[0] = index_lookup(getindex_strings[0], dbase);

        for (i = 1; i < MAX_FORMS; i++)
            if ((getindex_strings[i][0]) != 0 && (strcmp(getindex_strings[0], getindex_strings[i])))
                getindex_offsets[i] = index_lookup(getindex_strings[i], dbase);
    }

    for (i = getindex_offset; i < MAX_FORMS; i++)
        if (getindex_offsets[i]) {
            getindex_offset = i + 1;
            return getindex_offsets[i];
        }

    return 0;
}

/* Convert string to lower case remove trailing adjective marker if found */
void SafeWordNet::strtolower(char* str) const
{
    char* s = str;
    while (*s != '\0') {
        if (*s >= 'A' && *s <= 'Z') {
            int ch = tolower(*str);
            *s = static_cast<char>(ch);
        }
        else if (*s == '(') {
            break;
        }
        s++;
    }
}

/* Replace all occurences of 'from' with 'to' in 'str' */
std::string SafeWordNet::strsubst(const char* str, char from, char to)
{
    std::string s = str;

    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == from)
            s[i] = to;
    }

    return s;
}

void SafeWordNet::strtolower(std::string& s) const
{
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            int ch = tolower(s[i]);
            s[i] = static_cast<char>(ch);
        }
        else if (s[i] == '(') {
            break;
        }
    }
}

void SafeWordNet::WNOverview(char* searchstr, int pos)
{
    SynsetPtr cursyn;
    IndexPtr idx = nullptr;
    char *cpstring = searchstr, *bufstart;
    int sense, i, offsetcnt;
    int svdflag, skipit;
    unsigned long offsets[MAXSENSE];

    cpstring = searchstr;
    bufstart = global_searchbuffer;
    for (i = 0; i < MAXSENSE; i++)
        offsets[i] = 0;
    offsetcnt = 0;

    while ((idx = getindex(cpstring, pos)) != nullptr) {

        cpstring = nullptr;	/* clear for next call to getindex() */
        wnresults.SenseCount[wnresults.numforms++] = idx->off_cnt;
        wnresults.OutSenseCount[wnresults.numforms] = 0;

        printbuffer("                                                                                                   \n");

        /* Print synset for each sense.  If requested, precede
        synset with synset offset and/or lexical file information.*/
        for (sense = 0; sense < idx->off_cnt; sense++) {

            for (i = 0, skipit = 0; i < offsetcnt && !skipit; i++)
                if (offsets[i] == idx->offset[sense])
                    skipit = 1;

            if (!skipit) {
                offsets[offsetcnt++] = idx->offset[sense];
                cursyn = read_synset(pos, idx->offset[sense], idx->wd);

                char tmp_buf[256];
                if (idx->tagged_cnt != -1 && ((sense + 1) <= idx->tagged_cnt)) {
                    _snprintf_s(tmp_buf, 256, "%d. (%d) ", sense + 1, GetTagcnt(idx, sense + 1));
                }
                else {
                    _snprintf_s(tmp_buf, 256, "%d. ", sense + 1);
                }

                svdflag = global_dflag;
                global_dflag = 1;
                printsynset(tmp_buf, cursyn, "\n", DEFON, ALLWORDS, SKIP_ANTS, SKIP_MARKER);
                global_dflag = svdflag;

                wnresults.OutSenseCount[wnresults.numforms]++;
                wnresults.printcnt++;

                free_synset(cursyn);
            }
        }

        /* Print sense summary message */
        i = wnresults.OutSenseCount[wnresults.numforms];

        if (i > 0) {
            char tmp_buf[256];
            std::string s_tmpbuf;
            if (i == 1) {
                s_tmpbuf = "\nThe ";
                s_tmpbuf += partnames[pos];
                s_tmpbuf += " ";
                s_tmpbuf += idx->wd;
                s_tmpbuf += " has 1 sense";
            }
            else {
                s_tmpbuf = "\nThe ";
                s_tmpbuf += partnames[pos];
                s_tmpbuf += " ";
                s_tmpbuf += idx->wd;
                _snprintf_s(tmp_buf, 256, "%d", i);
                s_tmpbuf += tmp_buf;
                s_tmpbuf += " senses";
            }

            if (idx->tagged_cnt > 0) {
                _snprintf_s(tmp_buf, 256, " (first %d from tagged texts)\n", idx->tagged_cnt);
                s_tmpbuf += tmp_buf;
            }
            else if (idx->tagged_cnt == 0) {
                _snprintf_s(tmp_buf, 256, " (no senses from tagged texts)\n");
                s_tmpbuf += tmp_buf;
            }

            strncpy(bufstart, s_tmpbuf.c_str(), s_tmpbuf.size());
            bufstart = global_searchbuffer + strlen(global_searchbuffer);
        }
        else
            bufstart[0] = '\0';

        wnresults.numforms++;
        free_index(idx);
    }
}

void SafeWordNet::printbuffer(char* str)
{
    if (global_overflag)
        return;
    if (strlen(global_searchbuffer) + strlen(str) >= SEARCHBUF)
        global_overflag = 1;
    else
        strcat(global_searchbuffer, str);
}

/* This function parses an entry from an index file into an Index data
* structure. It takes the byte offset and file number, and optionally the
* line. If the line is nullptr, parse_index will get the line from the file.
* If the line is non-nullptr, parse_index won't look at the file, but it still
* needs the dbase and offset parameters to be set, so it can store them in
* the Index struct.
*/
IndexPtr SafeWordNet::parse_index(long offset, int dbase, const char *input_line)
{
    IndexPtr idx = nullptr;
    char *ptrtok;
    int j;

    std::unique_ptr<char> line_ptr;
    char* line = nullptr;

    if (!input_line) {
        std::string s = read_index(offset, indexfps[dbase]);
        line_ptr = std::unique_ptr<char>(new char[s.size() + 1]);
        strcpy(line_ptr.get(), s.c_str());
    }
    else {
        line_ptr = std::unique_ptr<char>(new char[strlen(input_line) + 1]);
        strcpy(line_ptr.get(), input_line);
    }
    line = line_ptr.get();

    idx = (IndexPtr)malloc(sizeof(Index));
    assert(idx);

    /* set offset of entry in index file */
    idx->idxoffset = offset;

    idx->wd = '\0';
    idx->pos = '\0';
    idx->off_cnt = 0;
    idx->tagged_cnt = 0;
    idx->sense_cnt = 0;
    idx->offset = '\0';
    idx->ptruse_cnt = 0;
    idx->ptruse = '\0';

    /* get the word */
    ptrtok = strtok(line, " \n");

    idx->wd = (char*)malloc(strlen(ptrtok) + 1);
    assert(idx->wd);
    strcpy(idx->wd, ptrtok);

    /* get the part of speech */
    ptrtok = strtok(nullptr, " \n");
    idx->pos = (char*)malloc(strlen(ptrtok) + 1);
    assert(idx->pos);
    strcpy(idx->pos, ptrtok);

    /* get the collins count */
    ptrtok = strtok(nullptr, " \n");
    idx->sense_cnt = atoi(ptrtok);

    /* get the number of pointers types */
    ptrtok = strtok(nullptr, " \n");
    idx->ptruse_cnt = atoi(ptrtok);

    if (idx->ptruse_cnt) {
        idx->ptruse = (int *)malloc(idx->ptruse_cnt * (sizeof(int)));
        assert(idx->ptruse);

        /* get the pointers types */
        for (j = 0; j < idx->ptruse_cnt; j++) {
            ptrtok = strtok(nullptr, " \n");
            idx->ptruse[j] = getptrtype(ptrtok);
        }
    }

    /* get the number of offsets */
    ptrtok = strtok(nullptr, " \n");
    idx->off_cnt = atoi(ptrtok);

    /* get the number of senses that are tagged */
    ptrtok = strtok(nullptr, " \n");
    idx->tagged_cnt = atoi(ptrtok);

    /* make space for the offsets */
    idx->offset = (unsigned long *)malloc(idx->off_cnt * (sizeof(long)));
    assert(idx->offset);

    /* get the offsets */
    for (j = 0; j < idx->off_cnt; j++) {
        ptrtok = strtok(nullptr, " \n");
        idx->offset[j] = atol(ptrtok);
    }

    return idx;
}

/* Find word in index file and return parsed entry in data structure.
   Input word must be exact match of string in database. */
IndexPtr SafeWordNet::index_lookup(const char *word, int dbase)
{
    RFILE *fp = indexfps[dbase];
    if (fp == nullptr) {
        char msgbuf[256];
        _snprintf_s(msgbuf, 256, "WordNet library error: %s indexfile not open\n", partnames[dbase]);
        display_message(msgbuf);
        return nullptr;
    }

    IndexPtr idx = nullptr;

    long last_bin_search_offset = 0;
    std::string line = bin_search(word, fp, &last_bin_search_offset);
    if (!line.empty()) {
        idx = parse_index(last_bin_search_offset, dbase, line.c_str());
    }

    return idx;
}

/* Return pointer code for pointer type characer passed. */
int SafeWordNet::getptrtype(const char *ptrstr)
{
    for (int i = 1; i <= MAXPTR; i++) {
        if (!strcmp(ptrstr, ptrtyp[i]))
            return i;
    }

    return 0;
}

/* Free an index structure */
void SafeWordNet::free_index(IndexPtr idx)
{
    free(idx->wd);
    free(idx->pos);
    if (idx->ptruse)
        free(idx->ptruse);
    free(idx->offset);
    free(idx);
}

/* Free a synset */
void SafeWordNet::free_synset(SynsetPtr synptr)
{
    int i;

    free(synptr->pos);
    for (i = 0; i < synptr->wcount; i++){
        free(synptr->words[i]);
    }
    free(synptr->words);
    free(synptr->wnsns);
    free(synptr->lexid);
    if (synptr->ptrcount) {
        free(synptr->ptrtyp);
        free(synptr->ptroff);
        free(synptr->ppos);
        free(synptr->pto);
        free(synptr->pfrm);
    }
    if (synptr->fcount) {
        free(synptr->frmid);
        free(synptr->frmto);
    }
    if (synptr->defn)
        free(synptr->defn);
    if (synptr->headword)
        free(synptr->headword);
    if (synptr->ptrlist)
        free_syns(synptr->ptrlist); /* changed from free_synset() */
    free(synptr);
}

/* Free a synset linked list allocated by findtheinfo_ds() */
void SafeWordNet::free_syns(SynsetPtr synptr)
{
    SynsetPtr cursyn, nextsyn;

    if (synptr) {
        cursyn = synptr;
        while (cursyn) {
            if (cursyn->nextform)
                free_syns(cursyn->nextform);
            nextsyn = cursyn->nextss;
            free_synset(cursyn);
            cursyn = nextsyn;
        }
    }
}

/* Read synset from data file at byte offset passed and return parsed entry in data structure. */
SynsetPtr SafeWordNet::read_synset(int dbase, long boffset, const char *word)
{
    //printf("read_synset: %d, %d, %s\n", dbase, boffset, word);
    RFILE* fp = datafps[dbase];
    if (nullptr == fp) {
        char msgbuf[256];
        _snprintf_s(msgbuf, 256, "WordNet library error: %s datafile not open\n", partnames[dbase]);
        display_message(msgbuf);
        return nullptr;
    }

    /* position file to byte offset requested */
    rfseek(fp, boffset, SEEK_SET);

    return parse_synset(fp, dbase, word); /* parse synset and return */
}

/* Read synset at current byte offset in file and return parsed entry
in data structure. */
SynsetPtr SafeWordNet::parse_synset(RFILE *fp, int dbase, const char *word)
{
    char tbuf[SMLINEBUF];
    char *ptrtok;
    char *tmpptr;
    int foundpert = 0;
    char wdnum[3];
    int i;
    SynsetPtr synptr;
    long loc;           /* sanity check on file location */

    loc = rftell(fp);

    assert(nullptr != fp);
    if ((tmpptr = rfgets(parse_synset_line, LINEBUF, fp)) == nullptr)
        return nullptr;

    synptr = (SynsetPtr)malloc(sizeof(Synset));
    assert(synptr);

    synptr->hereiam = 0;
    synptr->sstype = DONT_KNOW;
    synptr->fnum = 0;
    synptr->pos = '\0';
    synptr->wcount = 0;
    synptr->words = '\0';
    synptr->whichword = 0;
    synptr->ptrcount = 0;
    synptr->ptrtyp = '\0';
    synptr->ptroff = '\0';
    synptr->ppos = '\0';
    synptr->pto = '\0';
    synptr->pfrm = '\0';
    synptr->fcount = 0;
    synptr->frmid = '\0';
    synptr->frmto = '\0';
    synptr->defn = '\0';
    synptr->key = 0;
    synptr->nextss = nullptr;
    synptr->nextform = nullptr;
    synptr->searchtype = -1;
    synptr->ptrlist = nullptr;
    synptr->headword = nullptr;
    synptr->headsense = 0;

    ptrtok = parse_synset_line;

    /* looking at offset */
    ptrtok = strtok(parse_synset_line, " \n");
    synptr->hereiam = atol(ptrtok);

    /* sanity check - make sure starting file offset matches first field */
    if (synptr->hereiam != loc) {
        char msgbuf[256];
        _snprintf_s(msgbuf, 256, "WordNet library error: no synset at location %d\n", loc);
        display_message(msgbuf);
        free(synptr);
        return nullptr;
    }

    /* looking at FNUM */
    ptrtok = strtok(nullptr, " \n");
    synptr->fnum = atoi(ptrtok);

    /* looking at POS */
    ptrtok = strtok(nullptr, " \n");
    synptr->pos = (char*)malloc(strlen(ptrtok) + 1);
    assert(synptr->pos);
    strcpy(synptr->pos, ptrtok);
    if (getsstype(synptr->pos) == SATELLITE)
        synptr->sstype = INDIRECT_ANT;

    /* looking at numwords */
    ptrtok = strtok(nullptr, " \n");
    synptr->wcount = strtol(ptrtok, nullptr, 16);

    synptr->words = (char **)malloc(synptr->wcount  * sizeof(char *));
    assert(synptr->words);
    synptr->wnsns = (int *)malloc(synptr->wcount * sizeof(int));
    assert(synptr->wnsns);
    synptr->lexid = (int *)malloc(synptr->wcount * sizeof(int));
    assert(synptr->lexid);

    for (i = 0; i < synptr->wcount; i++) {
        ptrtok = strtok(nullptr, " \n");
        synptr->words[i] = (char*)malloc(strlen(ptrtok) + 1);
        assert(synptr->words[i]);
        strcpy(synptr->words[i], ptrtok);

        //printf("parse_synset, words[%d]: %s\n", i, synptr->words[i]);

        /* is this the word we're looking for? */
        strtolower(ptrtok);
        if (word && !strcmp(word, ptrtok))
            synptr->whichword = i + 1;

        ptrtok = strtok(nullptr, " \n");
        sscanf(ptrtok, "%x", &synptr->lexid[i]);
    }

    /* get the pointer count */
    ptrtok = strtok(nullptr, " \n");
    synptr->ptrcount = atoi(ptrtok);

    if (synptr->ptrcount) {
        /* alloc storage for the pointers */
        synptr->ptrtyp = (int *)malloc(synptr->ptrcount * sizeof(int));
        assert(synptr->ptrtyp);
        synptr->ptroff = (long *)malloc(synptr->ptrcount * sizeof(long));
        assert(synptr->ptroff);
        synptr->ppos = (int *)malloc(synptr->ptrcount * sizeof(int));
        assert(synptr->ppos);
        synptr->pto = (int *)malloc(synptr->ptrcount * sizeof(int));
        assert(synptr->pto);
        synptr->pfrm = (int *)malloc(synptr->ptrcount * sizeof(int));
        assert(synptr->pfrm);

        for (i = 0; i < synptr->ptrcount; i++) {
            /* get the pointer type */
            ptrtok = strtok(nullptr, " \n");
            synptr->ptrtyp[i] = getptrtype(ptrtok);
            /* For adjectives, set the synset type if it has a direct
            antonym */
            if (dbase == ADJ &&	synptr->sstype == DONT_KNOW) {
                if (synptr->ptrtyp[i] == ANTPTR)
                    synptr->sstype = DIRECT_ANT;
                else if (synptr->ptrtyp[i] == PERTPTR)
                    foundpert = 1;
            }

            /* get the pointer offset */
            ptrtok = strtok(nullptr, " \n");
            synptr->ptroff[i] = atol(ptrtok);

            /* get the pointer part of speech */
            ptrtok = strtok(nullptr, " \n");
            synptr->ppos[i] = getpos(ptrtok);

            /* get the lexp to/from restrictions */
            ptrtok = strtok(nullptr, " \n");

            tmpptr = ptrtok;
            strncpy(wdnum, tmpptr, 2);
            wdnum[2] = '\0';
            synptr->pfrm[i] = strtol(wdnum, (char **)nullptr, 16);

            tmpptr += 2;
            strncpy(wdnum, tmpptr, 2);
            wdnum[2] = '\0';
            synptr->pto[i] = strtol(wdnum, (char **)nullptr, 16);
        }
    }

    /* If synset type is still not set, see if it's a pertainym */
    if (dbase == ADJ && synptr->sstype == DONT_KNOW && foundpert == 1)
        synptr->sstype = PERTAINYM;

    /* retireve optional information from verb synset */
    if (dbase == VERB) {
        ptrtok = strtok(nullptr, " \n");
        synptr->fcount = atoi(ptrtok);

        /* allocate frame storage */

        synptr->frmid = (int *)malloc(synptr->fcount * sizeof(int));
        assert(synptr->frmid);
        synptr->frmto = (int *)malloc(synptr->fcount * sizeof(int));
        assert(synptr->frmto);

        for (i = 0; i < synptr->fcount; i++) {
            /* skip the frame pointer (+) */
            ptrtok = strtok(nullptr, " \n");

            ptrtok = strtok(nullptr, " \n");
            synptr->frmid[i] = atoi(ptrtok);

            ptrtok = strtok(nullptr, " \n");
            synptr->frmto[i] = strtol(ptrtok, nullptr, 16);
        }
    }

    /* get the optional definition */
    ptrtok = strtok(nullptr, " \n");
    if (ptrtok) {
        ptrtok = strtok(nullptr, " \n");
        sprintf(tbuf, "");
        while (ptrtok != nullptr) {
            strcat(tbuf, ptrtok);
            ptrtok = strtok(nullptr, " \n");
            if (ptrtok)
                strcat(tbuf, " ");
        }
        assert((1 + strlen(tbuf)) < sizeof(tbuf));
        synptr->defn = (char*)malloc(strlen(tbuf) + 4);
        assert(synptr->defn);
        sprintf(synptr->defn, "(%s)", tbuf);
    }

    /* we have unique keys */
    if (keyindexfp) {
        char temp_buffer[256];
        _snprintf_s(temp_buffer, 256, "%c:%8.8d", partchars[dbase], synptr->hereiam);
        synptr->key = GetKeyForOffset(temp_buffer);
    }

    /* Can't do earlier - calls indexlookup which messes up strtok calls */
    for (i = 0; i < synptr->wcount; i++)
        synptr->wnsns[i] = getsearchsense(synptr, i + 1);

    return(synptr);
}

/* Return synset type code for string passed. */
int SafeWordNet::getsstype(const char *s)
{
    switch (*s) {
    case 'n':
        return NOUN;
    case 'a':
        return ADJ;
    case 'v':
        return VERB;
    case 's':
        return SATELLITE;
    case 'r':
        return ADV;
    default:
        {
            char msgbuf[256];
            _snprintf_s(msgbuf, 256, "WordNet library error: Unknown synset type %s\n", s);
            display_message(msgbuf);
            assert(false);
            return -1;
        }
    }
}

/* Return part of speech code for string passed */
int SafeWordNet::getpos(const char *s)
{
    switch (*s) {
    case 'n':
        return NOUN;
    case 'a':
    case 's':
        return ADJ;
    case 'v':
        return VERB;
    case 'r':
        return ADV;
    default:
        {
            char msgbuf[256];
            _snprintf_s(msgbuf, 256, "WordNet library error: unknown part of speech %s\n", s);
            display_message(msgbuf);
            assert(false);
            return -1;
        }
    }
}

int SafeWordNet::getsearchsense(SynsetPtr synptr, int whichword)
{
    IndexPtr idx;
    int i;

    std::string s = strsubst(synptr->words[whichword - 1], ' ', '_');
    strtolower(s);

    idx = index_lookup(s.c_str(), getpos(synptr->pos));

    if (idx) {
        for (i = 0; i < idx->off_cnt; i++) {
            if (static_cast<__int64>(idx->offset[i]) == static_cast<__int64>(synptr->hereiam)) {
                free_index(idx);
                return(i + 1);
            }
        }
        free_index(idx);
    }
    return 0;
}

unsigned int SafeWordNet::GetKeyForOffset(const char *loc)
{
    unsigned int key;
    char rloc[11] = "";
    char tmpbuf[256];

    /* Try to open file in case wn_init wasn't called */
    if (!revkeyindexfp) {
        strcpy(tmpbuf, REVKEYIDXFILE);
        revkeyindexfp = rfopen(tmpbuf, "r");
    }

    if (revkeyindexfp) {
        std::string line = bin_search(loc, revkeyindexfp, nullptr);
        if (!line.empty()) {
            sscanf(line.c_str(), "%s %d", rloc, &key);
            return key;
        }
    }

    return 0;
}

void SafeWordNet::printsynset(char *head, SynsetPtr synptr, char *tail, int definition, int wdnum, int antflag, int markerflag)
{
    int i, wdcnt;
    char tbuf[SMLINEBUF];

    tbuf[0] = '\0';		/* clear working buffer */

    strcat(tbuf, head);		/* print head */

    /* Precede synset with additional information as indiecated
    by flags */

    if (global_offsetflag)     /* print synset offset */
        sprintf(tbuf + strlen(tbuf), "{%8.8d} ", synptr->hereiam);
    if (global_fileinfoflag) { /* print lexicographer file information */
        sprintf(tbuf + strlen(tbuf), "<%s> ", lexfiles[synptr->fnum]);
        global_prlexid = 1;    /* print lexicographer id after word */
    }
    else
        global_prlexid = 0;

    if (wdnum)			/* print only specific word asked for */
        catword(tbuf, synptr, wdnum - 1, markerflag, antflag);
    else			/* print all words in synset */
        for (i = 0, wdcnt = synptr->wcount; i < wdcnt; i++) {
            catword(tbuf, synptr, i, markerflag, antflag);
            if (i < wdcnt - 1)
                strcat(tbuf, ", ");
        }

    if (definition && global_dflag && synptr->defn) {
        strcat(tbuf, " -- ");
        strcat(tbuf, synptr->defn);
    }

    strcat(tbuf, tail);
    printbuffer(tbuf);
}

void SafeWordNet::catword(char *buf, SynsetPtr synptr, int wdnum, int adjmarker, int antflag)
{
    

    /* Copy the word (since deadjify() changes original string),
    deadjify() the copy and append to buffer */
    int adj_marker = 0;
    std::string word_buf = synptr->words[wdnum];
    //strcpy(wdbuf, synptr->words[wdnum]);
    deadjify(word_buf, adj_marker);
    //strcat(buf, deadjify(wdbuf));
    strcat(buf, word_buf.c_str());

    /* Print additional lexicographer information and WordNet sense
    number as indicated by flags */

    if (global_prlexid && (synptr->lexid[wdnum] != 0))
        sprintf(buf + strlen(buf), "%d", synptr->lexid[wdnum]);
    if (global_wnsnsflag)
        sprintf(buf + strlen(buf), "#%d", synptr->wnsns[wdnum]);

    /* For adjectives, append adjective marker if present, and
    print antonym if flag is passed */

    if (getpos(synptr->pos) == ADJ) {
        if (adjmarker == PRINT_MARKER)
            strcat(buf, markers[adj_marker]);
        if (antflag == PRINT_ANTS)
            strcat(buf, printant(ADJ, synptr, wdnum + 1, vs, ""));
    }
}

/* Strip off () enclosed comments from a word */
void SafeWordNet::deadjify(std::string& word, int& adj_marker) const
{
    adj_marker = UNKNOWN_MARKER; /* default if not adj or unknown */

    for (size_t i = 0; i < word.size(); ++i) {
        if (word[i] == '(') {
            if (!strncmp(&word[i], "(a)", 3))
                adj_marker = ATTRIBUTIVE;
            else if (!strncmp(&word[i], "(ip)", 4))
                adj_marker = IMMED_POSTNOMINAL;
            else if (!strncmp(&word[i], "(p)", 3))
                adj_marker = PREDICATIVE;

            word = word.substr(0, i);
            assert(word.size() == i);
            return;
        }
    }
}

char* SafeWordNet::printant(int dbase, SynsetPtr synptr, int wdnum, const char *templ, char *tail)
{
    int i, j, wdoff;
    SynsetPtr psynptr;
    char tbuf[WORDBUF];
    int first = 1;

    printant_retbuf[0] = '\0';

    /* Go through all the pointers looking for anotnyms from the word
    indicated by wdnum.  When found, print all the antonym's
    antonym pointers which point back to wdnum. */

    for (i = 0; i < synptr->ptrcount; i++) {
        if (synptr->ptrtyp[i] == ANTPTR && synptr->pfrm[i] == wdnum) {

            psynptr = read_synset(dbase, synptr->ptroff[i], "");

            for (j = 0; j < psynptr->ptrcount; j++) {
                if (psynptr->ptrtyp[j] == ANTPTR &&
                    psynptr->pto[j] == wdnum &&
                    psynptr->ptroff[j] == synptr->hereiam) {

                    wdoff = (psynptr->pfrm[j] ? (psynptr->pfrm[j] - 1) : 0);

                    /* Construct buffer containing formatted antonym,
                    then add it onto end of return buffer */

                    int adj_marker = 0;
                    //strcpy(wdbuf, psynptr->words[wdoff]);
                    std::string word_buf = psynptr->words[wdoff];
                    deadjify(word_buf, adj_marker);
                    //strcpy(tbuf, deadjify(wdbuf));
                    strcpy(tbuf, word_buf.c_str());

                    /* Print additional lexicographer information and
                    WordNet sense number as indicated by flags */

                    if (global_prlexid && (psynptr->lexid[wdoff] != 0))
                        sprintf(tbuf + strlen(tbuf), "%d", psynptr->lexid[wdoff]);
                    if (global_wnsnsflag)
                        sprintf(tbuf + strlen(tbuf), "#%d", psynptr->wnsns[wdoff]);
                    if (!first)
                        strcat(printant_retbuf, tail);
                    else
                        first = 0;
                    sprintf(printant_retbuf + strlen(printant_retbuf), templ, tbuf);
                }
            }
            free_synset(psynptr);
        }
    }
    return printant_retbuf;
}

/* Return number of times sense is tagged */
int SafeWordNet::GetTagcnt(IndexPtr idx, int sense)
{
    char buf[256];
    int cnt = 0;

    if (cntlistfp) {
        std::string sensekey;
        WNSnsToStr(idx, sense, sensekey);

        std::string line = bin_search(sensekey.c_str(), cntlistfp, nullptr);
        if (!line.empty()) {
            int snum = 0;
            sscanf(line.c_str(), "%s %d %d", buf, &snum, &cnt);
        }
    }

    return cnt;
}

/* Convert WordNet sense number passed of IndexPtr entry to sense key. */
void SafeWordNet::WNSnsToStr(IndexPtr idx, int sense, std::string& ret)
{
    //printf("WNSnsToStr\n");

    SynsetPtr sptr, adjss;
    char sensekey[512], lowerword[256];
    int j, sstype, pos;

    pos = getpos(idx->pos);
    sptr = read_synset(pos, idx->offset[sense - 1], "");

    if ((sstype = getsstype(sptr->pos)) == SATELLITE) {
        for (j = 0; j < sptr->ptrcount; j++) {
            if (sptr->ptrtyp[j] == SIMPTR) {
                adjss = read_synset(sptr->ppos[j], sptr->ptroff[j], "");
                sptr->headword = (char*)malloc(strlen(adjss->words[0]) + 1);
                assert(sptr->headword);
                strcpy(sptr->headword, adjss->words[0]);
                strtolower(sptr->headword);

                assert(adjss->lexid[0] >= std::numeric_limits<short>::lowest());
                assert(adjss->lexid[0] <= std::numeric_limits<short>::max());
                sptr->headsense = static_cast<short>(adjss->lexid[0]);

                free_synset(adjss);
                break;
            }
        }
    }

    for (j = 0; j < sptr->wcount; j++) {
        strcpy(lowerword, sptr->words[j]);
        strtolower(lowerword);
        if (!strcmp(lowerword, idx->wd))
            break;
    }

    if (j == sptr->wcount) {
        free_synset(sptr);
        ret = "";
        return;
    }

    if (sstype == SATELLITE)
        _snprintf_s(sensekey, 512, "%s%%%-1.1d:%-2.2d:%-2.2d:%s:%-2.2d", idx->wd, SATELLITE, sptr->fnum, sptr->lexid[j], sptr->headword, sptr->headsense);
    else
        _snprintf_s(sensekey, 512, "%s%%%-1.1d:%-2.2d:%-2.2d::", idx->wd, pos, sptr->fnum, sptr->lexid[j]);

    free_synset(sptr);

    ret = sensekey;
}

/* Set bit for each POS that search word is in.  0 returned if word is not in WordNet. */
unsigned int SafeWordNet::in_wn(const char *word, int pos)
{
    int i;
    unsigned int retval = 0;

    if (pos == ALL_POS) {
        for (i = 1; i < NUMPARTS + 1; i++) {
            if (indexfps[i]) {
                std::string line = bin_search(word, indexfps[i], nullptr);
                if (!line.empty()) {
                    retval |= bit(i);
                }
            } else {
                //printf("Index file not opened 1\n");
            }
        }
    } else {
        if (indexfps[pos]) {
            std::string line = bin_search(word, indexfps[pos], nullptr);
            if (!line.empty()) {
                retval |= bit(pos);
            }
        } else {
            //printf("Index file not opened 2\n");
        }
    }

    return retval;
}

/* Determine if any of the synsets that this word is in have inherited meronyms or holonyms. */
int SafeWordNet::HasHoloMero(IndexPtr index, int ptrtyp)
{
    //printf("HasHoloMero\n");

    int i, j;
    SynsetPtr synset, psynset;
    int found = 0;
    int ptrbase;

    ptrbase = (ptrtyp == HMERONYM) ? HASMEMBERPTR : ISMEMBERPTR;

    for (i = 0; i < index->off_cnt; i++) {
        synset = read_synset(NOUN, index->offset[i], "");
        for (j = 0; j < synset->ptrcount; j++) {
            if (synset->ptrtyp[j] == HYPERPTR) {
                psynset = read_synset(NOUN, synset->ptroff[j], "");
                found += HasPtr(psynset, ptrbase);
                found += HasPtr(psynset, ptrbase + 1);
                found += HasPtr(psynset, ptrbase + 2);

                free_synset(psynset);
            }
        }
        free_synset(synset);
    }
    return(found);
}

int SafeWordNet::HasPtr(SynsetPtr synptr, int ptrtyp)
{
    for (int i = 0; i < synptr->ptrcount; i++) {
        if (synptr->ptrtyp[i] == ptrtyp) {
            return 1;
        }
    }

    return 0;
}

/*
 Search code interfaces to WordNet database
 findtheinfo_ds2() - return search results in linked list data structrure
*/
// Xin added depth param to give the option to trace all synset in ptrlist chain
SynsetPtr SafeWordNet::findtheinfo_ds2(const char *searchstr, int dbase, int ptrtyp, int whichsense, int depth2)
{
    IndexPtr idx;
    SynsetPtr cursyn;
    SynsetPtr synlist = nullptr, lastsyn = nullptr;
    int depth = depth2; //int depth = 0; modified by Xin to trace all synset in ptrlist chain
    int newsense = 0;

    wnresults.numforms = 0;
    wnresults.printcnt = 0;

    while ((idx = getindex(searchstr, dbase)) != nullptr) {
        searchstr = nullptr;    /* clear out for next call */
        newsense = 1;

        if (ptrtyp < 0) {
            ptrtyp = -ptrtyp;
            depth = 1;
        }

        assert(wnresults.numforms < MAX_FORMS);
        wnresults.SenseCount[wnresults.numforms] = idx->off_cnt;
        assert(wnresults.numforms < MAX_FORMS);
        wnresults.OutSenseCount[wnresults.numforms] = 0;
        wnresults.searchbuf = nullptr;
        wnresults.searchds = nullptr;

        /* Go through all of the searchword's senses in the database and perform the search requested. */
        for (global_sense = 0; global_sense < idx->off_cnt; global_sense++) {
            if (whichsense == ALLSENSES || whichsense == global_sense + 1) {
                //printf("%llx global_sense: %d\n", idx->offset, global_sense);
                cursyn = read_synset(dbase, idx->offset[global_sense], idx->wd);
                if (lastsyn) {
                    if (newsense)
                        lastsyn->nextform = cursyn;
                    else
                        lastsyn->nextss = cursyn;
                }

                if (!synlist)
                    synlist = cursyn;

                newsense = 0;

                cursyn->searchtype = ptrtyp;
                cursyn->ptrlist = traceptrs_ds(cursyn, ptrtyp, getpos(cursyn->pos), depth);

                lastsyn = cursyn;

                if (whichsense == global_sense + 1)
                    break;
            }
        }

        free_index(idx);
        wnresults.numforms++;

        if (ptrtyp == COORDS) {	/* clean up by removing hypernym */
            lastsyn = synlist->ptrlist;
            synlist->ptrlist = lastsyn->ptrlist;
            free_synset(lastsyn);
        }
    }
    wnresults.searchds = synlist;
    return synlist;
}

/* Recursive search algorithm to trace a pointer tree and return results
in linked list of data structures. */
SynsetPtr SafeWordNet::traceptrs_ds(SynsetPtr synptr, int ptrtyp, int dbase, int depth)
{
    UNREF_PARAM(dbase);

    int i;
    SynsetPtr cursyn, synlist = nullptr, lastsyn = nullptr;
    int tstptrtyp, docoords;

    /* If synset is a satellite, find the head word of its
    head synset and the head word's sense number. */

    if (getsstype(synptr->pos) == SATELLITE) {
        for (i = 0; i < synptr->ptrcount; i++) {
            if (synptr->ptrtyp[i] == SIMPTR) {
                cursyn = read_synset(synptr->ppos[i], synptr->ptroff[i], "");
                synptr->headword = (char*)malloc(strlen(cursyn->words[0]) + 1);
                assert(synptr->headword);
                strcpy(synptr->headword, cursyn->words[0]);

                assert(cursyn->lexid[0] >= std::numeric_limits<short>::lowest() && "less than lower bound of lexid");
                assert(cursyn->lexid[0] <= std::numeric_limits<short>::max() && "larger than upper bound of lexid");
                synptr->headsense = static_cast<short>(cursyn->lexid[0]);
                free_synset(cursyn);
                break;
            }
        }
    }

    if (ptrtyp == COORDS) {
        tstptrtyp = HYPERPTR;
        docoords = 1;
    }
    else {
        tstptrtyp = ptrtyp;
        docoords = 0;
    }

    for (i = 0; i < synptr->ptrcount; i++) {
        if ((synptr->ptrtyp[i] == tstptrtyp) &&
            ((synptr->pfrm[i] == 0) ||
            (synptr->pfrm[i] == synptr->whichword))) {

            cursyn = read_synset(synptr->ppos[i], synptr->ptroff[i], "");
            cursyn->searchtype = ptrtyp;

            if (lastsyn)
                lastsyn->nextss = cursyn;

            if (!synlist)
                synlist = cursyn;

            lastsyn = cursyn;

            if (depth) {
                depth = depthcheck(depth, cursyn);
                cursyn->ptrlist = traceptrs_ds(cursyn, ptrtyp, getpos(cursyn->pos), (depth + 1));
            }
            else if (docoords) {
                cursyn->ptrlist = traceptrs_ds(cursyn, HYPOPTR, NOUN, 0);
            }
        }
    }

    return(synlist);
}

int SafeWordNet::depthcheck(int depth, SynsetPtr synptr)
{
    if (depth >= MAXDEPTH) {
        char msgbuf[256];
        _snprintf_s(msgbuf, 256, "WordNet library error: Error Cycle detected\n   %s\n", synptr->words[0]);
        display_message(msgbuf);
        depth = -1;     /* reset to get one more trace then quit */
    }
    return depth;
}

int SafeWordNet::wninit()
{
    if (!wninit_done) {
        wninit_openerr = do_init();

        if (!wninit_openerr) {
            wninit_done = 1;
            OpenDB = 1;
            wninit_openerr = morphinit();
        }
    }

    return wninit_openerr;
}

int SafeWordNet::re_wninit(void)
{
    closefps();

    int openerr = do_init();

    if (!openerr) {
        OpenDB = 1;
        openerr = re_morphinit();
    }

    return openerr;
}

void SafeWordNet::closefps(void)
{
    int i;

    if (OpenDB) {
        for (i = 1; i < NUMPARTS + 1; i++) {
            if (datafps[i]) {
                rfclose(datafps[i]);
                datafps[i] = nullptr;
            }

            if (indexfps[i]) {
                rfclose(indexfps[i]);
                indexfps[i] = nullptr;
            }
        }

        if (sensefp) {
            rfclose(sensefp);
            sensefp = nullptr;
        }

        if (cntlistfp) {
            rfclose(cntlistfp);
            cntlistfp = nullptr;
        }

        if (keyindexfp) {
            rfclose(keyindexfp);
            keyindexfp = nullptr;
        }

        if (vsentfilefp) {
            rfclose(vsentfilefp);
            vsentfilefp = nullptr;
        }

        if (vidxfilefp) {
            rfclose(vidxfilefp);
            vidxfilefp = nullptr;
        }

        OpenDB = 0;
    }
}

int SafeWordNet::do_init()
{
    int openerr = 0;
    char tmpbuf[256];

    for (int i = 1; i < NUMPARTS + 1; i++) {
        _snprintf_s(tmpbuf, 256, DATAFILE, partnames[i]);
        if ((datafps[i] = rfopen(tmpbuf, "r")) == nullptr) {
            char msgbuf[256];
            _snprintf_s(msgbuf, 256, "WordNet library error: Can't open datafile(%s)\n", tmpbuf);
            display_message(msgbuf);
            openerr = -1;
        }

        _snprintf_s(tmpbuf, 256, INDEXFILE, partnames[i]);
        if ((indexfps[i] = rfopen(tmpbuf, "r")) == nullptr) {
            char msgbuf[256];
            _snprintf_s(msgbuf, 256, "WordNet library error: Can't open indexfile(%s)\n", tmpbuf);
            display_message(msgbuf);
            openerr = -1;
        }
    }

    /* This file isn't used by the library and doesn't have to
    be present.  No error is reported if the open fails. */
    strcpy(tmpbuf, SENSEIDXFILE);
    sensefp = rfopen(tmpbuf, "r");

    /* If this file isn't present, the runtime code will skip printint out
    the number of times each sense was tagged. */
    strcpy(tmpbuf, CNTLISTFILE);
    cntlistfp = rfopen(tmpbuf, "r");

    /* This file doesn't have to be present.  No error is reported if the open fails. */
    //strcpy(tmpbuf, KEYIDXFILE);
    //keyindexfp = rfopen(tmpbuf, "r");

    //strcpy(tmpbuf, REVKEYIDXFILE);
    //revkeyindexfp = rfopen(tmpbuf, "r");

    strcpy(tmpbuf, VRBSENTFILE);
    if ((vsentfilefp = rfopen(tmpbuf, "r")) == nullptr) {
        char msgbuf[256];
        _snprintf_s(msgbuf, 256, "WordNet library warning: Can't open verb example sentence file(%s)\n", tmpbuf);
        display_message(msgbuf);
    }

    strcpy(tmpbuf, VRBIDXFILE);
    if ((vidxfilefp = rfopen(tmpbuf, "r")) == nullptr) {
        char msgbuf[256];
        _snprintf_s(msgbuf, 256, "WordNet library warning: Can't open verb example sentence index file(%s)\n", tmpbuf);
        display_message(msgbuf);
    }

    return openerr;
}

/* Count the number of underscore or space separated words in a string. */
int SafeWordNet::cntwords(const char* s, char separator) const
{
    int wdcnt = 0;

    while (*s) {
        if (*s == separator || *s == ' ' || *s == '_') {
            wdcnt++;
            while (*s && (*s == separator || *s == ' ' || *s == '_'))
                s++;
        }
        else
            s++;
    }

    return ++wdcnt;
}