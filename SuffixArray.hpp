// Suffix Array Class
// ------------------
// It provides an engine to build and manipulate a
// suffix array for a string or a set of strings.
//
// 
// Algorithms
// ----------
// [1] DC3 algorithm
//
//
// TODO
// ----
// [1] DC3 algorithm               [DONE]
// [2] Generalized suffix array    [DONE]
// [3] Linear-time LCP array       [DONE]
// [4] BWT array
// [5] Prefix-suffix overloap
// [6] General DC algorithm 
//     with modulo q
// 
//
// References
// ----------
// [1] Karkkainen J, et al. Linear work suffix array construction.
//     http://algo2.iti.kit.edu/documents/jacm05-revised.pdf
// [2] Kasai T, et al. Linear-time longest-common-prefix computation
//     in suffix arrays and its applications.
//     http://web.cs.iastate.edu/~cs548/references/linear_lcp.pdf
//
//
// Last Changed
// ------------
// July 20, 2015    Feng Zeng    Create it.
// July 21, 2015    Feng Zeng    Add generalized suffix array.
//                               Add linear-time lcp array construction.
// July 22, 2015    Feng Zeng    Change to support char32_t*.
//
//
//
//

#ifndef _SUFFIXARRAY_HPP
#define _SUFFIXARRAY_HPP

#include <string>
#include <map>
#include <vector>
#include <tuple>
using namespace std;

// <seq_id,pos>
typedef tuple<int,int> Suffix;
typedef int Modulo;
typedef string Triple;
typedef vector<int> Array;
typedef vector<string> ArrayS;
typedef vector<u32string> ArrayL;
typedef u32string StringL;
typedef char32_t  CharL;

class SuffixArray
{
public:
    SuffixArray(){}
    SuffixArray(string& a) {build(0,a);}
    SuffixArray(int id, string& a) {build(id,a);}
    ~SuffixArray(){}

private:
    void lsd(CharL* s, Array& from, Array& to, int d);
    int compare(CharL* a, CharL* b, int k);
    Array suffix_array_inverse(Array& sa, Array& idx);

    bool leq(CharL a1, CharL a2, CharL b1, CharL b2);
    bool leq(CharL a1, CharL a2, CharL a3, CharL b1, CharL b2, CharL b3);

    Array suffix_array_dc3(CharL* a,int n);

private:
    int   longest_common_prefix(string& a, int i, int j);
    void  build_suffix_array(int id, string& a);
    void  build_lcp_array(int id, string& a);
    void  build_rank_array();
    void  build_bwt_array(string& a);

public:
    void build(int id, string& a);

public:
    vector<Suffix>  sa;
    vector<int>     lcp;
    map<Suffix,int> rank;
    vector<char>    bwt;
};


class GeneralizedSuffixArray 
{
public:
    GeneralizedSuffixArray(){}
    GeneralizedSuffixArray(ArrayS& A){build(A);}
    ~GeneralizedSuffixArray(){}

private:
    tuple<int,int> lcp_compare(string& a, int i, string& b, int j, int k);
    void merge_with(ArrayS& A, SuffixArray& sa2);
    void build_rank_array();
    //void build_bwt_array(ArrayS& A);

public:
    void build(ArrayS& A);

public:
    vector<Suffix>  sa;
    vector<int>     lcp;
    map<Suffix,int> rank;
    vector<char>    bwt;
};

#endif
