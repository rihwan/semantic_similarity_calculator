/*
 * Node.js addon Semantic Similarity Calculator
 * Copyright (C) 2016  Innfarn Yoo (rihwan@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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