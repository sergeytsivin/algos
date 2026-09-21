#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    if (!(std::cin >> s)) {
        return 0;
    }

    std::map<std::pair<int, char>, int> transition;
    std::vector<int> answer;

    int next_code = 26;
    int cur = s[0] - 'A';

    for (std::size_t i = 1; i < s.size(); ++i) {
        const std::pair<int, char> key = {cur, s[i]};
        const auto it = transition.find(key);

        if (it != transition.end()) {
            cur = it->second;
        } else {
            answer.push_back(cur);
            transition[key] = next_code;
            ++next_code;
            cur = s[i] - 'A';
        }
    }

    answer.push_back(cur);

    std::cout << answer.size() << '\n';
    for (std::size_t i = 0; i < answer.size(); ++i) {
        if (i != 0) {
            std::cout << ' ';
        }
        std::cout << answer[i];
    }
    std::cout << '\n';
}
