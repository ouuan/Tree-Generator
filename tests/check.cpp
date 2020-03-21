#include <iostream>
#include <vector>
#include <cassert>
#include <functional>

using namespace std;

int main()
{
    int n;

    cin >> n;

    vector<int> f(n + 1);
    for (int i = 1; i <= n; ++i) f[i] = i;

    function<int(int)> find = [&](int x) { return x == f[x] ? x : f[x] = find(f[x]); };

    for (int i = 1; i < n; ++i)
    {
        int u, v;
        cin >> u >> v;
        assert(find(u) != find(v));
        f[find(v)] = find(u);
    }

    return 0;
}