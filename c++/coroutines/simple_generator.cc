#include <coroutine>
#include <generator>
#include <iostream>

std::generator<int> fibbonaci_generator() {
  int a{};
  int b{1};

  int c{};

  while (true) {
    co_yield a;
    c = a + b;
    a = b;
    b = c;
  }
}

auto fib = fibbonaci_generator();

int main() {
  int i = 0;
  for (auto f = fib.begin(); f != fib.end(); ++f) {
    if (i == 10) {
      break;
    }

    std::cout << "Another generator's value... : " << *f << std::endl;
    ++i;
  }
  std::cout << std::endl;
}
