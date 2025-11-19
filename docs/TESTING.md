# Документация по модульным тестам

## Обзор

Проект содержит комплексный набор модульных тестов для классов `NewtonInterpolator` и `GnuplotVisualizer`. Тесты написаны без использования внешних фреймворков, используя только стандартную библиотеку C++.

## Структура тестов

```
tests/
├── test_utils.h              # Утилиты для тестирования (заголовочный файл)
├── test_utils.cpp            # Реализация утилит
├── test_newton_interpolator.h    # Объявления тестов для NewtonInterpolator
├── test_newton_interpolator.cpp  # Тесты для NewtonInterpolator
├── test_gnuplot_visualizer.h      # Объявления тестов для GnuplotVisualizer
├── test_gnuplot_visualizer.cpp   # Тесты для GnuplotVisualizer
└── test_main.cpp             # Главный файл для запуска всех тестов
```

## Запуск тестов

### Компиляция тестов

```bash
make test
```

Эта команда компилирует все тестовые файлы и создает исполняемый файл `build/test.exe`.

### Запуск тестов

```bash
make run-tests
```

Или напрямую:

```bash
./build/test.exe
```

### Очистка

```bash
make clean
```

## Принципы тестирования

Все тесты следуют следующим принципам:

### 1. AAA (Arrange-Act-Assert)

Каждый тест структурирован в три части:

- **Arrange** (Подготовка) - настройка тестовых данных и окружения
- **Act** (Действие) - выполнение тестируемой операции
- **Assert** (Проверка) - проверка результата

**Пример:**
```cpp
void test_interpolate_linear_function() {
    // Arrange - готовим данные линейной функции
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y = {1.0, 3.0, 5.0, 7.0};
    NewtonInterpolator interpolator(x, y);
    
    // Act - интерполируем
    double result = interpolator.interpolate(1.5);
    
    // Assert - проверяем результат
    double expected = 2.0 * 1.5 + 1.0; // 4.0
    assert_double_equal(result, expected, 1e-9, 
                       "Интерполяция линейной функции дает точный результат");
}
```

### 2. Атомарность

Каждый тест проверяет одну конкретную функциональность или сценарий использования.

### 3. Модульность

Тесты разделены по классам и функциональным областям:
- Тесты для `NewtonInterpolator` в отдельном файле
- Тесты для `GnuplotVisualizer` в отдельном файле

### 4. Целевая проверка

Каждый тест имеет четкую цель и проверяет конкретное поведение системы.

## Утилиты для тестирования

### `assert_true(condition, message)`

Проверяет булево условие.

```cpp
assert_true(interpolator.get_node_count() == 3, 
            "Конструктор корректно инициализирует количество узлов");
```

### `assert_double_equal(actual, expected, epsilon, message)`

Проверяет равенство чисел с плавающей точкой с заданной точностью.

```cpp
assert_double_equal(result, expected, 1e-9, 
                   "Интерполяция дает точный результат");
```

### `assert_throws<ExceptionType>(func, message)`

Проверяет, что функция выбрасывает исключение определенного типа.

```cpp
assert_throws<std::invalid_argument>(
    [&]() { NewtonInterpolator interpolator(x, y); },
    "Конструктор выбрасывает исключение при несовпадающих размерах"
);
```

### `assert_vector_equal(actual, expected, epsilon, message)`

Проверяет равенство двух векторов с заданной точностью.

```cpp
assert_vector_equal(results, expected, 1e-9, 
                   "Векторы результатов совпадают");
```

### `test_group(name)`

Выводит заголовок группы тестов для лучшей читаемости вывода.

```cpp
test_group("Интерполяция линейной функции");
```

## Тесты для NewtonInterpolator

### Конструкторы

- `test_default_constructor()` - проверка конструктора по умолчанию
- `test_constructor_with_valid_data()` - конструктор с валидными данными
- `test_constructor_with_mismatched_sizes()` - обработка несовпадающих размеров
- `test_constructor_with_insufficient_points()` - обработка недостаточного количества точек

### Установка данных

- `test_set_data_valid()` - установка валидных данных
- `test_set_data_invalid()` - обработка невалидных данных

### Интерполяция

- `test_interpolate_linear_function()` - интерполяция линейной функции (точная)
- `test_interpolate_quadratic_function()` - интерполяция квадратичной функции (точная)
- `test_interpolate_at_nodes()` - интерполяция в узлах (должна возвращать исходные значения)
- `test_interpolate_sin_function()` - интерполяция функции sin(x) (приближенная)
- `test_interpolate_without_data()` - обработка интерполяции без инициализации
- `test_interpolate_extrapolation()` - экстраполяция за пределами узлов

### Множественная интерполяция

- `test_interpolate_multiple()` - интерполяция нескольких точек
- `test_interpolate_multiple_empty()` - обработка пустого вектора точек

### Утилиты

- `test_generate_points_valid()` - генерация точек с валидными параметрами
- `test_generate_points_insufficient()` - обработка недостаточного количества точек
- `test_generate_points_same_start_end()` - обработка случая start == end
- `test_get_node_count()` - проверка счетчика узлов

## Тесты для GnuplotVisualizer

### Визуализация

- `test_plot_speedup_creates_files()` - создание файлов данных и скрипта
- `test_plot_speedup_empty_vectors()` - обработка пустых векторов
- `test_plot_speedup_mismatched_sizes()` - обработка несовпадающих размеров
- `test_plot_speedup_single_element()` - обработка одного элемента
- `test_plot_speedup_large_dataset()` - обработка большого набора данных
- `test_plot_speedup_negative_values()` - обработка отрицательных значений

**Примечание:** Тесты для `GnuplotVisualizer` могут не выполнять фактическое построение графиков, если gnuplot не установлен в системе. Это нормальное поведение - тесты проверяют, что метод не падает с критическими ошибками.

## Статистика тестов

После запуска тестов выводится статистика:

```
