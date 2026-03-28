#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

auto is_anagram(std::string_view s, std::string_view t) -> bool {
    std::unordered_map<char, int> freq;

    /// "abcda"
    /// "bcdaa"

    for (const auto& symb : s) {
        ++freq[symb];
    }

    for (const auto& symb : t) {
        --freq[symb];
        if (freq.count(symb) < 0) {
            return false;
        }
    }

    for (const auto& el : freq) {
        if (el.second != 0) {
            return false;
        }
    }

    return true;
}

int main() {
    const char a1[] = "fuck you bitch";
    const char a2[] = "no, actually, fuck you!";
    const char a3[] = "ufck uoy hctib";

    std::cout << "Is anagram : " << "\n";

    std::cout << "a1 : " << a1 << "\n";
    std::cout << "a2 : " << a2 << "\n";
    std::cout << "a3 : " << a3 << "\n";

    std::cout << "a1 and a2 : " << is_anagram(a1, a2) << "\n";
    std::cout << "a2 and a3 : " << is_anagram(a2, a3) << "\n";
    std::cout << "a1 and a3 : " << is_anagram(a1, a3) << "\n";
    std::cout << "a1 and a1 : " << is_anagram(a1, a1) << "\n";

    return 0;
}
