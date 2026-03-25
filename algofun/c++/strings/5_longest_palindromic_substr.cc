#include <iostream>
#include <string>
#include <string_view>

// Input : s = "babad"
// Output : "bab" / "aba"

// std::string curr_pldr;
// 2 pointers + dp
//
// there are two cases : given palindrome has even length or odd length
// => aech substr has unique center =>
// => we can find the center and expand to the left and right
// while both sybmols [center - 1] and [center + 1] are the same
//
// each string has N symbols
// => there are N centers for odd length
// => there are N - 1 centers for even length
// hence there are N + (N - 1) = 2N - 1 centers
//
// for each potential center we call expand_around_center() with two pointers
//


int expand_around_center(std::string_view str, int left, int right) {
    while (left >= 0 && right < str.length() && str[left] == str[right]) {
        --left;
        ++right;
    }

    // length of the palindrome
    // for "bab"
    // l = -1; r = 3;
    // => we have to fix this "edges issue"
    // => right - left - 1 = 3 - (-1) - 1 = 3 = "bab".length()
    return right - left - 1;
}

std::string longest_palindrome(std::string_view str) {
    if (str.empty()) {
        return "";
    }

    int start = 0;
    int max_length = 0;

    // N iterations, but on each we check for 2 potential centers
    // => Finally, we check all of 2N - 1 centers
    // on each iteration we assume that i is an index of the center
    for (int i = 0; i < str.length(); ++i) {
        // check for odd length
        auto l_1 = expand_around_center(str, i, i);

        // check for even length
        auto l_2 = expand_around_center(str, i, i + 1);

        // take local maximum from even and odd palindromes
        int curr_max_length = std::max(l_1, l_2);

        if (curr_max_length > max_length) {
            max_length = curr_max_length;

            // determinataion of the begin index of found palindrome
            // we know it's center and total max_length
            //
            // since i - is an index => left and right edges are symmetric
            // => start = i - (max_length) / 2;
            // but bc of cpp division, we have to devide it correctly
            // for even and odd length cases :
            //
            // [example] : i = 1; max_len = 3
            // => 1 - (3 - 1) / 2 = 1 - 2/2 = 0
            //
            // [example] : i = 1; max_len = 4
            // => 1 - (4 - 1) / 2 = 1 - 3/2 = 1 - 1 = 0
            // by this '- 1' we fixed this problem
            start = i - (max_length - 1) / 2;
        }
    }

    return std::string(str.substr(start, max_length));
}

int main() {
    std::string str;
    std::cin >> str;

    auto result = longest_palindrome(str);

    std::cout << "5. Longest palindrome substring : " << result << std::endl;

    return 0;
}
