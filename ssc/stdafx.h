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