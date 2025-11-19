/**
 * @file test_main.cpp
 * @brief Главный файл для запуска всех модульных тестов
 */

#include "test_utils.h"
#include "test_newton_interpolator.h"
#include "test_gnuplot_visualizer.h"
#include <iostream>

int main() {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    std::cout << "========================================\n";
    std::cout << "Запуск модульных тестов\n";
    std::cout << "========================================\n";
    
    // Сбрасываем статистику
    g_test_stats.reset();
    
    // Запускаем все тесты
    run_newton_interpolator_tests();
    run_gnuplot_visualizer_tests();
    
    // Выводим итоги
    g_test_stats.print_summary();
    
    // Возвращаем код выхода в зависимости от результатов
    if (g_test_stats.failed > 0) {
        std::cout << "\nТестирование завершено с ошибками.\n";
        return 1;
    } else {
        std::cout << "\nВсе тесты пройдены успешно! ✅\n";
        return 0;
    }
}

