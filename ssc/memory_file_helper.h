/*
 * Node.js addon Semantic Similarity Calculator
 * Copyright (C) 2017 Innfarn Yoo (rihwan@gmail.com)

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __MEMORY_FILE_HELPER_H__
#define __MEMORY_FILE_HELPER_H__

#define RFILE MemoryFile

#define rfopen  mfopen
#define rfseek  mfseek
#define rfclose mfclose
#define rftell  mftell
#define rfgets  mfgets
#define rrewind mrewind
#define rgetc   mgetc

class MemoryFile;

MemoryFile* mfopen(const char* file_name, const char* mode);
void        mfclose(MemoryFile* file);
int         mfseek(MemoryFile* file, long offset, int origin);
long int    mftell(MemoryFile* file);
char*       mfgets(char* str, int num, MemoryFile* file);
void        mrewind(MemoryFile* file);
int         mgetc(MemoryFile* file);

void goto_next_line(RFILE* fp);
int read_till_next_line(RFILE* fp, char* str, int num);
long get_file_size(RFILE* fp);
void rfgets_start_end(MemoryFile* file, const char*& s, const char*& e);
std::string trim_front_space(const std::string& s);

#endif//__MEMORY_FILE_HELPER_H__