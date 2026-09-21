#include <cstdint>
#include <iostream>
#include <vector>

using Mask = std::uint64_t;

namespace {

std::vector<Mask> adj;

int count(Mask mask) {
    return __builtin_popcountll(mask);
}

bool solve(Mask mask, int k) {
    if (k == 0) {
        return true;
    }
    if (count(mask) < k) {
        return false;
    }

    const int v = __builtin_ctzll(mask);
    const Mask bit = Mask{1} << v;

    // Берём вершину: удаляем из доступных её саму и всех её соседей.
    const Mask next = mask & ~bit & ~adj[v];
    return solve(next, k - 1) ||
           solve(mask & ~bit, k);  // Не берём вершину.
}

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    int k = 0;
    if (!(std::cin >> n >> m >> k)) {
        return 0;
    }

    adj.assign(n, 0);
    for (int i = 0; i < m; ++i) {
        int u = 0;
        int v = 0;
        std::cin >> u >> v;
        --u;
        --v;
        adj[u] |= Mask{1} << v;
        adj[v] |= Mask{1} << u;
    }

    const Mask all = (Mask{1} << n) - 1;
    std::cout << (solve(all, k) ? "YES\n" : "NO\n");
}
