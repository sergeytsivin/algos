#include <algorithm>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

namespace {

std::uint64_t next(std::uint64_t& state) {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    int m;
    if (!(std::cin >> n >> m)) {
        return 0;
    }

    int steps;
    long long temperature;
    long long cool_num;
    long long cool_den;
    std::uint64_t state;
    std::cin >> steps >> temperature >> cool_num >> cool_den >> state;

    std::vector<std::vector<std::pair<int, long long>>> adj(n);
    std::vector<int> edge_u(m);
    std::vector<int> edge_v(m);
    std::vector<long long> edge_w(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> edge_u[i] >> edge_v[i] >> edge_w[i];
        --edge_u[i];
        --edge_v[i];
        adj[edge_u[i]].push_back({edge_v[i], edge_w[i]});
        adj[edge_v[i]].push_back({edge_u[i], edge_w[i]});
    }

    std::vector<int> side(n);
    for (int v = 0; v < n; ++v) {
        side[v] = static_cast<int>(next(state) & 1ULL);
    }

    long long value = 0;
    for (int i = 0; i < m; ++i) {
        if (side[edge_u[i]] != side[edge_v[i]]) {
            value += edge_w[i];
        }
    }

    long long best_value = value;
    std::vector<int> best_side = side;
    int steps_done = 0;

    while (steps_done < steps) {
        const int v =
            static_cast<int>(next(state) % static_cast<std::uint64_t>(n));
        long long gain = 0;
        for (const auto& [u, w] : adj[v]) {
            gain += (side[u] == side[v] ? w : -w);
        }

        bool accept = gain >= 0;
        if (gain < 0) {
            const long long loss = -gain;
            const std::uint64_t range =
                static_cast<std::uint64_t>(temperature + loss);
            accept = next(state) % range < static_cast<std::uint64_t>(temperature);
        }

        if (accept) {
            side[v] ^= 1;
            value += gain;
            if (value > best_value) {
                best_value = value;
                best_side = side;
            }
        }

        temperature = std::max(1LL, temperature * cool_num / cool_den);
        ++steps_done;
    }

    // Разрез не меняется при одновременной смене стороны всех вершин.
    if (best_side[0] == 1) {
        for (int& bit : best_side) {
            bit ^= 1;
        }
    }

    std::cout << best_value << '\n';
    for (int bit : best_side) {
        std::cout << bit;
    }
    std::cout << '\n';
}
