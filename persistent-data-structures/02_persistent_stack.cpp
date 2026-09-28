#include <deque>
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
    // Добавление в конец deque не меняет адреса уже созданных узлов.
    std::deque<Node> pool;
    std::vector<const Node*> head(q + 1, nullptr);
    int k = 0;
    while (q--) {
        std::string op;
        int v;
        std::cin >> op >> v;
        if (op == "push") {
            int x;
            std::cin >> x;
            pool.push_back({x, head[v]});
            head[++k] = &pool.back();
            std::cout << k << '\n';
        } else if (op == "pop") {
            head[++k] = head[v]->next;
            std::cout << k << ' ' << head[v]->x << '\n';
        } else {
            std::cout << head[v]->x << '\n';
        }
    }
}
