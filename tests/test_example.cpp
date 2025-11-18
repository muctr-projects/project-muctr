#include <cassert>
#include <iostream>

// Пример простого теста
void test_example() {
    assert(1 + 1 == 2);
    std::cout << "✓ Basic test passed\n";
}

int main() {
    std::cout << "Running tests...\n";
    test_example();
    std::cout << "All tests passed! ✅\n";
    return 0;
}