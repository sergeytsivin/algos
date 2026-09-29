#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int c, q;
    std::cin >> c >> q;
    std::vector<int> a(c);
    int head = 0;
    int size = 0;
    while (q--) {
        std::string op;
        std::cin >> op;
        if (op == "push") {
            int x;
            std::cin >> x;
            int i = (head + size) % c;
            a[i] = x;
            ++size;
        } else if (op == "pop") {
            std::cout << a[head] << '\n';
            head = (head + 1) % c;
            --size;
        } else if (op == "front") {
            std::cout << a[head] << '\n';
        } else {
            std::cout << size << '\n';
        }
    }
}
