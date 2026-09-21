#include <cstdint>
#include <iostream>
#include <vector>

using Mask = std::uint64_t;

namespace {

int n;
std::vector<Mask> adjacency;
int best_size = 0;
Mask best_set = 0;

int bit_count(Mask mask) {
    return __builtin_popcountll(mask);
}

int choose_vertex(Mask candidates) {
    int chosen = -1;
    int maximum_degree = -1;

    for (int vertex = 0; vertex < n; ++vertex) {
        const Mask bit = Mask{1} << vertex;
        if ((candidates & bit) == 0) {
            continue;
        }

        const int degree = bit_count(adjacency[vertex] & candidates);
        if (degree > maximum_degree) {
            maximum_degree = degree;
            chosen = vertex;
        }
    }

    return chosen;
}

void search(Mask candidates, Mask current_set, int current_size) {
    if (current_size + bit_count(candidates) <= best_size) {
        return;
    }

    if (candidates == 0) {
        best_size = current_size;
        best_set = current_set;
        return;
    }

    const int vertex = choose_vertex(candidates);
    const Mask vertex_bit = Mask{1} << vertex;

    // Берём вершину: удаляем из доступных её саму и всех её соседей.
    const Mask after_take = candidates & ~vertex_bit & ~adjacency[vertex];
    search(after_take, current_set | vertex_bit, current_size + 1);

    // Не берём вершину.
    const Mask after_skip = candidates & ~vertex_bit;
    search(after_skip, current_set, current_size);
}

}  // пространство имён

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int edge_count;
    if (!(std::cin >> n >> edge_count)) {
        return 0;
    }

    adjacency.assign(n, 0);
    for (int i = 0; i < edge_count; ++i) {
        int from, to;
        std::cin >> from >> to;
        --from;
        --to;

        if (from == to) {
            continue;
        }
        adjacency[from] |= Mask{1} << to;
        adjacency[to] |= Mask{1} << from;
    }

    const Mask all_vertices = n == 0 ? 0 : (Mask{1} << n) - 1;
    search(all_vertices, 0, 0);

    std::cout << best_size << '\n';
    bool first = true;
    for (int vertex = 0; vertex < n; ++vertex) {
        if ((best_set & (Mask{1} << vertex)) == 0) {
            continue;
        }
        if (!first) {
            std::cout << ' ';
        }
        std::cout << vertex + 1;
        first = false;
    }
    std::cout << '\n';
}
