#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>
#include <vector>

struct Node {
    char min_symbol;
    int left;
    int right;
};

void make_codes(int v, std::string& path, const std::vector<Node>& tree,
                std::array<std::string, 26>& code) {
    const Node& node = tree[v];
    if (node.left == -1) {
        code[node.min_symbol - 'A'] = path.empty() ? "0" : path;
        return;
    }

    path.push_back('0');
    make_codes(node.left, path, tree, code);

    path.back() = '1';
    make_codes(node.right, path, tree, code);

    path.pop_back();
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::string s;
    if (!(std::cin >> n >> s)) {
        return 0;
    }

    std::array<long long, 26> freq{};
    for (char c : s) {
        ++freq[c - 'A'];
    }

    std::vector<Node> tree;

    // Элемент очереди: вес, минимальная буква и номер корня дерева.
    using Item = std::tuple<long long, char, int>;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> q;

    int k = 0;
    for (int i = 0; i < 26; ++i) {
        if (freq[i] == 0) {
            continue;
        }

        const char c = static_cast<char>('A' + i);
        tree.push_back({c, -1, -1});
        const int v = static_cast<int>(tree.size()) - 1;
        q.emplace(freq[i], c, v);
        ++k;
    }

    while (q.size() > 1) {
        const auto [w1, min1, v1] = q.top();
        q.pop();
        const auto [w2, min2, v2] = q.top();
        q.pop();

        // Первое извлечённое дерево становится левым сыном.
        const char min_symbol = std::min(min1, min2);
        tree.push_back({min_symbol, v1, v2});
        const int v = static_cast<int>(tree.size()) - 1;
        q.emplace(w1 + w2, min_symbol, v);
    }

    const int root = std::get<2>(q.top());
    std::array<std::string, 26> code;
    std::string path;
    make_codes(root, path, tree, code);

    std::cout << k << '\n';
    for (int i = 0; i < 26; ++i) {
        if (freq[i] != 0) {
            std::cout << static_cast<char>('A' + i) << ' ' << code[i] << '\n';
        }
    }

    for (char c : s) {
        std::cout << code[c - 'A'];
    }
    std::cout << '\n';
}
