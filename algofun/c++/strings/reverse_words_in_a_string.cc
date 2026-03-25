#include <algorithm>
#include <string_view>
#include <iostream>

auto reverse_words_in_a_string(std::string& str) -> std::string_view {
    str.erase(0, str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ') + 1);

    std::reverse(str.begin(), str.end());

    size_t start = 0;

    for (size_t curr = 0; curr < str.length(); ++curr) {
        if (str[curr] == ' ' or curr == str.length() - 1) {
            std::reverse(str.begin() + start, str.begin() + curr);

            start = curr + 1;
        }
    }

    return str;
}

/*
 *
 * => сначала разворачиваем всю строку, затем ищем пробелы
 * и разворачиваем слова отдельно
 *
 * "hello my dear friends" -> "sdneirf raed ym olleh" -> "friends dear my hello"
 */

int main() {
    std::string str = "    abcdefg hijklmnop qrstuvw xyz!      ";

    std::cout << "Reversed words in a string : " << str << "\n";

    std::cout << " is : " << reverse_words_in_a_string(str) << std::endl;

    return 0;
}
