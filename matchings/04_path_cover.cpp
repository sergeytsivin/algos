#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;

bool dfs(vector<vector<int>>& graphA, int vA,
         vector<int>& visitedA, vector<int>& pairFromA) {
    if (visitedA[vA]) return false;
    visitedA[vA] = 1;
    for (int vB : graphA[vA]) {
        if (pairFromA[vB] == -1 ||
            dfs(graphA, pairFromA[vB], visitedA, pairFromA)) {
            pairFromA[vB] = vA;
            return true;
        }
    }
    return false;
}

int main() {
    int n, m;
    if (!(std::cin >> n >> m)) return 0;
    vector<vector<int>> graphA(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        graphA[u - 1].push_back(v - 1);
    }

    vector<int> visitedA(n), pairFromA(n, -1);
    int pairs = 0;
    for (int vA = 0; vA < n; ++vA) {
        std::fill(visitedA.begin(), visitedA.end(), 0);
        if (dfs(graphA, vA, visitedA, pairFromA)) ++pairs;
    }

    vector<int> next(n, -1);
    for (int v = 0; v < n; ++v) {
        if (pairFromA[v] != -1) next[pairFromA[v]] = v;
    }
    std::cout << n - pairs << '\n';
    for (int start = 0; start < n; ++start) {
        // Правая копия без партнёра означает отсутствие предшественника.
        if (pairFromA[start] != -1) continue;
        vector<int> path;
        for (int v = start; v != -1; v = next[v]) path.push_back(v);
        std::cout << path.size();
        for (int v : path) std::cout << ' ' << v + 1;
        std::cout << '\n';
    }
}
