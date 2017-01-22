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
#ifndef __MEMORY_FILE_H__
#define __MEMORY_FILE_H__

class MemoryFile
{
private:
    std::string fileName_;
    const void* originalPtr_;
    unsigned int originalSize_;
    const char* readPtr_;

    void skipIfNewLineCharExist();

public:
    MemoryFile(const void* ptr, unsigned int size, const std::string& file_name);
    MemoryFile(const MemoryFile& file);

    MemoryFile& operator=(const MemoryFile& rhs);

    bool valid() const;
    bool eof() const;

    unsigned int getOriginalSize() const;

    int seek(long offset, int origin);
    long int tell() const;

    char* gets(char* str, int num);
    void gets_start_end(const char*& s, const char*& e);

    void rewind();
    int getc();
    int peek() const;
};

#endif//__MEMORY_FILE_H__