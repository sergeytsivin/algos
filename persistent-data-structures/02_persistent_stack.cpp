#include <iostream>
#include <string>
#include <vector>

struct Node {
    int x;
    const Node* next;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q;
    std::cin >> q;
    // Узлы сохраняем до завершения программы: они нужны старым версиям.
    std::vector<const Node*> head(q + 1, nullptr);
    int k = 0;
    while (q--) {
        std::string op;
        int v;
        std::cin >> op >> v;
        if (op == "push") {
            int x;
            std::cin >> x;
            head[++k] = new Node{x, head[v]};
            std::cout << k << '\n';
        } else if (op == "pop") {
            head[++k] = head[v]->next;
            std::cout << k << ' ' << head[v]->x << '\n';
        } else {
            std::cout << head[v]->x << '\n';
        }
    }
}
