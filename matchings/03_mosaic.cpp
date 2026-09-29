#include <algorithm>
#include <iostream>
#include <string>
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
    long long a, b;
    if (!(cin >> n >> m >> a >> b)) return 0;
    vector<string> s(n);
    int cnt = 0;
    for (auto& row : s) {
        cin >> row;
        for (char c : row) cnt += (c == '*');
    }
    if (a >= 2 * b) {
        cout << cnt * b << '\n';
        return 0;
    }

    vector<vector<int>> id(n, vector<int>(m, -1));
    int na = 0, nb = 0;
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < m; ++y) {
            if (s[x][y] == '*') {
                if ((x + y) % 2 == 0) id[x][y] = na++;
                else id[x][y] = nb++;
            }
        }
    }

    g.resize(na);
    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < m; ++y) {
            if (s[x][y] != '*' || (x + y) % 2 != 0) continue;
            for (int d = 0; d < 4; ++d) {
                int nx = x + dx[d], ny = y + dy[d];
                if (nx >= 0 && nx < n && ny >= 0 && ny < m &&
                    s[nx][ny] == '*') {
                    g[id[x][y]].push_back(id[nx][ny]);
                }
            }
        }
    }

    used.resize(na);
    mt.assign(nb, -1);
    int k = 0;
    for (int v = 0; v < na; ++v) {
        fill(used.begin(), used.end(), 0);
        if (dfs(v)) ++k;
    }
    cout << k * a + (cnt - 2 * k) * b << '\n';
}
