#include <deque>
#include <iostream>
#include <string>
#include <vector>

struct Node {
    int x;
    const Node* left;
    const Node* right;
};

struct Version {
    const Node* root;
    int head;
    int size;
};

std::deque<Node> pool;

const Node* build(int l, int r) {
    if (r - l == 1) {
        pool.push_back({0, nullptr, nullptr});
    } else {
        int m = (l + r) / 2;
        const Node* left = build(l, m);
        const Node* right = build(m, r);
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

    int c, q;
    std::cin >> c >> q;
    std::vector<Version> ver(q + 1);
    ver[0] = {build(0, c), 0, 0};
    int k = 0;
    while (q--) {
        std::string op;
        int v;
        std::cin >> op >> v;
        Version cur = ver[v];
        if (op == "push") {
            int x;
            std::cin >> x;
            int i = (cur.head + cur.size) % c;
            cur.root = setElement(cur.root, 0, c, i, x);
            ++cur.size;
            ver[++k] = cur;
            std::cout << k << '\n';
        } else if (op == "pop") {
            int x = getElement(cur.root, 0, c, cur.head);
            cur.head = (cur.head + 1) % c;
            --cur.size;
            // Ячейку не очищаем: размер исключает её из новой очереди.
            ver[++k] = cur;
            std::cout << k << ' ' << x << '\n';
        } else if (op == "front") {
            std::cout << getElement(cur.root, 0, c, cur.head) << '\n';
        } else {
            std::cout << cur.size << '\n';
        }
    }
}
