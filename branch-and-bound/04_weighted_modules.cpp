#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

using Mask = std::uint64_t;

namespace {

int vertex_count;
int required_count;
std::vector<long long> weight;
std::vector<Mask> adjacent;

long long best_weight = std::numeric_limits<long long>::lowest();
Mask best_set = 0;

int bit_count(Mask mask) {
    return __builtin_popcountll(mask);
}

int first_vertex(Mask mask) {
    return __builtin_ctzll(mask);
}

long long optimistic_weight(Mask candidates, int need) {
    std::vector<long long> available_weights;
    available_weights.reserve(bit_count(candidates));

    while (candidates != 0) {
        const int vertex = first_vertex(candidates);
        candidates &= candidates - 1;
        available_weights.push_back(weight[vertex]);
    }

    std::sort(available_weights.begin(), available_weights.end(), std::greater<>());

    long long result = 0;
    for (int index = 0; index < need; ++index) {
        result += available_weights[index];
    }
    return result;
}

int choose_vertex(Mask candidates) {
    int chosen = -1;
    int largest_degree = -1;
    long long largest_weight = std::numeric_limits<long long>::lowest();

    Mask remaining = candidates;
    while (remaining != 0) {
        const int vertex = first_vertex(remaining);
        remaining &= remaining - 1;
        const int degree = bit_count(adjacent[vertex] & candidates);
        if (degree > largest_degree ||
            (degree == largest_degree && weight[vertex] > largest_weight)) {
            chosen = vertex;
            largest_degree = degree;
            largest_weight = weight[vertex];
        }
    }
    return chosen;
}

void search(Mask candidates, int need, long long current_weight, Mask selected) {
    if (need == 0) {
        if (current_weight > best_weight) {
            best_weight = current_weight;
            best_set = selected;
        }
        return;
    }

    if (bit_count(candidates) < need) {
        return;
    }

    if (current_weight + optimistic_weight(candidates, need) <= best_weight) {
        return;
    }

    const int vertex = choose_vertex(candidates);
    const Mask vertex_bit = Mask{1} << vertex;

    const Mask compatible = candidates & ~adjacent[vertex] & ~vertex_bit;
    search(compatible, need - 1, current_weight + weight[vertex], selected | vertex_bit);

    search(candidates & ~vertex_bit, need, current_weight, selected);
}

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int edge_count;
    if (!(std::cin >> vertex_count >> edge_count >> required_count)) {
        return 0;
    }

    weight.resize(vertex_count);
    for (long long& value : weight) {
        std::cin >> value;
    }

    adjacent.assign(vertex_count, 0);
    for (int edge = 0; edge < edge_count; ++edge) {
        int from;
        int to;
        std::cin >> from >> to;
        --from;
        --to;
        adjacent[from] |= Mask{1} << to;
        adjacent[to] |= Mask{1} << from;
    }

    const Mask all_vertices = (Mask{1} << vertex_count) - 1;
    search(all_vertices, required_count, 0, 0);

    if (best_weight == std::numeric_limits<long long>::lowest()) {
        std::cout << "IMPOSSIBLE\n";
        return 0;
    }

    std::cout << best_weight << '\n';
    bool first = true;
    for (int vertex = 0; vertex < vertex_count; ++vertex) {
        if ((best_set & (Mask{1} << vertex)) != 0) {
            if (!first) {
                std::cout << ' ';
            }
            std::cout << vertex + 1;
            first = false;
        }
    }
    std::cout << '\n';
}
