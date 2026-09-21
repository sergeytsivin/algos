#include <algorithm>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct Individual {
    string x;
    int value = 0;
};

int cut_value(const string& x, const vector<pair<int, int>>& edges) {
    int value = 0;
    for (auto [u, v] : edges) {
        value += (x[u] != x[v]);
    }
    return value;
}

void normalize(string& x) {
    // Разрез не меняется при замене обеих долей местами.
    if (!x.empty() && x[0] == '1') {
        for (char& bit : x) {
            bit = (bit == '0' ? '1' : '0');
        }
    }
}

bool better(const Individual& a, const Individual& b) {
    if (a.value != b.value) {
        return a.value > b.value;
    }
    return a.x < b.x;
}

int tournament(const vector<Individual>& population, mt19937& rng) {
    int best = static_cast<int>(rng() % population.size());
    for (int attempt = 1; attempt < 3; ++attempt) {
        int candidate = static_cast<int>(rng() % population.size());
        if (better(population[candidate], population[best])) {
            best = candidate;
        }
    }
    return best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, population_size, generations, mutation;
    uint32_t seed;
    cin >> n >> m >> population_size >> generations >> mutation >> seed;

    vector<pair<int, int>> edges(m);
    for (auto& [u, v] : edges) {
        cin >> u >> v;
        --u;
        --v;
    }

    mt19937 rng(seed);
    vector<Individual> population;
    population.reserve(population_size);
    for (int i = 0; i < population_size; ++i) {
        string x(n, '0');
        for (char& bit : x) {
            bit = static_cast<char>('0' + rng() % 2);
        }
        normalize(x);
        population.push_back({x, cut_value(x, edges)});
    }

    sort(population.begin(), population.end(), better);
    for (int generation = 0; generation < generations; ++generation) {
        vector<Individual> next;
        next.reserve(population_size);

        // Элитизм: лучший разрез без изменений переходит в новое поколение.
        next.push_back(population[0]);
        while (static_cast<int>(next.size()) < population_size) {
            int first = tournament(population, rng);
            int second = tournament(population, rng);

            int point = n;
            if (n >= 2) {
                point = 1 + static_cast<int>(rng() % (n - 1));
            }
            string child = population[first].x.substr(0, point)
                         + population[second].x.substr(point);

            for (char& bit : child) {
                if (rng() % 1000 < static_cast<uint32_t>(mutation)) {
                    bit = (bit == '0' ? '1' : '0');
                }
            }
            normalize(child);
            next.push_back({child, cut_value(child, edges)});
        }

        population = std::move(next);
        sort(population.begin(), population.end(), better);
    }

    cout << population[0].value << '\n' << population[0].x << '\n';
}
