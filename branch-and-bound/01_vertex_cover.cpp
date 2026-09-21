#include <cstdint>
#include <iostream>
#include <vector>

using Mask = std::uint64_t;

namespace {

std::vector<Mask> adj;

int first(Mask mask) {
    return __builtin_ctzll(mask);
}

bool solve(Mask mask, int k) {
    int u = -1;
    int v = -1;

    // Ищем любое ребро, у которого оба конца ещё находятся в mask.
    for (u = 0; u < static_cast<int>(adj.size()); ++u) {
        if ((mask & (Mask{1} << u)) == 0) {
            continue;
        }
        const Mask neighbors = adj[u] & mask;
        if (neighbors != 0) {
            v = first(neighbors);
            break;
        }
    }

    if (v == -1) {
        return true;
    }
    if (k == 0) {
        return false;
    }

    // В покрытие должен входить хотя бы один конец ребра (u, v).
    return solve(mask & ~(Mask{1} << u), k - 1) ||
           solve(mask & ~(Mask{1} << v), k - 1);
}

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    int m;
    int k;
    if (!(std::cin >> n >> m >> k)) {
        return 0;
    }

    adj.assign(n, 0);
    for (int i = 0; i < m; ++i) {
        int u;
        int v;
        std::cin >> u >> v;
        --u;
        --v;

        // Кратные рёбра лишь повторно устанавливают те же биты.
        adj[u] |= Mask{1} << v;
        adj[v] |= Mask{1} << u;
    }

    const Mask all = (Mask{1} << n) - 1;
    std::cout << (solve(all, k) ? "YES\n" : "NO\n");
}
