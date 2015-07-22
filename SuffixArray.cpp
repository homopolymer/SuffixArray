#include "SuffixArray.hpp"
#include <algorithm>
#include <iostream>
#include <cstring>
using namespace std;

void SuffixArray::lsd(CharL* s, vector<int>& from, vector<int>& to, int d)
{
    int Z=2048;
    vector<int> count(Z,0);

    // counting
    for (int i=0; i<from.size(); i++)
    {
        count[s[from[i]+d]+1]++;
    }   

    // accumulative sum
    for (int i=1; i<Z; i++)
    {
        count[i] += count[i-1];
    }

    // sort
    to.resize(from.size());
    for (int i=0; i<from.size(); i++)
    {
        to[count[s[from[i]+d]]++] = from[i];
    }
}

int SuffixArray::compare(CharL* a, CharL* b, int k)
{
    for (int i=0; i<k; i++)
    {
        if (a[i] < b[i]) return -1;
        else if (a[i] > b[i]) return 1;
    }
    
    return 0;
}


bool SuffixArray::leq(CharL a1, CharL a2, CharL b1, CharL b2)
{
    return (a1<b1 || a1==b1 && a2<=b2);
}

bool SuffixArray::leq(CharL a1, CharL a2, CharL a3, CharL b1, CharL b2, CharL b3)
{
    return (a1<b1 || a1==b1 && leq(a2,a3,b2,b3));
}

vector<int> SuffixArray::suffix_array_inverse(vector<int>& sa, vector<int>& idx)
{
    vector<int> rank(sa.size(),0);

    for (int i=0; i<sa.size(); i++)
    {
        rank[idx[sa[i]]] = i+1;
    }

    return rank;
}

vector<int> SuffixArray::suffix_array_dc3(CharL* a,int n)
{
    CharL b = U'$'; 
    int i,j,c,name=0;
    vector<int> SA, RANK;
    vector<int> R0,P0,SA0;
    vector<int> R12,P12,SA12,RANK12,I;
    vector<CharL> T;
    CharL *t = new CharL[n+2], *p = new CharL[3], *q = new CharL[3];
    //StringL t = a+U"$$",p,q;
    CharL u1,u2,u3,v1,v2,v3;

    // copy a to t
    t = new CharL[n+2];
    memcpy(t,a,n*sizeof(CharL));
    t[n]   = U'$';
    t[n+1] = U'$';

    I.resize(n);
    // get (i mod 3==1)
    for (i=1,j=0; i<n; i+=3,j++)
    {
        R12.push_back(i);
        P12.push_back(i);
        I[i] = j;
    }
    // get (i mod 3==2)
    for (i=2; i<n; i+=3,j++)
    {
        R12.push_back(i);
        P12.push_back(i);
        I[i] = j;
    }

    // sort (i mod 3==1) and (i mod 3==2)
    lsd(t, P12, SA12, 2);
    lsd(t, SA12, P12, 1);
    lsd(t, P12, SA12, 0);

    // assign lexicographical name
    RANK12 = suffix_array_inverse(SA12,I);
    T.resize(SA12.size());
    T[0] = b+(char)RANK12[I[SA12[0]]];
    name++;
    //p = t.substr(SA12[0],3);
    memcpy(p,t+SA12[0],3*sizeof(CharL));
    for (i=1; i<SA12.size(); i++)
    {
        //q = t.substr(SA12[i],3);
        memcpy(q,t+SA12[i],3*sizeof(CharL));
        c = compare(p,q,3);

        if (c==0) T[i] = T[i-1];
        else
        {
            T[i] = b+RANK12[I[SA12[i]]];
            name++;
            memcpy(p,q,3*sizeof(CharL));
        }
    }

    // recursion
    if (name < SA12.size())
    {
        CharL* x = new CharL[R12.size()+1];
        for (j=0; j<R12.size(); j++)
        {
            i = RANK12[I[R12[j]]]-1;
            x[j] = T[i];
        }
        x[R12.size()] = U'$';
        vector<int> SA3 = suffix_array_dc3(x,R12.size()+1);
        for (i=1; i<SA3.size(); i++)
        {
            SA12[i-1] = R12[SA3[i]];
        }
        RANK12 = suffix_array_inverse(SA12,I);
   
        delete x;
    }

    // get (i mod 3==0)
    for (i=0; i<n; i+=3)
    {
        R0.push_back(i);
    }
    // modify t
    for (j=0; j<R12.size(); j++)
    {
        t[R12[j]] = RANK12[j]+b;
    }
    // sort (i mod 3==0)
    lsd(t,R0,P0,1);
    lsd(t,P0,SA0,0);

    // merge SA0 and SA12
    i = 0;
    j = 0;
    while(i<SA0.size() && j<SA12.size())
    {
        if (SA12[j]%3==1)
        {
            u1 = a[SA0[i]];
            if (SA0[i]+1<n) u2 = RANK12[I[SA0[i]+1]];
            else u2 = '$';
            v1 = a[SA12[j]];
            if (SA12[j]+1<n) v2 = RANK12[I[SA12[j]+1]];
            else v2 = '$';
            if(leq(u1,u2,v1,v2))
            {
                SA.push_back(SA0[i++]);
            }else
            {
                SA.push_back(SA12[j++]);
            }
        }else if (SA12[j]%3==2)
        {
            u1 = a[SA0[i]];
            if (SA0[i]+1<n) u2 = a[SA0[i]+1];
            else u2 = '$';
            if (SA0[i]+2<n) u3 = RANK12[I[SA0[i]+2]];
            else u3 = 0;
            v1 = a[SA12[j]];
            if (SA12[j]+1<n) v2 = a[SA12[j]+1];
            else v2 = '$';
            if (SA12[j]+2<n) v3 = RANK12[I[SA12[j]+2]];
            else v3 = 0;
            if (leq(u1,u2,u3,v1,v2,v3))
            {
                SA.push_back(SA0[i++]);
            }else
            {
                SA.push_back(SA12[j++]);
            }
        }
    }
    while(i<SA0.size()) SA.push_back(SA0[i++]);
    while(j<SA12.size()) SA.push_back(SA12[j++]);
        
    delete t;
    delete p;
    delete q;

    return SA;
}


void SuffixArray::build_suffix_array(int id, string& a)
{
    CharL* t = new CharL[a.length()];
    for (int i=0; i<a.length(); i++)
    {
        t[i] = (CharL)a[i];
    }

    Array _sa = suffix_array_dc3(t,a.length());
    for (auto it=_sa.begin(); it!=_sa.end(); ++it)
    {
        sa.push_back(Suffix(id,*it));
    }

    delete t;
}

void SuffixArray::build_rank_array()
{
    for (int i=0; i<sa.size(); i++)
    {
        rank[sa[i]] = i;
    }
}

void SuffixArray::build_bwt_array(string& a)
{
    int i,j;
    for (i=0; i<a.length(); i++)
    {
        j = get<1>(sa[i]);
        if (j==0) bwt.push_back('$');
        else bwt.push_back(a[j-1]);
    }
}


int SuffixArray::longest_common_prefix(string& a, int i, int j)
{
    int l=0;
    while (i+l<a.length() && j+l<a.length())
    {
        if (a[i+l]==a[j+l]) l++;
        else break;
    }
    return l;
}

void SuffixArray::build_lcp_array(int id, string& a)
{
    lcp.resize(a.size());
    lcp[0] = 0;
    int i,j,p,h = 0;
    for (i=0; i<a.length()-1; i++)
    {
        if (h==0)
        {
            p = rank[Suffix(id,i)];
            j = get<1>(sa[p-1]);
            h = longest_common_prefix(a,i,j);
            lcp[p] = h;
        }else
        {
            p = rank[Suffix(id,i)];
            j = get<1>(sa[p-1]);
            h += longest_common_prefix(a,i+h,j+h);
            lcp[p] = h;
        }
        if (h>0) h=h-1;
    }
}

void SuffixArray::build(int id, string& a)
{
    // suffix array
    build_suffix_array(id,a);

    // rank array
    build_rank_array();
  
    // lcp array
    build_lcp_array(id,a);

    // bwt array
    build_bwt_array(a);
}

// generalized suffix array
void GeneralizedSuffixArray::build(ArrayS& A)
{
    for (int i=0; i<A.size(); i++)
    {
        SuffixArray sa2(i,A[i]);
        if (i==0)
        {
            copy(sa2.sa.begin(), sa2.sa.end(), back_inserter(sa));
            copy(sa2.lcp.begin(), sa2.lcp.end(), back_inserter(lcp));
            copy(sa2.bwt.begin(), sa2.bwt.end(), back_inserter(bwt));
        }else
        {
            merge_with(A, sa2);
        }
    }

    // build rank array
    build_rank_array();
}

tuple<int,int> GeneralizedSuffixArray::lcp_compare(string& a, int i, string& b, int j, int k)
{
    while (i+k<a.length() && j+k<b.length())
    {
        if (a[i+k]==b[j+k]) k++;
        else break;
    }
    
    if (i+k>=a.length()) return make_tuple(-1,k);
    else if (j+k>=b.length()) return make_tuple(1,k);
    else
    {
        if (a[i+k]<b[j+k]) return make_tuple(-1,k);
        else if (a[i+k]>b[j+k]) return make_tuple(1,k);
    }
}

void GeneralizedSuffixArray::merge_with(ArrayS& A, SuffixArray& sa2)
{
    int i=1,j=0,k,u,v,g,h;
    tuple<int,int> r;
    vector<Suffix> sa3;
    vector<int>    lcp3;
    vector<CharL>  bwt3;
    sa3.push_back(sa[0]);
    lcp3.push_back(lcp[0]);
    bwt3.push_back(bwt[0]);

    while (i<sa.size() && j<sa2.sa.size())
    {
        if (lcp[i]>sa2.lcp[j])
        {
            sa3.push_back(sa[i]);
            lcp3.push_back(lcp[i]);
            bwt3.push_back(bwt[i]);
            i++;
        }else if (lcp[i]<sa2.lcp[j])
        {
            sa3.push_back(sa2.sa[j]);
            lcp3.push_back(sa2.lcp[j]);
            bwt3.push_back(sa2.bwt[j]);
            j++;
        }else
        {
            k = lcp[i];
            u = get<0>(sa[i]);
            g = get<1>(sa[i]);
            v = get<0>(sa2.sa[j]);
            h = get<1>(sa2.sa[j]);
            r = lcp_compare(A[u],g,A[v],h,k);
            if (get<0>(r)<0)
            {
                sa3.push_back(sa[i]);
                lcp3.push_back(lcp[i]);
                bwt3.push_back(bwt[i]);
                sa2.lcp[j] = get<1>(r);
                i++;
            }else
            {
                sa3.push_back(sa2.sa[j]);
                lcp3.push_back(sa2.lcp[j]);
                bwt3.push_back(sa2.bwt[j]);
                lcp[i] = get<1>(r);
                j++;
            }
        }
    }

    while(i<sa.size())
    {
        sa3.push_back(sa[i]);
        lcp3.push_back(lcp[i]);
        bwt3.push_back(bwt[i]);
        i++;
    }

    while(j<sa2.sa.size())
    {
        sa3.push_back(sa2.sa[j]);
        lcp3.push_back(sa2.lcp[j]);
        bwt3.push_back(sa2.bwt[j]);
        j++;
    }

    sa.resize(sa3.size());
    lcp.resize(lcp3.size());
    bwt.resize(bwt3.size());
    copy(sa3.begin(),sa3.end(),sa.begin());
    copy(lcp3.begin(),lcp3.end(),lcp.begin());
    copy(bwt3.begin(),bwt3.end(),bwt.begin());
}

void GeneralizedSuffixArray::build_rank_array()
{
    for (int i=0; i<sa.size(); i++)
    {
        rank[sa[i]] = i;
    }
}
