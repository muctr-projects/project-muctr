/**
 * @file test_utils.h
 * @brief Утилиты для модульного тестирования без внешних фреймворков
 */

#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <functional>

/**
 * @brief Счетчик тестов
 */
struct TestStats {
    int total = 0;
    int passed = 0;
    int failed = 0;
    
    void reset() {
        total = 0;
        passed = 0;
        failed = 0;
    }
    
    void print_summary() const {
        std::cout << "\n========================================\n";
        std::cout << "Итоги тестирования:\n";
        std::cout << "Всего тестов: " << total << "\n";
        std::cout << "Пройдено: " << passed << "\n";
        std::cout << "Провалено: " << failed << "\n";
        std::cout << "========================================\n";
    }
};

extern TestStats g_test_stats;

/**
 * @brief Проверяет условие и выводит сообщение
 */
inline void assert_true(bool condition, const std::string& message) {
    g_test_stats.total++;
    if (condition) {
        g_test_stats.passed++;
        std::cout << "  ✓ " << message << "\n";
    } else {
        g_test_stats.failed++;
        std::cout << "  ✗ " << message << " [FAILED]\n";
    }
}

/**
 * @brief Проверяет равенство двух значений с заданной точностью
 */
inline void assert_double_equal(double actual, double expected, 
                                double epsilon = 1e-9, 
                                const std::string& message = "") {
    g_test_stats.total++;
    double diff = std::abs(actual - expected);
    if (diff < epsilon) {
        g_test_stats.passed++;
        std::cout << "  ✓ " << (message.empty() ? "Значения равны" : message) << "\n";
    } else {
        g_test_stats.failed++;
        std::cout << "  ✗ " << (message.empty() ? "Значения не равны" : message) 
                  << " [ожидалось: " << expected << ", получено: " << actual 
                  << ", разница: " << diff << "]\n";
    }
}

/**
 * @brief Проверяет, что исключение было выброшено
 */
template<typename ExceptionType>
inline void assert_throws(const std::function<void()>& func, const std::string& message) {
    g_test_stats.total++;
    try {
        func();
        g_test_stats.failed++;
        std::cout << "  ✗ " << message << " [ожидалось исключение, но его не было]\n";
    } catch (const ExceptionType&) {
        g_test_stats.passed++;
        std::cout << "  ✓ " << message << "\n";
    } catch (...) {
        g_test_stats.failed++;
        std::cout << "  ✗ " << message << " [выброшено другое исключение]\n";
    }
}

/**
 * @brief Проверяет равенство векторов
 */
inline void assert_vector_equal(const std::vector<double>& actual, 
                                const std::vector<double>& expected,
                                double epsilon = 1e-9,
                                const std::string& message = "") {
    g_test_stats.total++;
    if (actual.size() != expected.size()) {
        g_test_stats.failed++;
        std::cout << "  ✗ " << (message.empty() ? "Размеры векторов не совпадают" : message)
                  << " [ожидался размер: " << expected.size() 
                  << ", получен: " << actual.size() << "]\n";
        return;
    }
    
    bool all_equal = true;
    for (size_t i = 0; i < actual.size(); i++) {
        if (std::abs(actual[i] - expected[i]) >= epsilon) {
            all_equal = false;
            break;
        }
    }
    
    if (all_equal) {
        g_test_stats.passed++;
        std::cout << "  ✓ " << (message.empty() ? "Векторы равны" : message) << "\n";
    } else {
        g_test_stats.failed++;
        std::cout << "  ✗ " << (message.empty() ? "Векторы не равны" : message) << "\n";
    }
}

/**
 * @brief Выводит заголовок группы тестов
 */
inline void test_group(const std::string& group_name) {
    std::cout << "\n[" << group_name << "]\n";
}

#endif // TEST_UTILS_H

