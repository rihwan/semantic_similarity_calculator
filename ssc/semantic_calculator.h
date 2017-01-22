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
#ifndef __SEMANTIC_CALCULATOR_H__
#define __SEMANTIC_CALCULATOR_H__

#include "wordnet_extended.h"
#include "sentence_similarity.h"

class SemanticCalculator
    : public WordnetExtended
{
public:
    typedef SemanticCalculator my_type;

private:
    SentenceSimilarityLi2006* sentenceSimilarity_;

    double weightPerfectMatch_;
    double weightSentenceMatch_;

private:
    SemanticCalculator(const my_type&);
    SemanticCalculator(my_type&&);
    SemanticCalculator& operator=(const my_type&);
    SemanticCalculator& operator=(my_type&&);

    void normalize(std::vector<std::string>& keywords);
    void getSynsets(const std::string& word, std::vector<std::string>& synsets);

public:
    SemanticCalculator(int freq_dict_res_id);
    virtual ~SemanticCalculator();

    bool openDB();
    void closeDB();

    double calcSemanticSimilarity(
        const std::string& src_keywords,
        const std::string& dst_keywords);
};

#endif//__SEMANTIC_CALCULATOR_H__