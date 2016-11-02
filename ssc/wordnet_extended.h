//=======================================================================
// Copyright 2013
// Authors: Xin Liu tonyxliu@hotmail.co.uk
//
// Distributed under GPL v2
//=======================================================================
#ifndef __WORDNET_EXTENDED_H__
#define __WORDNET_EXTENDED_H__

#include "safe_wordnet.h"

class WordnetExtended
{
public:
    typedef std::shared_ptr<Synset> SynsetPtrS;

    //graph types
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, SynsetPtrS,
        boost::property < boost::edge_weight_t, int > > UndirectedGraph;
    typedef boost::graph_traits < UndirectedGraph >::vertex_descriptor vertex_t;
    typedef boost::graph_traits < UndirectedGraph >::edge_descriptor edge_t;

protected:
    SafeWordNet wordnet_;

    // frequency dict path
    const int _freq_dict_res_id;

    bool _freq_dict_loaded;
    long _freq_total;
    std::map<std::string, int> _freq_dict;

    void mark_depth(const vertex_t v, const UndirectedGraph& g, int depth, std::vector<vertex_t>& passed_vertices);
    void load_freq_dict_from_res();

private:
    WordnetExtended(const WordnetExtended&);
    WordnetExtended(WordnetExtended&&);
    WordnetExtended& operator=(const WordnetExtended&);
    WordnetExtended& operator=(WordnetExtended&&);

public:
    ///// Constructor 
    WordnetExtended(int freq_dict_res_id)
        : _freq_dict_res_id(freq_dict_res_id)
        , _freq_dict_loaded(false) {}

    ///
    /// c++ notes: use pointers if you want to do arithmetic with them or if you ever have to pass a nullptr-pointer.
    /// use references otherwise.
    void build_synset_adjacency_list(
        const std::vector<std::string> & words,
        UndirectedGraph &adj_list);

    /// return shortest path
    ///		v1, v2 is the vertex contains word that being used for calculation
    int compute_distance(
        const UndirectedGraph & adj_list,
        const std::string & w1,
        const std::string & w2,
        vertex_t & v1,
        vertex_t & v2);

    /// return whether a synset already exist in graph
    vertex_t find_vertex(
        const UndirectedGraph & adj_list,
        long hereiam, char *pos);

    void normalization(std::vector<std::string> &);

    ///Li et al.(2006) 4.2.2
    float get_freq_weight(const std::string &);
};

#endif//__WORDNET_EXTENDED_H__