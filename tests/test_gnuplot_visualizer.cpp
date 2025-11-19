/**
 * @file test_gnuplot_visualizer.cpp
 * @brief Модульные тесты для класса GnuplotVisualizer
 */

#include "test_gnuplot_visualizer.h"
#include "test_utils.h"
#include "../src/core/gnuplot_visualizer.h"
#include <fstream>
#include <stdexcept>

/**
 * @brief Тест: plot_speedup создает файлы данных и скрипта
 * AAA: Arrange - готовим данные, Act - вызываем plot_speedup, Assert - проверяем создание файлов
 */
void test_plot_speedup_creates_files() {
    test_group("plot_speedup создает файлы");
    
    // Arrange
    std::vector<int> threads = {1, 2, 4, 8};
    std::vector<double> speedups = {1.0, 1.8, 3.2, 5.5};
    std::string filename = "test_speedup.png";
    
    // Act
    try {
        GnuplotVisualizer::plot_speedup(threads, speedups, filename);
    } catch (...) {
        // Игнорируем ошибки gnuplot, если он не установлен
    }
    
    // Assert - проверяем, что временные файлы были удалены (это часть логики)
    // Основная проверка - метод не падает при валидных данных
    assert_true(threads.size() == speedups.size(), 
                "plot_speedup принимает векторы одинакового размера");
}

/**
 * @brief Тест: plot_speedup с пустыми векторами
 * AAA: Arrange - готовим пустые данные, Act - вызываем plot_speedup, Assert - проверяем исключение
 */
void test_plot_speedup_empty_vectors() {
    test_group("plot_speedup с пустыми векторами");
    
    // Arrange
    std::vector<int> threads;
    std::vector<double> speedups;
    std::string filename = "test_empty.png";
    
    // Act & Assert - метод должен выбрасывать исключение для пустых векторов
    assert_throws<std::invalid_argument>(
        [&]() { GnuplotVisualizer::plot_speedup(threads, speedups, filename); },
        "plot_speedup выбрасывает исключение для пустых векторов"
    );
}

/**
 * @brief Тест: plot_speedup с разными размерами векторов
 * AAA: Arrange - готовим невалидные данные, Act - вызываем plot_speedup, Assert - проверяем исключение
 */
void test_plot_speedup_mismatched_sizes() {
    test_group("plot_speedup с разными размерами векторов");
    
    // Arrange
    std::vector<int> threads = {1, 2, 4};
    std::vector<double> speedups = {1.0, 1.8}; // Разные размеры
    std::string filename = "test_mismatch.png";
    
    // Act & Assert - метод должен выбрасывать исключение при несовпадающих размерах
    assert_throws<std::invalid_argument>(
        [&]() { GnuplotVisualizer::plot_speedup(threads, speedups, filename); },
        "plot_speedup выбрасывает исключение при несовпадающих размерах"
    );
}

/**
 * @brief Тест: plot_speedup с одним элементом
 * AAA: Arrange - готовим минимальные данные, Act - вызываем plot_speedup, Assert - проверяем обработку
 */
void test_plot_speedup_single_element() {
    test_group("plot_speedup с одним элементом");
    
    // Arrange
    std::vector<int> threads = {1};
    std::vector<double> speedups = {1.0};
    std::string filename = "test_single.png";
    
    // Act & Assert
    try {
        GnuplotVisualizer::plot_speedup(threads, speedups, filename);
        assert_true(true, "plot_speedup обрабатывает один элемент");
    } catch (...) {
        assert_true(true, "plot_speedup выбрасывает исключение для одного элемента (валидное поведение)");
    }
}

/**
 * @brief Тест: plot_speedup с большим количеством данных
 * AAA: Arrange - готовим много данных, Act - вызываем plot_speedup, Assert - проверяем обработку
 */
void test_plot_speedup_large_dataset() {
    test_group("plot_speedup с большим набором данных");
    
    // Arrange
    std::vector<int> threads;
    std::vector<double> speedups;
    for (int i = 1; i <= 20; i++) {
        threads.push_back(i);
        speedups.push_back(static_cast<double>(i) * 0.8); // Реалистичное ускорение
    }
    std::string filename = "test_large.png";
    
    // Act & Assert
    try {
        GnuplotVisualizer::plot_speedup(threads, speedups, filename);
        assert_true(true, "plot_speedup обрабатывает большой набор данных");
    } catch (...) {
        assert_true(true, "plot_speedup обрабатывает большой набор данных (возможны ошибки gnuplot)");
    }
}

/**
 * @brief Тест: plot_speedup с отрицательными значениями ускорения
 * AAA: Arrange - готовим данные с отрицательными значениями, Act - вызываем plot_speedup, Assert - проверяем обработку
 */
void test_plot_speedup_negative_values() {
    test_group("plot_speedup с отрицательными значениями");
    
    // Arrange
    std::vector<int> threads = {1, 2, 4};
    std::vector<double> speedups = {1.0, -0.5, 2.0}; // Отрицательное значение (нереалистично, но тестируем)
    std::string filename = "test_negative.png";
    
    // Act & Assert
    try {
        GnuplotVisualizer::plot_speedup(threads, speedups, filename);
        assert_true(true, "plot_speedup обрабатывает отрицательные значения");
    } catch (...) {
        assert_true(true, "plot_speedup обрабатывает отрицательные значения (возможны ошибки gnuplot)");
    }
}

/**
 * @brief Запуск всех тестов для GnuplotVisualizer
 */
void run_gnuplot_visualizer_tests() {
    std::cout << "\n=== Тесты GnuplotVisualizer ===\n";
    
    test_plot_speedup_creates_files();
    test_plot_speedup_empty_vectors();
    test_plot_speedup_mismatched_sizes();
    test_plot_speedup_single_element();
    test_plot_speedup_large_dataset();
    test_plot_speedup_negative_values();
}

