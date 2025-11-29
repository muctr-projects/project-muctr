#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <random>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <omp.h>
#include "core/newton_interpolator.h"
#include "core/gnuplot_visualizer.h"

/**
 * @brief Генерирует случайные точки для интерполяции
 */
std::vector<double> generate_random_points(double min, double max, size_t num_points) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(min, max);
    
    std::vector<double> points(num_points);
    for (size_t i = 0; i < num_points; i++) {
        points[i] = dist(gen);
    }
    return points;
}

/**
 * @brief Выбирает случайные точки из исходных узлов
 */
std::vector<std::pair<double, double>> select_random_nodes(const std::vector<double>& x_nodes, 
                                                         const std::vector<double>& y_nodes, 
                                                         size_t num_points) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, x_nodes.size() - 1);
    
    std::vector<std::pair<double, double>> selected_points;
    std::vector<bool> selected(x_nodes.size(), false);
    
    for (size_t i = 0; i < num_points && i < x_nodes.size(); i++) {
        size_t index;
        do {
            index = dist(gen);
        } while (selected[index]);
        
        selected[index] = true;
        selected_points.push_back({x_nodes[index], y_nodes[index]});
    }
    
    return selected_points;
}

/**
 * @brief Вычисляет абсолютную ошибку интерполяции
 */
double calculate_absolute_error(double original_y, double interpolated_y) {
    return std::abs(interpolated_y - original_y);
}

/**
 * @brief Измеряет время выполнения интерполяции
 */
double measure_execution_time(NewtonInterpolator& interpolator, 
                            const std::vector<double>& points, 
                            int num_threads) {
    omp_set_num_threads(num_threads);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<double> results = interpolator.interpolate_multiple(points);
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    
    return duration.count();
}

int main() {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    
    const double start_range = 0.0;
    const double end_range = 10.0;
    const double min_y = -10.0;
    const double max_y = 10.0;
    size_t num_nodes = 25;
    std::cout << "Тестирование интерполяции по " << num_nodes << " случайным точкам" << std::endl;

    for (int i = 0; i < 5; i++) {
        std::cout << "Итерация " << i+1 << ":" << std::endl;
        std::vector<double> x_nodes = generate_random_points(start_range, end_range, num_nodes);
        std::vector<double> y_nodes = generate_random_points(min_y, max_y, num_nodes);
        
        size_t num_interp_points = 10000;
        std::vector<int> thread_counts = {1, 2, 4, 6, 8, 12, 16};

        try {            
            NewtonInterpolator interpolator(x_nodes, y_nodes);
            
            std::vector<double> interp_points = NewtonInterpolator::generate_points(start_range, end_range, num_interp_points);
            
            std::vector<double> speedups;
            double single_thread_time = 0.0;
            
            for (int threads : thread_counts) {
                double time = measure_execution_time(interpolator, interp_points, threads);
                
                if (threads == 1) {
                    single_thread_time = time;
                }
                
                double speedup = single_thread_time / time;
                speedups.push_back(speedup);
                
                std::cout << "Потоки: " << threads << ", время: " << time 
                        << " мс, ускорение: " << speedup << std::endl;
            }
            
            std::string speedup_plot = "speedup_test_" + std::to_string(i+1) + ".png";
            try {
                GnuplotVisualizer::plot_speedup(thread_counts, speedups, speedup_plot);
                std::cout << "График сохранен: " << speedup_plot << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Предупреждение: не удалось создать график " << speedup_plot 
                        << ": " << e.what() << std::endl;
                std::cerr << "Продолжаем выполнение без графика..." << std::endl;
            }
            
            std::cout << "\nПроверка интерполяции в 5 случайных точках из исходных узлов:" << std::endl;
            
            std::vector<std::pair<double, double>> test_points = select_random_nodes(x_nodes, y_nodes, 5);
            std::vector<double> test_x;
            std::vector<double> true_y;
            
            for (const auto& point : test_points) {
                test_x.push_back(point.first);
                true_y.push_back(point.second);
            }
            
            std::vector<double> interpolated_results = interpolator.interpolate_multiple(test_x);
            
            std::cout << "==========================================================================" << std::endl;
            std::cout << "|      x       |  y истинное  | y интерполир. |   Ошибка    |" << std::endl;
            std::cout << "==========================================================================" << std::endl;
            
            for (size_t i = 0; i < test_x.size(); i++) {
                double x = test_x[i];
                double true_y_val = true_y[i];
                double interpolated_y = interpolated_results[i];
                double error = calculate_absolute_error(true_y_val, interpolated_y);
                
                std::cout << "| " << std::setw(12) << std::fixed << std::setprecision(6) << x 
                        << " | " << std::setw(12) << std::setprecision(6) << true_y_val
                        << " | " << std::setw(12) << std::setprecision(6) << interpolated_y
                        << " | " << std::setw(12) << std::setprecision(6) << error << " |" << std::endl;
            }
            std::cout << "==========================================================================\n\n" << std::endl;
            
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
            return 1;
        }
    }
    std::cout << "\nТестирование завершено." << std::endl;
    return 0;
}