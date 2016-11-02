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