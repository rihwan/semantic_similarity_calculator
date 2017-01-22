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
#include "memory_file.h"

MemoryFile::MemoryFile(const void* ptr, unsigned int size, const std::string& file_name)
    : originalPtr_(ptr), originalSize_(size), fileName_(file_name)
{
    readPtr_ = (const char*)ptr;
}

MemoryFile::MemoryFile(const MemoryFile& rhs)
    : originalPtr_(rhs.originalPtr_)
    , originalSize_(rhs.originalSize_)
    , readPtr_(rhs.readPtr_)
    , fileName_(rhs.fileName_)
{
}

MemoryFile& MemoryFile::operator=(const MemoryFile& rhs)
{
    if (this != &rhs) {
        originalPtr_ = rhs.originalPtr_;
        originalSize_ = rhs.originalSize_;
        readPtr_ = rhs.readPtr_;
        fileName_ = rhs.fileName_;
    }
    return (*this);
}

bool MemoryFile::valid() const
{
    const char* start = (const char*)originalPtr_;
    const char* end = start + originalSize_;
    if (readPtr_ - start < 0)
        return false;
    else if (end - readPtr_ < 0)
        return false;
    else
        return true;
}

bool MemoryFile::eof() const
{
    const char* start = (const char*)originalPtr_;
    const char* end = start + originalSize_;
    return (end == readPtr_);
}

unsigned int MemoryFile::getOriginalSize() const
{
    return originalSize_;
}

int MemoryFile::seek(long offset, int origin)
{
    assert(valid());
    if (originalPtr_) {
        if (SEEK_SET == origin) {
            if (offset < 0) {
                return -1;
            } else if (static_cast<unsigned int>(offset) > originalSize_) {
                return -2;
            }

            readPtr_ = (const char*)originalPtr_;
            readPtr_ += offset;

            assert(valid());

            return 0;
        } else if (SEEK_CUR == origin) {
            int diff = static_cast<int>(readPtr_ - (const char*)originalPtr_);
            assert(diff >= 0);

            if (static_cast<long>(diff)+offset < 0) {
                return -1;
            } else if (static_cast<long>(diff)+offset > static_cast<long>(originalSize_)) {
                return -2;
            }

            readPtr_ += offset;
            assert(valid());

            return 0;
        } else if (SEEK_END == origin) {
            if (offset > 0) {
                return -2;
            } else if (static_cast<unsigned int>(abs(offset)) > originalSize_) {
                return -1;
            }

            readPtr_ = (const char*)originalPtr_;
            readPtr_ += originalSize_; // Set to the end
            readPtr_ += offset;
            assert(valid());

            return 0;
        } else {
            return -4;
        }
    } else {
        return -3;
    }
}

long int MemoryFile::tell() const
{
    assert(valid());
    long int offset = static_cast<long int>(readPtr_ - (const char*)originalPtr_);
    return offset;
}

char* MemoryFile::gets(char* str, int num)
{
    assert(valid());
    assert(nullptr != str);
    assert(num > 0);

    long int offset = tell();
    const long int size = static_cast<long int>(originalSize_);

    if (offset == size) {
        return nullptr;
    }

    if (nullptr == str || num <= 0) {
        return nullptr;
    }

    int count = 0;
    while (offset < size && count < (num - 1)) {
        if (*readPtr_ == '\r' || *readPtr_ == '\n') {
            break;
        } else {
            str[count++] = *readPtr_++;
        }

        offset = tell();
    }

    assert(count <= num - 1);
    str[count] = '\0';

    assert(valid());

    return str;
}

void MemoryFile::gets_start_end(const char*& s, const char*& e)
{
    assert(valid());

    s = readPtr_;
    e = readPtr_;

    long int offset = tell();
    const long int size = static_cast<long int>(originalSize_);

    if (offset == size) {
        return;
    }

    int count = 0;
    while (offset < size) {
        if (*readPtr_ == '\r' || *readPtr_ == '\n') {
            break;
        } else {
            readPtr_++;
        }

        offset = tell();
    }

    e = readPtr_;
    e++;

    assert(static_cast<int>(e - s) == count);

    assert(valid());
}

int MemoryFile::getc()
{
    assert(valid());
    const long int offset = tell();
    const long int size = static_cast<long int>(originalSize_);

    if (offset == size) {
        return EOF;
    }

    return (int)(*readPtr_++);
}

int MemoryFile::peek() const
{
    assert(valid());
    const long int offset = tell();
    const long int size = static_cast<long int>(originalSize_);

    if (offset == size) {
        return EOF;
    }

    return (int)(*readPtr_);
}

void MemoryFile::rewind()
{
    readPtr_ = (const char*)originalPtr_;
}

void MemoryFile::skipIfNewLineCharExist()
{
    if (*readPtr_ == '\r') {
        if (*(readPtr_ + 1) == '\n') {
            readPtr_ += 2;
            return;
        } else {
            readPtr_ += 1;
            return;
        }
    } else if (*readPtr_ == '\n') {
        if (*(readPtr_ + 1) == '\r') {
            readPtr_ += 2;
            return;
        } else {
            readPtr_ += 1;
            return;
        }
    }
}