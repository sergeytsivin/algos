#include <iostream>
#include <string>

struct Node {
    int x;
    Node* next;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q;
    std::cin >> q;
    Node* head = nullptr;
    while (q--) {
        std::string op;
        std::cin >> op;
        if (op == "push") {
            int x;
            std::cin >> x;
            head = new Node{x, head};
        } else if (op == "pop") {
            Node* old = head;
            std::cout << old->x << '\n';
            head = old->next;
            delete old;
        } else {
            std::cout << head->x << '\n';
        }
    }

    while (head != nullptr) {
        Node* old = head;
        head = old->next;
        delete old;
    }
}
