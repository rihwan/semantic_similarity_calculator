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