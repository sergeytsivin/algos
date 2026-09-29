#include <iostream>
#include <string>
#include <vector>

struct Node {
    int x;
    Node* left;
    Node* right;
};

Node* build(const std::vector<int>& a, int l, int r) {
    if (r - l == 1) {
        return new Node{a[l], nullptr, nullptr};
    }
    int m = (l + r) / 2;
    Node* left = build(a, l, m);
    Node* right = build(a, m, r);
    return new Node{0, left, right};
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

void setElement(Node* v, int l, int r, int i, int x) {
    if (r - l == 1) {
        v->x = x;
        return;
    }
    int m = (l + r) / 2;
    if (i < m) {
        setElement(v->left, l, m, i, x);
    } else {
        setElement(v->right, m, r, i, x);
    }
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
    Node* root = build(a, 0, n);
    while (q--) {
        std::string op;
        int i;
        std::cin >> op >> i;
        if (op == "get") {
            std::cout << getElement(root, 0, n, i) << '\n';
        } else {
            int x;
            std::cin >> x;
            setElement(root, 0, n, i, x);
        }
    }
}
