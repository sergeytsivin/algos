#include <deque>
#include <iostream>
#include <string>
#include <vector>

struct Node {
    int x;
    const Node* left;
    const Node* right;
};

std::deque<Node> pool;

const Node* build(const std::vector<int>& a, int l, int r) {
    if (r - l == 1) {
        pool.push_back({a[l], nullptr, nullptr});
    } else {
        int m = (l + r) / 2;
        const Node* left = build(a, l, m);
        const Node* right = build(a, m, r);
        pool.push_back({0, left, right});
    }
    return &pool.back();
}

int getElement(const Node* v, int l, int r, int i) {
    if (r - l == 1) {
        return v->x;
    }
    int m = (l + r) / 2;
    if (i < m) {
        return getElement(v->left, l, m, i);
    }
    return getElement(v->right, m, r, i);
}

const Node* setElement(const Node* v, int l, int r, int i, int x) {
    pool.push_back(*v);
    Node* u = &pool.back();
    // Меняем только свежую копию; второй ребёнок остаётся общим.
    if (r - l == 1) {
        u->x = x;
    } else {
        int m = (l + r) / 2;
        if (i < m) {
            u->left = setElement(v->left, l, m, i, x);
        } else {
            u->right = setElement(v->right, m, r, i, x);
        }
    }
    return u;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::vector<int> a(n);
    for (int& x : a) {
        std::cin >> x;
    }
    std::vector<const Node*> root(q + 1, nullptr);
    root[0] = build(a, 0, n);
    int k = 0;
    while (q--) {
        std::string op;
        int v, i;
        std::cin >> op >> v >> i;
        if (op == "get") {
            std::cout << getElement(root[v], 0, n, i) << '\n';
        } else {
            int x;
            std::cin >> x;
            root[++k] = setElement(root[v], 0, n, i, x);
            std::cout << k << '\n';
        }
    }
}
