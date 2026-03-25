#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>

/*
 * Given a string s and an integer k,
 * reverse the first k characters for every 2k characters
 * counting from the start of the string.
 *
 * If there are fewer than k characters left,
 * reverse all of them.
 *
 * If there are less than 2k but greater than or equal to k characters,
 * then reverse the first k characters and leave the other as original.
 *
 *
 */


auto rev_str_II(std::string& str, size_t k) -> std::string_view {
    size_t len = str.length();

    // "abcdefg", k = 2
    // "bacdfeg"

    for (int i = 0; i < len; i += 2 * k) {
        auto left = i;
        auto right = std::min(i + k - 1, len - 1);

        while (left < right) {
            std::swap(str[left], str[right]);
            ++left;
            --right;
        }
    }
    return str;
}

int main() {
    std::string str = "abcdefg";
    int k = 2;

    std::cout << str << " -> " << rev_str_II(str, k) << std::endl;

    return 0;
}
