#include "lib_add/add.hpp"
#include "lib_mul/multiply.hpp"
#include <iostream>

int main() {
    std::cout << "2 + 3 = " << add(2, 3) << std::endl;
    std::cout << "2 * 3 * FACTOR = " << multiply(2, 3) << std::endl;

    return 0;
}
