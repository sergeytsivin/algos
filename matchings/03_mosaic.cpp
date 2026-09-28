#include <algorithm>
#include <iostream>
#include <string>
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
    long long a, b;
    if (!(std::cin >> n >> m >> a >> b)) return 0;
    vector<std::string> board(n);
    int cells = 0;
    for (auto& row : board) {
        std::cin >> row;
        for (char c : row) cells += (c == '*');
    }
    if (a >= 2 * b) {
        std::cout << cells * b << '\n';
        return 0;
    }

    vector<vector<int>> id(n, vector<int>(m, -1));
    int na = 0, nb = 0;
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < m; ++y) {
            if (board[x][y] == '*') {
                if ((x + y) % 2 == 0) id[x][y] = na++;
                else id[x][y] = nb++;
            }
        }
    }

    vector<vector<int>> graphA(na);
    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < m; ++y) {
            if (board[x][y] != '*' || (x + y) % 2 != 0) continue;
            for (int d = 0; d < 4; ++d) {
                int nx = x + dx[d], ny = y + dy[d];
                if (nx >= 0 && nx < n && ny >= 0 && ny < m &&
                    board[nx][ny] == '*') {
                    graphA[id[x][y]].push_back(id[nx][ny]);
                }
            }
        }
    }

    vector<int> visitedA(na), pairFromA(nb, -1);
    int pairs = 0;
    for (int vA = 0; vA < na; ++vA) {
        std::fill(visitedA.begin(), visitedA.end(), 0);
        if (dfs(graphA, vA, visitedA, pairFromA)) ++pairs;
    }
    std::cout << pairs * a + (cells - 2 * pairs) * b << '\n';
}
