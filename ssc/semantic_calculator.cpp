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
#include "semantic_calculator.h"

SemanticCalculator::SemanticCalculator(int freq_dict_res_id)
    : WordnetExtended(freq_dict_res_id)
    , sentenceSimilarity_(nullptr)
    , weightPerfectMatch_(0.5)
    , weightSentenceMatch_(0.5)
{
}

SemanticCalculator::~SemanticCalculator()
{
    closeDB();
}

bool SemanticCalculator::openDB()
{
    if (!wordnet_.getOpenDB()) {
        // wninit return 0 as no error
        if (wordnet_.wninit()) {
            std::cout << "Failed to open wordnet files" << std::endl;
            return false;
        }
    } else {
        std::cout << "Tried to reopen wordnet files" << std::endl;
    }

    if (!sentenceSimilarity_) {
        sentenceSimilarity_ = new SentenceSimilarityLi2006((*this));
    }

    return true;
}

void SemanticCalculator::closeDB()
{
    delete sentenceSimilarity_;
    sentenceSimilarity_ = nullptr;

    if (wordnet_.getOpenDB()) {
        wordnet_.closefps();
    } else {
        std::cout << "Close WordNet Database" << std::endl;
    }
}

double SemanticCalculator::calcSemanticSimilarity(
    const std::string& src_keywords,
    const std::string& dst_keywords)
{
    if (!wordnet_.getOpenDB()) {
        std::cout << "WordNet DB is not opened" << std::endl;
        return -1.0;
    }

    boost::tokenizer<> src_tok(src_keywords);
    boost::tokenizer<> dst_tok(dst_keywords);

    std::vector<std::string> src_v(src_tok.begin(), src_tok.end());
    std::vector<std::string> dst_v(dst_tok.begin(), dst_tok.end());

    if (src_v.size() > dst_v.size()) {
        src_v.swap(dst_v);
    }

    normalize(src_v);
    normalize(dst_v);

    double similarity_perfect_match = 0.0;

    std::vector<std::string> src_synset;
    std::vector<std::string> dst_synset;

    for (size_t src_i = 0; src_i < src_v.size(); ++src_i) {
        src_synset.clear();

        const std::string& src_word = src_v[src_i];
        bool src_synset_retrieved = false;

        for (size_t dst_i = src_i; dst_i < dst_v.size(); ++dst_i) {
            const std::string& dst_word = dst_v[dst_i];

            // Already same word! No need to compare
            if (src_word == dst_word) {
                similarity_perfect_match += 1.0 / static_cast<double>(src_v.size());
                break;
            } else {
                if (!src_synset_retrieved) {
                    getSynsets(src_word, src_synset);
                    src_synset_retrieved = true;
                }

                dst_synset.clear();
                getSynsets(dst_word, dst_synset);

                bool found_same = false;
                for (size_t si = 0; si < src_synset.size(); ++si) {
                    for (size_t di = 0; di < dst_synset.size(); ++di) {
                        if (src_synset[si] == dst_synset[di]) {
                            found_same = true;
                            break;
                        }
                    }

                    if (found_same)
                        break;
                }

                if (found_same) {
                    similarity_perfect_match += 1.0 / static_cast<double>(src_v.size());
                    break;
                }
            }
        }
    }

    WordnetExtended::UndirectedGraph g;
    double similarity_sentence_match = sentenceSimilarity_->compute_similarity(src_keywords, dst_keywords, g);

    return ((similarity_perfect_match*weightPerfectMatch_) + (similarity_sentence_match*weightSentenceMatch_));
}

void SemanticCalculator::normalize(std::vector<std::string>& keywords)
{
    if (!wordnet_.getOpenDB()) {
        std::cout << "WordNet DB is not opened" << std::endl;
        return;
    }

    for (size_t i = 0; i < keywords.size(); ++i) {
        std::string word = keywords[i];

        if (!wordnet_.in_wn(word.c_str(), ALL_POS)) {
            for (int j = 1; j <= 4; ++j) {
                const char* morph = wordnet_.morphword(word.c_str(), j);
                if (morph) {
                    keywords[i] = morph;
                    break;
                }
            }
        }
    }
}

void SemanticCalculator::getSynsets(const std::string& word, std::vector<std::string>& synsets)
{
    int all_poses = wordnet_.in_wn(word.c_str(), ALL_POS);
    for (int i_pos = 2; i_pos <= 8; i_pos = i_pos << 1) {
        if (all_poses & i_pos) {
            SynsetPtr synset_sense = wordnet_.findtheinfo_ds2(word.c_str(), static_cast<int>(sqrt(i_pos)), HYPERPTR, ALLSENSES, 1);
            if (synset_sense) {
                for (int i = 0; i < synset_sense->wcount; ++i) {
                    synsets.push_back(synset_sense->words[i]);
                }
                wordnet_.free_syns(synset_sense);
            }
        }
    }
}
