#include <iostream>
#include <string_view>

int needle_in_haystack(std::string_view haystack, std::string_view needle) {
    if (needle.empty()) {
        return 0;
    }

    int needle_l = needle.length();
    int haystack_l = haystack.length();

    if (haystack_l < needle_l) {
        return -1;
    }

    // why i < haystack_l - needle_l ???
    for (int i = 0; i < haystack_l - needle_l; ++i) {
        // раз первый символ в needle не совпадает => идем к следующему
        if (haystack[i] != needle[0]) {
            continue;
        }

        int width = 0;

        // sliding window
        while (width < needle_l && needle[width] == haystack[i + width]) {
            ++width;
        }

        if (width == needle_l) {
            // возвращаем i, тк нам нужно вернуть
            // индекс начала вхождения
            return i;
        }
    }

    return -1;
}

int main() {
    auto h1 = "sadbutsad";
    auto n1 = "sad";

    auto h2 = "leetcode";
    auto n2 = "leeto";

    auto h3 = "hallo!";
    auto n3 = "ll";

    std::cout << "haystack: " << h1 << ", needle: " << n1 << std::endl;
    std::cout << "Result: " << needle_in_haystack(h1, n1) << std::endl;


    std::cout << "haystack: " << h2 << ", needle: " << n2 << std::endl;
    std::cout << "Result: " << needle_in_haystack(h2, n2) << std::endl;

    std::cout << "haystack: " << h3 << ", needle: " << n3 << std::endl;
    std::cout << "Result: " << needle_in_haystack(h3, n3) << std::endl;

    return 0;
}
