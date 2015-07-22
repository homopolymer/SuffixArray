#include "SuffixArray.hpp"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main(int argc, char** argv)
{
    vector<string> A;
    for (int i=1; i<argc; i++)
        A.push_back(argv[i]);
    GeneralizedSuffixArray gsa(A);
    for (int i=0; i<gsa.sa.size(); i++)
        cout << get<0>(gsa.sa[i]) << " "
             << get<1>(gsa.sa[i]) << " "
             << gsa.lcp[i] << " " 
             << gsa.bwt[i] << endl;
}
