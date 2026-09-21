#include <iostream>
#include <string>

bool is_power_of_two(int x) {
    return (x & (x - 1)) == 0;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int r;
    std::string received;
    if (!(std::cin >> r >> received)) {
        return 0;
    }

    const int n = (1 << r) - 1;
    int syndrome = 0;

    // Каждый разряд синдрома хранит результат одной проверки на чётность.
    for (int i = 1; i <= n; ++i) {
        if (received[i - 1] == '1') {
            syndrome ^= i;
        }
    }

    const int error_position = syndrome;
    if (syndrome != 0) {
        char& bit = received[syndrome - 1];
        bit = (bit == '0' ? '1' : '0');
    }

    std::string data;
    data.reserve(n - r);
    for (int i = 1; i <= n; ++i) {
        if (!is_power_of_two(i)) {
            data.push_back(received[i - 1]);
        }
    }

    std::cout << error_position << '\n';
    std::cout << received << '\n';
    std::cout << data << '\n';
}
