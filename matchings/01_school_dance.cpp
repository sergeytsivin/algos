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
            // Меняем пару только после нахождения продолжения пути.
            mt[u] = v;
            return true;
        }
    }
    return false;
}

int main() {
    int n, m, k;
    if (!(cin >> n >> m >> k)) return 0;
    g.resize(n);
    for (int i = 0; i < k; ++i) {
        int a, b;
        cin >> a >> b;
        g[a - 1].push_back(b - 1);
    }

    used.resize(n);
    mt.assign(m, -1);
    int ans = 0;
    for (int v = 0; v < n; ++v) {
        fill(used.begin(), used.end(), 0);
        if (dfs(v)) ++ans;
    }

    cout << ans << '\n';
    for (int u = 0; u < m; ++u) {
        if (mt[u] != -1) {
            cout << mt[u] + 1 << ' ' << u + 1 << '\n';
        }
    }
}
