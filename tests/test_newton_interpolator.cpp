/**
 * @file test_newton_interpolator.cpp
 * @brief Модульные тесты для класса NewtonInterpolator
 */

#include "test_newton_interpolator.h"
#include "test_utils.h"
#include "../src/core/newton_interpolator.h"
#include <cmath>
#include <stdexcept>

/**
 * @brief Тест: Конструктор по умолчанию
 * AAA: Arrange - создаем объект, Act - проверяем состояние, Assert - проверяем что данные пусты
 */
void test_default_constructor() {
    test_group("Конструктор по умолчанию");
    
    // Arrange
    NewtonInterpolator interpolator;
    
    // Act & Assert
    assert_true(interpolator.get_node_count() == 0, 
                "Конструктор по умолчанию создает пустой интерполятор");
}

/**
 * @brief Тест: Конструктор с параметрами (валидные данные)
 * AAA: Arrange - готовим данные, Act - создаем объект, Assert - проверяем корректность
 */
void test_constructor_with_valid_data() {
    test_group("Конструктор с валидными данными");
    
    // Arrange
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {1.0, 2.0, 3.0};
    
    // Act
    NewtonInterpolator interpolator(x, y);
    
    // Assert
    assert_true(interpolator.get_node_count() == 3, 
                "Конструктор корректно инициализирует количество узлов");
}

/**
 * @brief Тест: Конструктор с несовпадающими размерами векторов
 * AAA: Arrange - готовим невалидные данные, Act - пытаемся создать объект, Assert - проверяем исключение
 */
void test_constructor_with_mismatched_sizes() {
    test_group("Конструктор с несовпадающими размерами");
    
    // Arrange
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {1.0, 2.0}; // Разные размеры
    
    // Act & Assert
    assert_throws<std::invalid_argument>(
        [&]() { NewtonInterpolator interpolator(x, y); },
        "Конструктор выбрасывает исключение при несовпадающих размерах"
    );
}

/**
 * @brief Тест: Конструктор с недостаточным количеством точек
 * AAA: Arrange - готовим недостаточно данных, Act - пытаемся создать объект, Assert - проверяем исключение
 */
void test_constructor_with_insufficient_points() {
    test_group("Конструктор с недостаточным количеством точек");
    
    // Arrange
    std::vector<double> x = {0.0};
    std::vector<double> y = {1.0};
    
    // Act & Assert
    assert_throws<std::invalid_argument>(
        [&]() { NewtonInterpolator interpolator(x, y); },
        "Конструктор выбрасывает исключение при менее чем 2 точках"
    );
}

/**
 * @brief Тест: set_data с валидными данными
 * AAA: Arrange - создаем пустой объект, Act - устанавливаем данные, Assert - проверяем результат
 */
void test_set_data_valid() {
    test_group("set_data с валидными данными");
    
    // Arrange
    NewtonInterpolator interpolator;
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {1.0, 4.0, 9.0};
    
    // Act
    interpolator.set_data(x, y);
    
    // Assert
    assert_true(interpolator.get_node_count() == 3, 
                "set_data корректно устанавливает количество узлов");
}

/**
 * @brief Тест: set_data с невалидными данными
 * AAA: Arrange - создаем объект, Act - пытаемся установить невалидные данные, Assert - проверяем исключение
 */
void test_set_data_invalid() {
    test_group("set_data с невалидными данными");
    
    // Arrange
    NewtonInterpolator interpolator;
    std::vector<double> x = {0.0, 1.0};
    std::vector<double> y = {1.0, 2.0, 3.0}; // Разные размеры
    
    // Act & Assert
    assert_throws<std::invalid_argument>(
        [&]() { interpolator.set_data(x, y); },
        "set_data выбрасывает исключение при несовпадающих размерах"
    );
}

/**
 * @brief Тест: Интерполяция линейной функции (точное совпадение)
 * AAA: Arrange - готовим данные линейной функции, Act - интерполируем, Assert - проверяем точность
 */
void test_interpolate_linear_function() {
    test_group("Интерполяция линейной функции");
    
    // Arrange - линейная функция y = 2x + 1
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y = {1.0, 3.0, 5.0, 7.0};
    NewtonInterpolator interpolator(x, y);
    
    // Act
    double result = interpolator.interpolate(1.5);
    
    // Assert - для линейной функции интерполяция должна быть точной
    double expected = 2.0 * 1.5 + 1.0; // 4.0
    assert_double_equal(result, expected, 1e-9, 
                       "Интерполяция линейной функции дает точный результат");
}

/**
 * @brief Тест: Интерполяция в узлах (должна возвращать исходные значения)
 * AAA: Arrange - готовим данные, Act - интерполируем в узлах, Assert - проверяем точное совпадение
 */
void test_interpolate_at_nodes() {
    test_group("Интерполяция в узлах");
    
    // Arrange
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {1.0, 4.0, 9.0};
    NewtonInterpolator interpolator(x, y);
    
    // Act & Assert - интерполяция в узлах должна давать точные значения
    for (size_t i = 0; i < x.size(); i++) {
        double result = interpolator.interpolate(x[i]);
        assert_double_equal(result, y[i], 1e-9, 
                           "Интерполяция в узле " + std::to_string(i) + " возвращает исходное значение");
    }
}

/**
 * @brief Тест: Интерполяция квадратичной функции
 * AAA: Arrange - готовим данные квадратичной функции, Act - интерполируем, Assert - проверяем точность
 */
void test_interpolate_quadratic_function() {
    test_group("Интерполяция квадратичной функции");
    
    // Arrange - квадратичная функция y = x^2
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y = {0.0, 1.0, 4.0, 9.0};
    NewtonInterpolator interpolator(x, y);
    
    // Act
    double result = interpolator.interpolate(1.5);
    
    // Assert - для квадратичной функции интерполяция должна быть точной
    double expected = 1.5 * 1.5; // 2.25
    assert_double_equal(result, expected, 1e-9, 
                       "Интерполяция квадратичной функции дает точный результат");
}

/**
 * @brief Тест: Интерполяция без инициализации данных
 * AAA: Arrange - создаем объект без данных, Act - пытаемся интерполировать, Assert - проверяем исключение
 */
void test_interpolate_without_data() {
    test_group("Интерполяция без инициализации данных");
    
    // Arrange
    NewtonInterpolator interpolator;
    
    // Act & Assert
    assert_throws<std::runtime_error>(
        [&]() { interpolator.interpolate(1.0); },
        "Интерполяция без данных выбрасывает исключение"
    );
}

/**
 * @brief Тест: Интерполяция нескольких точек
 * AAA: Arrange - готовим данные и точки, Act - интерполируем, Assert - проверяем размер и корректность
 */
void test_interpolate_multiple() {
    test_group("Интерполяция нескольких точек");
    
    // Arrange
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {1.0, 2.0, 3.0};
    NewtonInterpolator interpolator(x, y);
    std::vector<double> points = {0.0, 0.5, 1.0, 1.5, 2.0};
    
    // Act
    std::vector<double> results = interpolator.interpolate_multiple(points);
    
    // Assert
    assert_true(results.size() == points.size(), 
                "interpolate_multiple возвращает правильное количество результатов");
    
    // Проверяем, что результаты в узлах точные
    assert_double_equal(results[0], 1.0, 1e-9, "Результат в первом узле корректен");
    assert_double_equal(results[2], 2.0, 1e-9, "Результат во втором узле корректен");
    assert_double_equal(results[4], 3.0, 1e-9, "Результат в третьем узле корректен");
}

/**
 * @brief Тест: Интерполяция пустого вектора точек
 * AAA: Arrange - готовим данные и пустой вектор, Act - интерполируем, Assert - проверяем пустой результат
 */
void test_interpolate_multiple_empty() {
    test_group("Интерполяция пустого вектора точек");
    
    // Arrange
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {1.0, 2.0, 3.0};
    NewtonInterpolator interpolator(x, y);
    std::vector<double> points;
    
    // Act
    std::vector<double> results = interpolator.interpolate_multiple(points);
    
    // Assert
    assert_true(results.empty(), 
                "interpolate_multiple возвращает пустой вектор для пустого входа");
}

/**
 * @brief Тест: generate_points с валидными параметрами
 * AAA: Arrange - задаем параметры, Act - генерируем точки, Assert - проверяем корректность
 */
void test_generate_points_valid() {
    test_group("generate_points с валидными параметрами");
    
    // Arrange
    double start = 0.0;
    double end = 10.0;
    size_t num_points = 11;
    
    // Act
    std::vector<double> points = NewtonInterpolator::generate_points(start, end, num_points);
    
    // Assert
    assert_true(points.size() == num_points, 
                "generate_points создает правильное количество точек");
    assert_double_equal(points[0], start, 1e-9, "Первая точка равна start");
    assert_double_equal(points.back(), end, 1e-9, "Последняя точка равна end");
    
    // Проверяем равномерность распределения
    double expected_step = (end - start) / (num_points - 1);
    for (size_t i = 1; i < points.size(); i++) {
        double actual_step = points[i] - points[i-1];
        assert_double_equal(actual_step, expected_step, 1e-9, 
                           "Точки распределены равномерно (шаг " + std::to_string(i) + ")");
    }
}

/**
 * @brief Тест: generate_points с недостаточным количеством точек
 * AAA: Arrange - задаем невалидные параметры, Act - пытаемся генерировать, Assert - проверяем исключение
 */
void test_generate_points_insufficient() {
    test_group("generate_points с недостаточным количеством точек");
    
    // Arrange
    double start = 0.0;
    double end = 10.0;
    size_t num_points = 1;
    
    // Act & Assert
    assert_throws<std::invalid_argument>(
        [&]() { NewtonInterpolator::generate_points(start, end, num_points); },
        "generate_points выбрасывает исключение при менее чем 2 точках"
    );
}

/**
 * @brief Тест: generate_points с одинаковыми start и end
 * AAA: Arrange - задаем start == end, Act - генерируем точки, Assert - проверяем что все точки одинаковы
 */
void test_generate_points_same_start_end() {
    test_group("generate_points с одинаковыми start и end");
    
    // Arrange
    double start = 5.0;
    double end = 5.0;
    size_t num_points = 5;
    
    // Act
    std::vector<double> points = NewtonInterpolator::generate_points(start, end, num_points);
    
    // Assert
    assert_true(points.size() == num_points, 
                "generate_points создает правильное количество точек при start == end");
    
    for (size_t i = 0; i < points.size(); i++) {
        assert_double_equal(points[i], start, 1e-9, 
                           "Все точки равны start при start == end (точка " + std::to_string(i) + ")");
    }
}

/**
 * @brief Тест: Интерполяция функции sin(x) с высокой точностью
 * AAA: Arrange - готовим данные sin(x), Act - интерполируем, Assert - проверяем точность
 */
void test_interpolate_sin_function() {
    test_group("Интерполяция функции sin(x)");
    
    // Arrange - sin(x) на интервале [0, π]
    std::vector<double> x = {0.0, M_PI/4, M_PI/2, 3*M_PI/4, M_PI};
    std::vector<double> y;
    for (double xi : x) {
        y.push_back(std::sin(xi));
    }
    NewtonInterpolator interpolator(x, y);
    
    // Act
    double test_point = M_PI/6; // 30 градусов
    double result = interpolator.interpolate(test_point);
    
    // Assert - для полинома 4-й степени точность должна быть разумной
    // Для не-полиномиальной функции точность интерполяции ограничена
    double expected = std::sin(test_point);
    assert_double_equal(result, expected, 1e-3, 
                       "Интерполяция sin(x) дает разумно точный результат");
}

/**
 * @brief Тест: get_node_count после различных операций
 * AAA: Arrange - создаем объект, Act - выполняем операции, Assert - проверяем счетчик узлов
 */
void test_get_node_count() {
    test_group("get_node_count");
    
    // Arrange & Act
    NewtonInterpolator interpolator1;
    assert_true(interpolator1.get_node_count() == 0, 
                "get_node_count возвращает 0 для пустого интерполятора");
    
    // Arrange & Act
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0};
    std::vector<double> y = {1.0, 2.0, 3.0, 4.0, 5.0};
    NewtonInterpolator interpolator2(x, y);
    
    // Assert
    assert_true(interpolator2.get_node_count() == 5, 
                "get_node_count возвращает правильное количество узлов");
}

/**
 * @brief Тест: Интерполяция за пределами узлов (экстраполяция)
 * AAA: Arrange - готовим данные, Act - интерполируем вне диапазона, Assert - проверяем что не падает
 */
void test_interpolate_extrapolation() {
    test_group("Интерполяция за пределами узлов (экстраполяция)");
    
    // Arrange
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {1.0, 2.0, 3.0};
    NewtonInterpolator interpolator(x, y);
    
    // Act - экстраполяция влево
    double result_left = interpolator.interpolate(-1.0);
    
    // Act - экстраполяция вправо
    double result_right = interpolator.interpolate(3.0);
    
    // Assert - метод не должен падать, но результат может быть неточным
    assert_true(std::isfinite(result_left), 
                "Экстраполяция влево возвращает конечное значение");
    assert_true(std::isfinite(result_right), 
                "Экстраполяция вправо возвращает конечное значение");
}

/**
 * @brief Запуск всех тестов для NewtonInterpolator
 */
void run_newton_interpolator_tests() {
    std::cout << "\n=== Тесты NewtonInterpolator ===\n";
    
    test_default_constructor();
    test_constructor_with_valid_data();
    test_constructor_with_mismatched_sizes();
    test_constructor_with_insufficient_points();
    test_set_data_valid();
    test_set_data_invalid();
    test_interpolate_linear_function();
    test_interpolate_at_nodes();
    test_interpolate_quadratic_function();
    test_interpolate_without_data();
    test_interpolate_multiple();
    test_interpolate_multiple_empty();
    test_generate_points_valid();
    test_generate_points_insufficient();
    test_generate_points_same_start_end();
    test_interpolate_sin_function();
    test_get_node_count();
    test_interpolate_extrapolation();
}

