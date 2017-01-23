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

#ifndef EXE_BUILD

#pragma warning(push)
#pragma warning(disable:4512)
#pragma warning(disable:4100)
#pragma warning(disable:4127)
#include "node.h"
#pragma warning(pop)

#include "resource.h"
#include "res_file_manager.h"
#include "rc_helper.h"

#include "semantic_calculator.h"

// Resource files
ResFileManager g_ResFileManager;

std::vector<SemanticCalculator*> g_SCForClear;
__declspec(thread) SemanticCalculator* g_SemanticCalculator = nullptr;
__declspec(thread) bool g_SemanticCalculatorInitialized = false;

namespace ssc {

    using node::AtExit;

    using v8::Exception;
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Number;
    using v8::Object;
    using v8::String;
    using v8::Value;

    void calculateSemanticSimilarity(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();

        if (args.Length() != 2) {
            // Throw an Error that is passed back to JavaScript
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
            return;
        }

        if (!args[0]->IsString() || !args[1]->IsString()) {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
            return;
        }

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
            std::string s1(*v8::String::Utf8Value(args[0]->ToString()));
            std::string s2(*v8::String::Utf8Value(args[1]->ToString()));

            const double s = g_SemanticCalculator->calcSemanticSimilarity(s1, s2);
            Local<Number> num = Number::New(isolate, static_cast<double>(s));

            args.GetReturnValue().Set(num);
        } else {
            isolate->ThrowException(Exception::ReferenceError(String::NewFromUtf8(isolate, "SentenceSimilarity Engine not initialized")));
            return;
        }
    }

    void addResourceFiles()
    {
        // Tricky! Be careful.
        // Adding all resource files
        assert((sizeof(RES_FILENAMES) / sizeof(const char*)) == 120 - 103 + 1);
        for (int i = 103; i <= 121; ++i) {
            g_ResFileManager.addResId(i, RES_FILENAMES[i - 103]);
        }
    }

    static void clear(void*)
    {
        g_ResFileManager.clear();

        for (size_t i = 0; i < g_SCForClear.size(); ++i) {
            delete g_SCForClear[i];
            g_SCForClear[i] = nullptr;
        }
    }

    void init(Local<Object> exports) {
        AtExit(clear);
        addResourceFiles();
        NODE_SET_METHOD(exports, "calculateSemanticSimilarity", calculateSemanticSimilarity);
    }

    NODE_MODULE(addon, init)
}

#endif