/*
Node.js addon Semantic Similarity Calculator
Copyright(C) DooAni 2016  Innfarn Yoo (rihwan@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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