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
#include "stdafx.h"

#ifdef  EXE_BUILD

#include "resource.h"
#include "res_file_manager.h"
#include "rc_helper.h"

#include "semantic_calculator.h"

// Resource files
ResFileManager g_ResFileManager;

std::vector<SemanticCalculator*> g_SCForClear;
__declspec(thread) SemanticCalculator* g_SemanticCalculator = nullptr;
__declspec(thread) bool g_SemanticCalculatorInitialized = false;

void clear();
void addResourceFiles();
double calculateSemanticSimilarity(const std::string& s1, const std::string& s2);

int _tmain(int argc, _TCHAR *argv[])
{
    if (argc < 3) {
        return -1;
    }

    addResourceFiles();

    double similarity = calculateSemanticSimilarity(argv[1], argv[2]);
    printf("%s <--> %s Similarity: %f\n", argv[1], argv[2], similarity);

    clear();
    return 0;
}

double calculateSemanticSimilarity(const std::string& s1, const std::string& s2)
{
    // Thread Local Storage (TLS)
    if (!g_SemanticCalculatorInitialized) {
        // We probably need to load database from resource!
        // Local file access failed.
        assert(nullptr == g_SemanticCalculator);
        g_SemanticCalculator = new SemanticCalculator();

        // This is only used for clearing at exit hooking
        g_SCForClear.push_back(g_SemanticCalculator);

        g_SemanticCalculator->openDB();

        g_SemanticCalculatorInitialized = true;
    }

    if (g_SemanticCalculator) {
        // Calling below lines cause runtime crash. Need to debug.
        double s = g_SemanticCalculator->calcSemanticSimilarity(s1, s2);
        return s;
    } else {
        return -1.0f;
    }
}

void addResourceFiles()
{
    // Tricky! Be careful.
    // Adding all resource files
    assert((sizeof(RES_FILENAMES) / sizeof(const char*)) == 121 - 103 + 1);
    for (int i = 103; i <= 121; ++i) {
        g_ResFileManager.addResId(i, RES_FILENAMES[i - 103]);
    }
}

void clear()
{
    g_ResFileManager.clear();

    for (size_t i = 0; i < g_SCForClear.size(); ++i) {
        delete g_SCForClear[i];
        g_SCForClear[i] = nullptr;
    }
}

#endif//_USRDLL