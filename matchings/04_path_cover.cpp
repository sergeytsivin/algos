#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> g;
vector<int> mt, used;

bool dfs(int v) {
    if (used[v]) return false;
    used[v] = 1;
    for (int u : g[v]) {
        if (mt[u] == -1 || dfs(mt[u])) {
            mt[u] = v;
            return true;
        }
    }
    return false;
}

int main() {
    int n, m;
    if (!(cin >> n >> m)) return 0;
    g.resize(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        g[u - 1].push_back(v - 1);
    }

    used.resize(n);
    mt.assign(n, -1);
    int k = 0;
    for (int v = 0; v < n; ++v) {
        fill(used.begin(), used.end(), 0);
        if (dfs(v)) ++k;
    }

    vector<int> nxt(n, -1);
    for (int v = 0; v < n; ++v) {
        if (mt[v] != -1) nxt[mt[v]] = v;
    }
    cout << n - k << '\n';
    for (int s = 0; s < n; ++s) {
        // Правая копия без партнёра означает отсутствие предшественника.
        if (mt[s] != -1) continue;
        vector<int> path;
        for (int v = s; v != -1; v = nxt[v]) path.push_back(v);
        cout << path.size();
        for (int v : path) cout << ' ' << v + 1;
        cout << '\n';
    }
}
