#include <cstdint>
#include <iostream>
#include <vector>

using Mask = std::uint64_t;

namespace {

std::vector<Mask> adjacency;

int popcount(Mask mask) {
    return __builtin_popcountll(mask);
}

bool has_independent_set(Mask candidates, int need) {
    if (need == 0) {
        return true;
    }
    if (popcount(candidates) < need) {
        return false;
    }

    int selected_vertex = -1;
    int maximum_degree = -1;
    bool has_edges = false;

    Mask remaining = candidates;
    while (remaining != 0) {
        const int vertex = __builtin_ctzll(remaining);
        const int degree = popcount(adjacency[vertex] & candidates);
        if (degree > maximum_degree) {
            maximum_degree = degree;
            selected_vertex = vertex;
        }
        has_edges |= degree != 0;
        remaining &= remaining - 1;
    }

    // Если в порождённом графе нет рёбер, можно выбрать любых кандидатов.
    if (!has_edges) {
        return true;
    }

    const Mask vertex_bit = Mask{1} << selected_vertex;

    // Берём вершину: удаляем из доступных её саму и всех её соседей.
    const Mask after_taking = candidates & ~vertex_bit & ~adjacency[selected_vertex];
    if (has_independent_set(after_taking, need - 1)) {
        return true;
    }

    // Не берём вершину: удаляем из доступных только её саму.
    return has_independent_set(candidates & ~vertex_bit, need);
}

}  // пространство имён

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    int k = 0;
    if (!(std::cin >> n >> m >> k)) {
        return 0;
    }

    adjacency.assign(n, 0);
    for (int i = 0; i < m; ++i) {
        int u = 0;
        int v = 0;
        std::cin >> u >> v;
        --u;
        --v;
        adjacency[u] |= Mask{1} << v;
        adjacency[v] |= Mask{1} << u;
    }

    if (k < 0 || k > n) {
        std::cout << "NO\n";
        return 0;
    }

    const Mask all_vertices = n == 0 ? 0 : (Mask{1} << n) - 1;
    std::cout << (has_independent_set(all_vertices, k) ? "YES\n" : "NO\n");
}
