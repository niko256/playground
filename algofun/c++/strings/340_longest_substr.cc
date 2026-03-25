#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

/*
 *
 * You are given a string s and an integer k.
 * Your task is to find the length of the longest substring within s
 * that contains at most k distinct characters.
 */

auto longest_substr_most_k_diff_chars(std::string_view str, size_t k) -> std::string_view {
    if (str.empty()) {
        return "";
    }

    size_t left = 0;
    size_t start_idx = 0;
    size_t max_len = 0;

    // "ecebababece"

    std::unordered_map<char, int> freq;
    freq.reserve(str.size());

    for (size_t right = 0; right < str.size(); ++right) {
        ++freq[str[right]];

        while (freq.size() > k) {
            if (--freq[str[left]] == 0) {
                freq.erase(str[left]);
            }

            ++left;
        }

        auto len = right - left + 1;
        if (len > max_len) {
            max_len = len;
            start_idx = left;
        }
    }

    return str.substr(start_idx, max_len);
}

int main() {
    auto str = "ecebababece";

    int k = 2;  // => "baba"

    std::cout << "Given string : " << str << "\n";


    std::cout << "result : " << longest_substr_most_k_diff_chars(str, k) << std::endl;

    return 0;
}
