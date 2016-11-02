/*
 * Node.js addon Semantic Similarity Calculator
 * Copyright  2016  Innfarn Yoo (rihwan@gmail.com)
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
#ifndef __RES_FILE_MANAGER_H__
#define __RES_FILE_MANAGER_H__

class ResFileManager
{
private:
    std::vector<int> resIds_;

    std::vector<void*> resData_;
    std::vector<unsigned int> resSize_;
    std::vector<std::string> resFileName_;

private:
    ResFileManager(const ResFileManager&);
    ResFileManager(ResFileManager&&);
    ResFileManager& operator=(const ResFileManager&);
    ResFileManager& operator=(ResFileManager&&);

    void loadResource(size_t index);

public:
    ResFileManager();
    virtual ~ResFileManager();

    size_t addResId(int id, const char* res_file_name);
    void clear();

    int getResIdByFileName(const char* res_file_name) const;

    size_t getNumResFiles() const;

    const void* getResData(int id);
    unsigned int getResSize(int id);
    const std::string& getResFileName(int id);
};

#endif//__RES_FILE_MANAGER_H__