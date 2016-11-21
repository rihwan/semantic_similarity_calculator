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
#include "stdafx.h"
#include "res_file_manager.h"
#include "resource.h"

ResFileManager::ResFileManager()
{
}

ResFileManager::~ResFileManager()
{
    clear();
}

size_t ResFileManager::addResId(int id, const char* res_file_name)
{
    assert(resIds_.size() == resData_.size());
    assert(resIds_.size() == resSize_.size());
    assert(resIds_.size() == resFileName_.size());
    assert(nullptr != res_file_name);

    for (size_t i = 0; i < resIds_.size(); ++i) {
        if (resIds_[i] == id) {
            return i;
        }
    }

    const size_t idx = resIds_.size();

    resIds_.push_back(id);
    resData_.push_back(nullptr);
    resSize_.push_back(0);
    resFileName_.push_back(res_file_name);

    assert(resData_.size() == resIds_.size());
    assert(resSize_.size() == resIds_.size());

    return idx;
}

void ResFileManager::clear()
{
    resIds_.clear();
    resData_.clear();
    resSize_.clear();
    resFileName_.clear();
}

int ResFileManager::getResIdByFileName(const char* res_file_name) const
{
    for (size_t i = 0; i < resFileName_.size(); ++i) {
        if (resFileName_[i] == res_file_name) {
            return resIds_[i];
        }
    }

    return -1;
}

const void* ResFileManager::getResData(int id)
{
    assert(resData_.size() == resIds_.size());
    for (size_t i = 0; i < resIds_.size(); ++i) {
        if (resIds_[i] == id) {
            loadResource(i);
            return resData_[i];
        }
    }

    assert(false);
    return nullptr;
}

unsigned int ResFileManager::getResSize(int id)
{
    assert(resSize_.size() == resIds_.size());
    for (size_t i = 0; i < resIds_.size(); ++i) {
        if (resIds_[i] == id) {
            loadResource(i);
            return resSize_[i];
        }
    }

    assert(false);
    return 0;
}

const std::string& ResFileManager::getResFileName(int id)
{
    assert(resFileName_.size() == resIds_.size());
    for (size_t i = 0; i < resIds_.size(); ++i) {
        if (resIds_[i] == id) {
            loadResource(i);
            return resFileName_[i];
        }
    }

    static const std::string empty_string;
    assert(false);
    return empty_string;
}

void ResFileManager::loadResource(size_t index)
{
    int res_id = resIds_[index];
    if (resData_[index] != nullptr)
        return;

    HMODULE hModule = GetModuleHandle("ssc.node");

    HRSRC res = ::FindResource(hModule, MAKEINTRESOURCE(res_id), RT_RCDATA);
    if (res == 0) {
        //printf("Failed to find resource!\n");
        return;
    }

    unsigned int res_size = ::SizeofResource(hModule, res);

    HGLOBAL res_data = ::LoadResource(hModule, res);
    void* res_data_ptr = ::LockResource(res_data);

    resData_[index] = res_data_ptr;
    resSize_[index] = res_size;
}