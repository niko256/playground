#include <iostream>
#include <string_view>
#include <unordered_map>

auto longest_substr_with_unique_chars(std::string_view str) -> size_t {
    std::unordered_map<char, int> freq;
    int max_len = 0;

    for (int right = 0; right < str.length(); ++right) {
        int left = 0;

        ++freq[str[right]];

        while (freq[str[right]] > 1) {
            auto left_char = str[left];
            --freq[left_char];
            ++left;

            if (freq[str[right]] == 0) {
                freq.erase(str[right]);
            }
        }

        auto local_len = right - left + 1;
        if (local_len > max_len) {
            max_len = local_len;
        }
    }
    return max_len;
}

int main() {
    return 0;
}
