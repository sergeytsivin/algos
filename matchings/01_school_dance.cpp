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
            // Меняем пару только после нахождения продолжения пути.
            pairFromA[vB] = vA;
            return true;
        }
    }
    return false;
}

int main() {
    int n, m, k;
    if (!(std::cin >> n >> m >> k)) return 0;
    vector<vector<int>> graphA(n);
    for (int i = 0; i < k; ++i) {
        int a, b;
        std::cin >> a >> b;
        graphA[a - 1].push_back(b - 1);
    }

    vector<int> visitedA(n), pairFromA(m, -1);
    int ans = 0;
    for (int vA = 0; vA < n; ++vA) {
        std::fill(visitedA.begin(), visitedA.end(), 0);
        if (dfs(graphA, vA, visitedA, pairFromA)) ++ans;
    }

    std::cout << ans << '\n';
    for (int vB = 0; vB < m; ++vB) {
        if (pairFromA[vB] != -1) {
            std::cout << pairFromA[vB] + 1 << ' ' << vB + 1 << '\n';
        }
    }
}
