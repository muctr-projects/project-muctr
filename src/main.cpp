#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <omp.h>
#include "core/newton_interpolator.h"
#include "core/gnuplot_visualizer.h"

/**
 * @brief Тестовые функции для интерполяции
 */
double test_function_1(double x) {
    return std::sin(x) + std::cos(2 * x);
}

double test_function_2(double x) {
    return x * x - 2 * x + 1;
}

double test_function_3(double x) {
    return std::exp(-x) * std::sin(3 * x);
}

double test_function_4(double x) {
    return std::sqrt(x + 0.5);
}

double test_function_5(double x) {
    return std::tanh(x) * std::cos(5 * x);
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

    std::cout << "Тестирование ускорения параллельной интерполяции..." << std::endl;
    std::vector<double> x_nodes = {0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0};
    double start = 0.0;
    double end = 4.0;
    size_t num_interp_points = 10000;
    std::vector<int> thread_counts = {1, 2, 4, 6, 8, 12, 16};

    std::vector<std::pair<std::string, double(*)(double)>> test_functions = {
        {"function_1", test_function_1},
        {"function_2", test_function_2},
        {"function_3", test_function_3},
        {"function_4", test_function_4},
        {"function_5", test_function_5}
    };

    try {
        for (const auto& test_func : test_functions) {
            std::string test_name = test_func.first;
            auto function = test_func.second;
            
            std::cout << "\nТестирование функции: " << test_name << std::endl;
            
            std::vector<double> y_nodes;
            for (double x : x_nodes) {
                y_nodes.push_back(function(x));
            }
            
            NewtonInterpolator interpolator(x_nodes, y_nodes);
            
            std::vector<double> interp_points = NewtonInterpolator::generate_points(start, end, num_interp_points);
            
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
            
            std::string speedup_plot = "speedup_" + test_name + ".png";
            GnuplotVisualizer::plot_speedup(thread_counts, speedups, speedup_plot);
            std::cout << "График сохранен: " << speedup_plot << std::endl;
        }
        
        std::cout << "\nВсе графики ускорения созданы:" << std::endl;
        for (const auto& test_func : test_functions) {
            std::cout << "speedup_" << test_func.first << ".png" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}