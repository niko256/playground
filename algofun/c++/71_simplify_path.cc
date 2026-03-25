#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

std::vector<std::string> parsing_path(const std::string& path) {
    std::vector<std::string> components;
    std::istringstream iss{path};
    std::string before_slash;

    while (std::getline(iss, before_slash, '/')) {
        if (!before_slash.empty()) {
            components.push_back(before_slash);
        }
    }

    return components;
}

std::string simplify_path(std::string_view str) {
    // делим путь по компонентам по символам разделения '/'
    // валидные компоненты храним в стеке
    //
    // граничные случаи :
    // - '.' - текущая директория, игнорируем
    // - '..' - родительская директория, удаляем последний элемент со стека
    // => поднимаемся вверх по пути
    // - игнорируем пустую строку
    // - отальные цепочки символов являются валидными  и мы добавляем их в стек
    //
    // /home/ -> /home
    auto components = parsing_path(str.data());

    std::vector<std::string> valid;

    for (const auto& comp : components) {
        if (comp == ".") {
            continue;
        } else if (comp == "..") {
            if (!valid.empty()) {
                valid.pop_back();
            }
        } else {
            // обычное имя директории / файла
            valid.push_back(comp);
        }
    }

    if (valid.empty()) {
        return "/";
    }

    std::string res;
    for (const auto& dir : valid) {
        res += "/" + dir;
    }

    return res;
}

int main() {
    std::string input;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "q" || input == "quit") {
            break;
        }

        std::string res = simplify_path(input);
        std::cout << "Simplified path : " << std::endl
                  << "before : " << input << std::endl
                  << "after : " << res << std::endl;
    }

    return 0;
}
