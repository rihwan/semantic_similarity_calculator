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
#ifndef __STDAFX_H__
#define __STDAFX_H__

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <strstream>
#include <iterator>
#include <memory>

#pragma warning(push)
#pragma warning(disable:4100)
#pragma warning(disable:4512)
#pragma warning(disable:4510)
#pragma warning(disable:4610)
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/tokenizer.hpp>
#include <boost/graph/breadth_first_search.hpp>
#pragma warning(pop)

#include <Windows.h>
#include <tchar.h>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cctype>

//#ifndef EXE_BUILD
//#define EXE_BUILD
//#endif

#endif//__STDAFX_H__