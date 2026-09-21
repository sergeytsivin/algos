#include <algorithm>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

namespace {

struct Result {
    int value;
    std::string side;
};

Result local_search(const std::vector<std::pair<int, int>>& edges,
                    int n,
                    std::string side) {
    int value = 0;
    for (const auto& [u, v] : edges) {
        value += side[u] != side[v];
    }

    while (true) {
        std::vector<int> gain(n, 0);
        for (const auto& [u, v] : edges) {
            if (side[u] == side[v]) {
                ++gain[u];
                ++gain[v];
            } else {
                --gain[u];
                --gain[v];
            }
        }

        int best = 0;
        for (int v = 1; v < n; ++v) {
            if (gain[v] > gain[best]) {
                best = v;
            }
        }
        if (gain[best] <= 0) {
            break;
        }

        value += gain[best];
        side[best] = side[best] == '0' ? '1' : '0';
    }

    // Разрез не меняется при одновременной смене долей всех вершин.
    if (side[0] == '1') {
        for (char& bit : side) {
            bit = bit == '0' ? '1' : '0';
        }
    }
    return {value, side};
}

}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    int m;
    int starts;
    std::uint32_t seed;
    if (!(std::cin >> n >> m >> starts >> seed)) {
        return 0;
    }

    std::vector<std::pair<int, int>> edges;
    edges.reserve(m);
    for (int i = 0; i < m; ++i) {
        int u;
        int v;
        std::cin >> u >> v;
        edges.push_back({u - 1, v - 1});
    }

    std::mt19937 gen(seed);
    Result answer{-1, ""};
    for (int attempt = 0; attempt < starts; ++attempt) {
        std::string side(n, '0');
        for (char& bit : side) {
            bit = static_cast<char>('0' + (gen() & 1U));
        }

        Result current = local_search(edges, n, side);
        if (current.value > answer.value ||
            (current.value == answer.value && current.side < answer.side)) {
            answer = std::move(current);
        }
    }

    std::cout << answer.value << '\n' << answer.side << '\n';
}
