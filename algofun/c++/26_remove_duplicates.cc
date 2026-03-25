#include <iostream>
#include <vector>

std::vector<int> remove_duplicates(std::vector<int>& nums) {
    int ins_pos = 1;
    // индекс равен 1,
    // тк первый элемент гарантированно не является дубликатом

    // указатель, который будет пробегать до первого недубликата
    // чтобы установить это значение на место ins_pos
    for (int curr = 1; curr < nums.size(); ++curr) {
        if (nums[curr] == nums[curr - 1]) {
            continue;
        }

        nums[ins_pos] = nums[curr];

        // сдвигаем ins_pos
        ++ins_pos;
    }

    return nums;
}

int main() {
    std::vector<int> nums{1, 2, 3, 3, 3, 3, 4, 5};

    auto result = remove_duplicates(nums);

    std::cout << " Before : {1, 2, 3, 3, 3, 3, 4, 5} : " << std::endl;

    std::cout << " After : {";

    for (auto& el : nums) {
        std::cout << el << ", ";
    }

    std::cout << " }" << std::endl;

    return 0;
}
