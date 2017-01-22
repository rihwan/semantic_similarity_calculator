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
#include "stdafx.h"
#include "memory_file_helper.h"
#include "memory_file.h"
#include "res_file_manager.h"

extern ResFileManager g_ResFileManager;

MemoryFile* mfopen(const char* file_name, const char*)
{
    int id = g_ResFileManager.getResIdByFileName(file_name);
    if (id > 0) {
        const void* ptr = g_ResFileManager.getResData(id);
        unsigned int size = g_ResFileManager.getResSize(id);
        const std::string& file_name = g_ResFileManager.getResFileName(id);
        return new MemoryFile(ptr, size, file_name);
    } else {
        //printf("%s failed to open\n", file_name);
        return nullptr;
    }
}

void mfclose(MemoryFile* file)
{
    if (file)
    {
        delete file;
        file = nullptr;
    }
}

int mfseek(MemoryFile* file, long offset, int origin)
{
    return file->seek(offset, origin);
}

long int mftell(MemoryFile* file)
{
    return file->tell();
}

char* mfgets(char* str, int num, MemoryFile* file)
{
    return file->gets(str, num);
}

void mrewind(MemoryFile* file)
{
    file->rewind();
}

int mgetc(MemoryFile* file)
{
    return file->getc();
}

void goto_next_line(RFILE* fp)
{
    int ch;
#ifdef USE_RESOURCE_FILE
    ch = fp->peek();
    while (ch != EOF && ch != '\r' && ch != '\n') {
        fp->getc();
        ch = fp->peek();
    }

    if (ch == '\r') {
        fp->getc();
        ch = fp->peek();
        if (ch == '\n') {
            fp->getc();
        }
    }
    else if (ch == '\n') {
        fp->getc();
        ch = fp->peek();
        if (ch == '\r') {
            fp->getc();
        }
    }
#else
    while (ch = getc(fp)) {
        if (ch == EOF) {
            break;
        } else if (ch == '\r') {
            ch = getc(fp);
            if (ch != '\n') {
                ungetc(ch, fp);
            }
            break;
        } else if (ch == '\n') {
            ch = getc(fp);
            if (ch != '\r') {
                ungetc(ch, fp);
            }
            break;
        }
    }
#endif
}

int read_till_next_line(RFILE* fp, char* str, int num)
{
    int length = 0;
    int ch;
#ifdef USE_RESOURCE_FILE
    ch = fp->peek();
    while (ch != EOF && ch != '\r' && ch != '\n' && length < num - 1) {
        ch = fp->getc();
        str[length++] = static_cast<char>(ch);
        ch = fp->peek();
    }

    if (ch == '\r') {
        fp->getc();
        ch = fp->peek();
        if (ch == '\n') {
            fp->getc();
        }
    } else if (ch == '\n') {
        fp->getc();
        ch = fp->peek();
        if (ch == '\r') {
            fp->getc();
        }
    }
#else
    while (ch = getc(fp) && length < num - 1) {
        if (ch == EOF) { // end of file
            break;
        } else if (ch == '\r') {
            ch = getc(fp);
            if (ch != '\n') {
                ungetc(ch, fp);
            }
            break;
        } else if (ch == '\n') {
            ch = getc(fp);
            if (ch != '\r') {
                ungetc(ch, fp);
            }
            break;
        } else {
            str[length++] = static_cast<char>(ch);
        }
    }
#endif
    str[length] = '\0';
    return length;
}

long get_file_size(RFILE* fp)
{
#ifdef USE_RESOURCE_FILE
    return static_cast<long>(fp->getOriginalSize());
#else
    long offset = ftell(fp);
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, offset, SEEK_SET);
    return size;
#endif
}

void rfgets_start_end(MemoryFile* file, const char*& s, const char*& e)
{
    file->gets_start_end(s, e);
}

std::string trim_front_space(const std::string& s)
{
    size_t i = 0;
    for (i = 0; i < s.size(); ++i) {
        if (s[i] != ' ') {
            break;
        }
    }

    if (i == 0) {
        return s;
    } else {
        return s.substr(i);
    }
}