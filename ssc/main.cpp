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