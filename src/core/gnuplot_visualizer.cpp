#include "gnuplot_visualizer.h"
#include <cstdlib>
#include <stdexcept>
#include <algorithm>

void GnuplotVisualizer::save_data_to_file(const std::vector<double>& x, 
                                        const std::vector<double>& y, 
                                        const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + filename);
    }
    
    for (size_t i = 0; i < x.size(); i++) {
        file << x[i] << " " << y[i] << "\n";
    }
    file.close();
}

void GnuplotVisualizer::plot_speedup(const std::vector<int>& threads, 
                                   const std::vector<double>& speedups,
                                   const std::string& filename) {
    if (threads.empty() || speedups.empty()) {
        throw std::invalid_argument("Векторы threads и speedups не могут быть пустыми");
    }
    
    if (threads.size() != speedups.size()) {
        throw std::invalid_argument("Размеры векторов threads и speedups должны совпадать");
    }
    
    std::ofstream data_file("speedup_data.txt");
    for (size_t i = 0; i < threads.size(); i++) {
        data_file << threads[i] << " " << speedups[i] << "\n";
    }
    data_file.close();
    
    std::ofstream script("plot_speedup.gp");
    if (!script.is_open()) {
        remove("speedup_data.txt");
        throw std::runtime_error("Не удалось создать файл скрипта gnuplot");
    }
    
    script << "set terminal pngcairo size 1200,800 enhanced font 'Arial,12'\n";
    script << "set output '" << filename << "'\n";
    script << "set title 'Зависимость ускорения многопоточной программы по сравнению с 1 потоком'\n";
    script << "set xlabel 'Количество потоков'\n";
    script << "set ylabel 'Ускорение'\n";
    script << "set grid\n";
    script << "set key top left\n";
    script << "set xrange [0:" << threads.back() << "]\n";
    script << "set yrange [0:" << threads.back() << "]\n";
    script << "plot 'speedup_data.txt' with linespoints pt 7 ps 2 lc rgb 'red' lw 2 title 'Измеренное ускорение', \\\n";
    script << "     x with lines lw 2 lc rgb 'blue' title 'Линейное ускорение'\n";
    script.close();
    
#ifdef _WIN32
    int result = system("gnuplot plot_speedup.gp 2>nul");
#else
    int result = system("gnuplot plot_speedup.gp 2>/dev/null");
#endif
    if (result != 0) {
        remove("speedup_data.txt");
        remove("plot_speedup.gp");
        throw std::runtime_error("Ошибка при выполнении gnuplot. Убедитесь, что gnuplot установлен в системе.");
    }
    
    remove("speedup_data.txt");
    remove("plot_speedup.gp");
}