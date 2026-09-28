#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using std::vector;

struct Graph {
    vector<vector<int>> g;
    int m;
};

struct Stats {
    std::uint64_t roots = 0;
    std::uint64_t scans = 0;
    std::uint64_t clearWrites = 0;
    int greedyPairs = 0;
};

struct Result {
    vector<int> pairFromA;
    int size = 0;
    Stats stats;
};

// В измеряемой версии Count=false: счётчиков внутри обхода нет.
template <bool Count>
bool dfs(const vector<vector<int>>& graphA, int vA, vector<int>& visitedA,
         vector<int>& pairFromA, int stamp, Stats& stats) {
    if (visitedA[vA] == stamp) return false;
    visitedA[vA] = stamp;
    for (int vB : graphA[vA]) {
        if constexpr (Count) ++stats.scans;
        if (pairFromA[vB] == -1 ||
            dfs<Count>(graphA, pairFromA[vB], visitedA, pairFromA, stamp, stats)) {
            pairFromA[vB] = vA;
            return true;
        }
    }
    return false;
}

template <bool Count>
Result kuhn(const Graph& graph, bool stamps, bool greedy) {
    int n = static_cast<int>(graph.g.size());
    Result result;
    result.pairFromA.assign(graph.m, -1);
    vector<int> visitedA(n, 0);
    vector<int> matchedA;
    if (greedy) {
        matchedA.assign(n, 0);
        for (int a = 0; a < n; ++a) {
            for (int b : graph.g[a]) {
                if constexpr (Count) ++result.stats.scans;
                if (result.pairFromA[b] == -1) {
                    result.pairFromA[b] = a;
                    matchedA[a] = 1;
                    ++result.size;
                    if constexpr (Count) ++result.stats.greedyPairs;
                    break;
                }
            }
        }
    }
    int stamp = 0;
    for (int a = 0; a < n; ++a) {
        if (greedy && matchedA[a]) continue;
        if (stamps) {
            ++stamp;
        } else {
            std::fill(visitedA.begin(), visitedA.end(), 0);
            stamp = 1;
            if constexpr (Count) result.stats.clearWrites += n;
        }
        if constexpr (Count) ++result.stats.roots;
        if (dfs<Count>(graph.g, a, visitedA, result.pairFromA, stamp, result.stats)) {
            ++result.size;
        }
    }
    return result;
}

Graph readGraph(const std::string& path) {
    std::ifstream in(path);
    int n, m, k;
    if (!(in >> n >> m >> k) || n < 1 || m < 1 || k < 0) {
        throw std::runtime_error("Cannot read graph: " + path);
    }
    Graph graph{vector<vector<int>>(n), m};
    for (int i = 0; i < k; ++i) {
        int a, b;
        if (!(in >> a >> b) || a < 1 || a > n || b < 1 || b > m) {
            throw std::runtime_error("Invalid edge: " + path);
        }
        graph.g[a - 1].push_back(b - 1);
    }
    std::string extra;
    if (in >> extra) throw std::runtime_error("Trailing data: " + path);
    return graph;
}

Graph transpose(const Graph& graph) {
    Graph rev{vector<vector<int>>(graph.m), static_cast<int>(graph.g.size())};
    for (int a = 0; a < static_cast<int>(graph.g.size()); ++a) {
        for (int b : graph.g[a]) rev.g[b].push_back(a);
    }
    return rev;
}

void check(const Graph& graph, const Result& result) {
    vector<int> used(graph.g.size(), 0);
    int size = 0;
    for (int b = 0; b < graph.m; ++b) {
        int a = result.pairFromA[b];
        if (a == -1) continue;
        if (a < 0 || a >= static_cast<int>(graph.g.size()) || used[a] ||
            std::find(graph.g[a].begin(), graph.g[a].end(), b) == graph.g[a].end()) {
            throw std::runtime_error("Invalid matching");
        }
        used[a] = 1;
        ++size;
    }
    if (size != result.size) throw std::runtime_error("Invalid matching size");
}

int measure(const std::string& name, const Graph& graph, bool stamps, bool greedy) {
    // Отдельный проход собирает счётчики и служит прогревом.
    Result counted = kuhn<true>(graph, stamps, greedy);
    check(graph, counted);
    vector<double> times;
    for (int run = 0; run < 5; ++run) {
        auto start = std::chrono::steady_clock::now();
        Result result = kuhn<false>(graph, stamps, greedy);
        auto finish = std::chrono::steady_clock::now();
        times.push_back(std::chrono::duration<double, std::milli>(finish - start).count());
        // Проверки и вывод находятся вне измеряемого участка.
        check(graph, result);
        if (result.size != counted.size) throw std::runtime_error("Unstable answer");
    }
    std::sort(times.begin(), times.end());
    std::cout << name << ',' << counted.size << ',' << std::fixed
              << std::setprecision(3) << times[2] << ',' << counted.stats.roots
              << ',' << counted.stats.scans << ',' << counted.stats.clearWrites
              << ',' << counted.stats.greedyPairs << '\n';
    return counted.size;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: bench sparse.txt dense.txt\n";
        return 1;
    }
    try {
        Graph sparse = readGraph(argv[1]);
        Graph dense = readGraph(argv[2]);
        Graph large{vector<vector<int>>(100000), 100};
        for (int a = 0; a < 100000; ++a) large.g[a].push_back(a % 100);
        Graph small = transpose(large);

        std::cout << "variant,size,median_ms,roots,edge_scans,clear_writes,greedy_pairs\n";
        int a1 = measure("A_large_stamps", large, true, false);
        int a2 = measure("A_small_stamps", small, true, false);
        int b1 = measure("B_clear", sparse, false, false);
        int b2 = measure("B_stamps", sparse, true, false);
        int c1 = measure("C_stamps", dense, true, false);
        int c2 = measure("C_stamps_greedy", dense, true, true);
        if (a1 != a2 || a1 != 100 || b1 != b2 || c1 != c2) {
            throw std::runtime_error("Different answers between variants");
        }
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
