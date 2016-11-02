/*
  binsearch.c - general binary search functions
*/
#include "stdafx.h"
#include "memory_file_helper.h"

/* Binary search - looks for the key passed at the start of a line
   in the file associated with open file descriptor fp, and returns
   a buffer containing the line in the file. */

#define KEY_LEN     (1024)
#define LINE_LEN    (1024*25)

///* General purpose binary search function to search for key as first
//   item on line in open file.  Item is delimited by space. */
//std::string read_index(long offset, RFILE *fp)
//{
//    rfseek(fp, offset, SEEK_SET);
//
//    const char* s = nullptr;
//    const char* e = nullptr;
//    rfgets_start_end(fp, s, e);
//
//    assert(s);
//    assert(e);
//
//    const int size = static_cast<int>(e - s);
//
//    std::string line = "";
//    while (s != e) {
//        line.push_back(*s++);
//    }
//    assert(static_cast<int>(line.size()) == size);
//
//    return line;
//}

//void trim_space(char* key)
//{
//    int len = static_cast<int>(strlen(key));
//    int count = 0;
//    for (int i = 0; i < len; ++i) {
//        if (key[i] == ' ' || key[i] == '\t' || key[i] == '\r' || key[i] == '\n')
//            count += 1;
//        else
//            break;
//    }
//
//    if (count > 0 && len < 256) {
//        char temp[256];
//        strcpy(temp, key + count);
//        strcpy(key, temp);
//    }
//
//    len = static_cast<int>(strlen(key));
//    for (int i = len - 1; i >= 0; --i) {
//        if (key[i] == ' ' || key[i] == '\t' || key[i] == '\r' || key[i] == '\n')
//            key[i] = '\0';
//        else
//            break;
//    }
//}
//
//std::string bin_search(const char *searchkey, RFILE *fp, long* last_bin_search_offset_ptr)
//{
//    char key[KEY_LEN];
//
//    std::unique_ptr<char> line_buffer(new char[LINE_LEN]);
//    char* linep = line_buffer.get();
//    linep[0] = '\0';
//
//    long top = 0;
//    long bot = get_file_size(fp);
//    long mid = (bot - top) / 2;
//    long diff = 1;
//
//    int comparison_result;
//    do {
//        rfseek(fp, mid - 1, SEEK_SET);
//
//        if (mid != 1) {
//            goto_next_line(fp);
//        }
//
//        if (last_bin_search_offset_ptr) {
//            (*last_bin_search_offset_ptr) = rftell(fp);
//        }
//
//        rfgets(linep, LINE_LEN, fp);
//        const int length = (int)(strchr(linep, ' ') - linep);
//        strncpy(key, linep, length);
//        key[length] = '\0';
//        trim_space(key);
//
//        comparison_result = strcmp(key, searchkey);
//        if (comparison_result < 0) {
//            top = mid;
//            diff = (bot - top) / 2;
//            mid = top + diff;
//        } else if (comparison_result > 0) {
//            bot = mid;
//            diff = (bot - top) / 2;
//            mid = top + diff;
//        } else {
//            break;
//        }
//
//    } while (comparison_result != 0 && diff != 0);
//
//    if (!comparison_result) {
//        return std::string(line_buffer.get());
//    } else {
//        return std::string();
//    }
//}
