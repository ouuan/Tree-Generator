/*

MIT License

Copyright (c) 2019 ouuan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef TREE_GENERATOR_BY_OUUAN_
#define TREE_GENERATOR_BY_OUUAN_ 1

#include <bits/stdc++.h>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/assoc_container.hpp>

namespace tree_generator_by_ouuan
{
    using namespace std;

    typedef pair<int, int> pii; 

    mt19937 defaultRNG(chrono::steady_clock::now().time_since_epoch().count());

    int defaultRandInt(int l, int r)
    {
        int out = defaultRNG() % (r - l + 1) + l;
        return out >= l ? out : out + r - l + 1; 
    }

    int (*randint)(int, int) = defaultRandInt;

    void defaultOutputEdge(ostream& os, int u, int pa)
    {
        if (randint(0, 1) == 0) os << u + 1 << ' ' << pa + 1 << endl;
        else os << pa + 1 << ' ' << u + 1 << endl;
    }

    void (*outputEdge)(ostream&, int, int) = defaultOutputEdge;

    class Tree
    {
    private:
        vector<int> p, id, eid;

    public:
        Tree()
        {
            p.push_back(-1);
            id.push_back(0);
        }
        Tree(int n)
        {
            assert(n > 0);
            p.push_back(-1);
            id.push_back(0);
            random(n - 1, 0);
        }
        Tree(const string& s)
        {
            p.push_back(-1);
            id.push_back(0);

            function<unsigned int(const string&, unsigned int)> findComma = [](const string& str, unsigned int sta)
            {
                return str.find(',', sta);
            };

            function<unsigned int(const string&, unsigned int)> findLetter = [](const string& str, unsigned int sta)
            {
                while (sta < str.size() && !isalpha(str[sta])) ++sta;
                return sta;
            };

            unsigned int pos = 0;

            while (pos < s.size())
            {
                if (pos + 1 >= s.size())
                {
                    cerr << "Error: can't recognize tree type abbreviation, it's too short.\n";
                    return;
                }
                string type = s.substr(pos, 2);
                pos += 2;
                for_each(type.begin(), type.end(), [](char& x){x = tolower(x);});
                if (type == "nd")
                {
                    int nxt = findLetter(s, pos);
                    int x = atoi(s.substr(pos, nxt - pos).c_str());
                    pos = nxt;
                    addNode(x);
                }
                else if (type == "rd" || type == "ch" || type == "st" || type == "fl"
                         || type == "bi" || type == "cb" || type == "sw")
                {
                    int nxt = findComma(s, pos);
                    int x = atoi(s.substr(pos, nxt - pos).c_str());
                    pos = nxt + 1;
                    nxt = findLetter(s, pos);
                    int y = atoi(s.substr(pos, nxt - pos).c_str());
                    pos = nxt;
                    if (type == "rd") random(x, y);
                    else if (type == "ch") chain(x, y);
                    else if (type == "st") star(x, y);
                    else if (type == "fl") flower(x, y);
                    else if (type == "bi") binary(x, y);
                    else if (type == "cb") completeBinary(x, y);
                    else if (type == "sw") silkworm(x, y);
                    else assert(false);
                }
                else if (type == "tl" || type == "md" || type == "cp" || type == "al")
                {
                    int nxt = findComma(s, pos);
                    int x = atoi(s.substr(pos, nxt - pos).c_str());
                    pos = nxt + 1;
                    nxt = findComma(s, pos);
                    int y = atoi(s.substr(pos, nxt - pos).c_str());
                    pos = nxt + 1;
                    nxt = findLetter(s, pos);
                    int z = atoi(s.substr(pos, nxt - pos).c_str());
                    pos = nxt;
                    if (type == "tl") tall(x, y, z);
                    else if (type == "md") maxDegree(x, y, z);
                    else if (type == "cp") complete(x, y, z);
                    else if (type == "al") addLeaves(x, y, z);
                    else assert(false);
                }
                else cerr << "Error: can't recognize tree type abbreviation " << type << ".\n";
            }
        }
        int size() const { return id.size(); }
        void addNode(int pa)
        {
            id.push_back(id.size());
            p.push_back(pa);
            eid.push_back(id.size() - 1);
        }
        void random(int n, int pa)
        {
            int sz = size();
            assert(n > 0);
            assert(pa >= 0);
            assert(pa < sz);
            addNode(pa);
            for (int i = sz + 1; i < sz + n; ++i) addNode(randint(sz, i - 1));
        }
        void tall(int n, int k, int pa)
        {
            int sz = size();
            assert(n > 0);
            assert(k > 0);
            assert(pa >= 0);
            assert(pa < sz);
            addNode(pa);
            for (int i = sz + 1; i < sz + n; ++i) addNode(randint(max(sz, i - k), i - 1));
        }
        void chain(int n, int pa)
        {
            tall(n, 1, pa);
        }
        void star(int n, int pa)
        {
            int sz = size();
            assert(n > 0);
            assert(pa >= 0);
            assert(pa < sz);
            addNode(pa);
            for (int i = sz + 1; i < sz + n; ++i) addNode(sz);
        }
        void flower(int n, int pa)
        {
            star(n, pa);
        }
        void maxDegree(int n, int k, int pa)
        {
            int sz = size();
            assert(n > 0);
            assert(k >= 2);
            assert(pa >= 0);
            assert(pa < sz);
            addNode(pa);
            __gnu_pbds::tree<pii, __gnu_pbds::null_type, less<pii>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> remain;
            remain.insert(pii(sz, k - 1));
            for (int i = sz + 1; i < sz + n; ++i)
            {
                auto it = remain.find_by_order(randint(0, remain.size() - 1));
                int u = it->first;
                int t = it->second;
                remain.erase(it);
                if (t > 1) remain.insert(pii(u, t - 1));
                addNode(u);
                remain.insert(pii(i, k - 1));
            }
        }
        void complete(int n, int k, int pa)
        {
            int sz = size();
            assert(n > 0);
            assert(k >= 2);
            assert(pa >= 0);
            assert(pa < sz);
            addNode(pa);
            for (int i = sz + 1; i < sz + n; ++i) addNode(sz + ceil(1.0 * (i - sz) / (k - 1) - 1e-9) - 1);
        }
        void binary(int n, int pa)
        {
            maxDegree(n, 3, pa);
        }
        void completeBinary(int n, int pa)
        {
            complete(n, 3, pa);
        }
        void silkworm(int n, int pa)
        {
            int sz = size();
            assert(n > 0);
            assert(pa >= 0);
            assert(pa < sz);
            int chain_len = (n + 1) / 2;
            chain(chain_len, pa);
            for (int i = sz; i + chain_len < sz + n; ++i) addNode(i);
        }
        void addLeaves(int n, int l, int r)
        {
            assert(n > 0);
            assert(l >= 0);
            assert(r < size());
            assert(l <= r);
            for (int i = 1; i <= n; ++i) addNode(randint(l, r));
        }
        void shuffleNodes(int from = 0)
        {
            for (int i = 0; i < from; ++i) id[i] = i;
            for (unsigned int i = from; i < id.size(); ++i)
            {
                id[i] = i;
                swap(id[i], id[randint(from, i)]);
            }
        }
        void shuffleEdges()
        {
            for (unsigned int i = 0; i < eid.size(); ++i)
            {
                eid[i] = i + 1;
                swap(eid[i], eid[randint(0, i)]);
            }
        }
        void printEdge(int edgeID, ostream& os = cout) const
        {
            outputEdge(os, id[eid[edgeID]], id[p[eid[edgeID]]]);
        }
        friend ostream& operator<<(ostream& os, const Tree& t)
        {
            for (unsigned int i = 0; i < t.eid.size(); ++i) t.printEdge(i, os);
            return os;
        }
    };
}

#endif
