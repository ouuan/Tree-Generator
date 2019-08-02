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
                while (sta < str.size() && str[sta] != ',') ++sta;
                return sta;
            };

            function<unsigned int(const string&, unsigned int)> findLetter = [](const string& str, unsigned int sta)
            {
                while (sta < str.size() && !isalpha(str[sta])) ++sta;
                return sta;
            };

            function<void(const string &)> error = [](const string & func)
            {
                cerr << "Error: the number of parameters for " << func << " is not correct." << endl;
            };

            unsigned int pos = 0;

            while (pos < s.size())
            {
                if (pos + 1 >= s.size())
                {
                    cerr << "Error: can't recognize the tree type abbreviation, it's too short.\n";
                    return;
                }
                string type = s.substr(pos, 2);
                pos += 2;
                for_each(type.begin(), type.end(), [](char& x){x = tolower(x);});
                int nextLetter = findLetter(s, pos);
                vector<int> par;
                while (1)
                {
                    int nextComma = findComma(s, pos);
                    par.push_back(atoi(s.substr(pos, nextComma - pos).c_str()));
                    pos = nextComma + 1;
                    if (nextComma >= nextLetter)
                    {
                        pos = nextLetter;
                        break;
                    }
                }
                if (type == "nd")
                {
                    if (par.size() == 1) addNode(par[0]);
                    else error("addNode");
                }
                else if (type == "rd")
                {
                    if (par.size() == 2) random(par[0], par[1]);
                    else if (par.size() == 4) random(par[0], par[1], par[2], par[3]);
                    else error("random");
                }
                else if (type == "tl")
                {
                    if (par.size() == 3) tall(par[0], par[1], par[2]);
                    else error("tall");
                }
                else if (type == "ch")
                {
                    if (par.size() == 2) chain(par[0], par[1]);
                    else error("chain");
                }
                else if (type == "st")
                {
                    if (par.size() == 2) star(par[0], par[1]);
                    else error("star");
                }
                else if (type == "fl")
                {
                    if (par.size() == 2) flower(par[0], par[1]);
                    else error("flower");
                }
                else if (type == "md")
                {
                    if (par.size() == 3) maxDegree(par[0], par[1], par[2]);
                    else error("maxDegree");
                }
                else if (type == "cp")
                {
                    if (par.size() == 3) complete(par[0], par[1], par[2]);
                    else error("complete");
                }
                else if (type == "bi")
                {
                    if (par.size() == 2) binary(par[0], par[1]);
                    else error("binary");
                }
                else if (type == "cb")
                {
                    if (par.size() == 2) completeBinary(par[0], par[1]);
                    else error("completeBinary");
                }
                else if (type == "sw")
                {
                    if (par.size() == 2) silkworm(par[0], par[1]);
                    else error("silkworm");
                }
                else if (type == "al")
                {
                    if (par.size() == 3) addLeaves(par[0], par[1], par[2]);
                    else error("addLeaves");
                }
                else cerr << "Error: can't recognize the tree type abbreviation " << type << ".\n";
            }
        }
        int size() const { return id.size(); }
        void addNode(int pa)
        {
            assert(pa >= 0);
            assert(pa < size());
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
            vector<int> prufer, cnt;
            vector<vector<int> > g;
            g.resize(n);
            cnt.resize(n, 0);
            for (int i = 0; i < n - 2; ++i)
            {
                int x = randint(0, n - 1);
                prufer.push_back(x);
                ++cnt[x];
            }
            priority_queue<int> q;
            for (int i = 0; i < n; ++i) if (!cnt[i]) q.push(i);
            for (auto v : prufer)
            {
                int u = q.top();
                g[u].push_back(v);
                g[v].push_back(u);
                q.pop();
                if (--cnt[v] == 0) q.push(v);
            }
            int x = q.top();
            q.pop();
            int y = q.top();
            g[x].push_back(y);
            g[y].push_back(x);

            queue<int> bfs;

            bfs.push(0);
            int _id = sz;
            addNode(pa);

            while (!bfs.empty())
            {
                int u = bfs.front();
                cnt[u] = 1;
                bfs.pop();
                for (auto v : g[u])
                {
                    if (cnt[v] == 0)
                    {
                        addNode(_id);
                        bfs.push(v);
                    }
                }
                ++_id;
            }
        }
        void random(int n, int low, int high, int pa)
        {
            int sz = size();
            assert(n > 0);
            assert(low >= 0);
            assert(high < 100);
            assert(high >= low);
            assert(pa >= 0);
            assert(pa < sz);
            addNode(pa);
            for (int i = 1; i < n; ++i) addNode(randint(i * low / 100, i * high / 100) + sz);
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
            assert(n > 0);
            assert(pa >= 0);
            assert(pa < size());
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
            assert(n > 0);
            assert(pa >= 0);
            assert(pa < size());
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
            assert(n > 0);
            assert(pa >= 0);
            assert(pa < size());
            maxDegree(n, 3, pa);
        }
        void completeBinary(int n, int pa)
        {
            assert(n > 0);
            assert(pa >= 0);
            assert(pa < size());
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
