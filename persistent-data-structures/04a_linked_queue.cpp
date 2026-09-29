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
    Node* tail = nullptr;
    int size = 0;
    while (q--) {
        std::string op;
        std::cin >> op;
        if (op == "push") {
            int x;
            std::cin >> x;
            Node* u = new Node{x, nullptr};
            if (tail == nullptr) {
                head = u;
            } else {
                tail->next = u;
            }
            tail = u;
            ++size;
        } else if (op == "pop") {
            Node* old = head;
            std::cout << old->x << '\n';
            head = old->next;
            if (head == nullptr) {
                // После удаления последнего элемента оба конца пусты.
                tail = nullptr;
            }
            delete old;
            --size;
        } else if (op == "front") {
            std::cout << head->x << '\n';
        } else {
            std::cout << size << '\n';
        }
    }

    while (head != nullptr) {
        Node* old = head;
        head = old->next;
        delete old;
    }
}
