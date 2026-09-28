#include <algorithm>
#include <iostream>

int main() {
    int n, m, x1, y1, x2, y2;
    if (!(std::cin >> n >> m >> x1 >> y1 >> x2 >> y2)) return 0;

    bool ok;
    if (n == 1 || m == 1) {
        int p = (n == 1 ? y1 : x1);
        int q = (n == 1 ? y2 : x2);
        if (p > q) std::swap(p, q);
        int len = n * m;
        // Каждая из трёх оставшихся полос должна иметь чётную длину.
        ok = (p - 1) % 2 == 0 && (q - p - 1) % 2 == 0 &&
             (len - q) % 2 == 0;
    } else {
        // В чётном прямоугольнике есть цикл по всем клеткам.
        ok = n * m % 2 == 0 && (x1 + y1) % 2 != (x2 + y2) % 2;
    }
    std::cout << (ok ? "YES" : "NO") << '\n';
}
