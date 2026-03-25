#include <iostream>
#include <string>
#include <vector>

void backtrack(
    std::vector<std::string>& vec,
    std::string current,
    int open,
    int closed,
    int n
) {
    if (open == n and closed == n) {
        vec.push_back(current);
        return;  // back to stack
    }

    if (open < n) {
        auto new_curr = current + "(";
        backtrack(vec, new_curr, open + 1, closed, n);

        std::cout << "[current] : " << new_curr << "\n";
    }

    if (closed < open) {
        auto new_curr = current + ")";
        backtrack(vec, new_curr, open, closed + 1, n);

        std::cout << "[current] : " << new_curr << "\n";
    }
}

auto generate_parantheses(int n) -> std::vector<std::string> {
    std::vector<std::string> res;
    backtrack(res, "", 0, 0, n);

    return res;
}

int main() {
    int input;
    std::cin >> input;

    generate_parantheses(input);

    return 0;
}
