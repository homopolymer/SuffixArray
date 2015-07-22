#include "SuffixArray.hpp"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char** argv)
{
    string a=argv[1];
    SuffixArray sa;
    sa.build(0,a);
    for (int i=0; i<a.length(); ++i)
        cout << get<1>(sa.sa[i]) << " " << sa.lcp[i] << " " << sa.bwt[i] << endl;

    return 0;
}
