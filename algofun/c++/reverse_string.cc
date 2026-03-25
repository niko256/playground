#include <cstdlib>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <string_view>
#include <iostream>

/*
 * "hello" -> "olleh"
 * "privet" -> "tevirp"
 * "" -> ""
 * "a" -> "a"
 *
 *
 */

auto reverse_string(char* str) -> std::string_view {
    if (str == nullptr) {
        throw std::logic_error("invalid string");
    }

    /*
     * two-pointers approach
     */

    /* strlen ~ O(n) */
    int len = strlen(str);

    if (len <= 1) {
        return str;
    }

    char* left = str;
    char* right = str + len - 1;

    while (left < right) {
        char tmp = *left;
        *left = *right;
        *right = tmp;

        ++left;
        --right;
    }

    return str;
}

auto reverse_substr(char* str, size_t left, size_t right) -> std::string_view {
    if (str == nullptr) {
        throw std::logic_error("invalid str...");
    }

    while (left < right) {
        char tmp = str[left];
        str[left] = str[right];
        str[right] = tmp;

        ++left;
        --right;
    }
    return str;
}

int main() {
    char str[100];
    char str2[] = "qwertypop";

    // std::cout << "Enter a string : ";

    // std::cin >> str;

    // std::cout << "Initial string: " << str << std::endl;

    // try {
    //     reverse_string(str);
    // } catch (const std::exception& err) {
    //     std::cerr << "Error : " << err.what() << std::endl;
    //     abort();
    // }


    // std::cout << "Reversed string: " << str << std::endl;

    std::cout << "Enter edges of substring that you want to reverse inside this string : "
              << str2 << "\n";

    size_t left, right;

    std::cin >> left >> right;

    try {
        std::cout << "Reversed substring : " << reverse_substr(str2, left, right) << "\n";
    } catch (const std::exception& err) {
        std::cerr << "Error... : " << err.what() << std::endl;
        abort();
    }

    return 0;
}
