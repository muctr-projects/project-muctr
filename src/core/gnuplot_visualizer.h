/**
 * @file gnuplot_visualizer.h
 * @brief Класс для визуализации результатов с помощью Gnuplot
 */
#include <vector>
#include <string>
#include <fstream>

/**
 * @class GnuplotVisualizer
 * @brief Класс для создания графиков с помощью Gnuplot
 */
class GnuplotVisualizer {
public:
    /**
     * @brief Строит график ускорения
     * @param threads Вектор количества потоков
     * @param speedups Вектор значений ускорения
     * @param filename Имя файла для сохранения графика
     */
    static void plot_speedup(const std::vector<int>& threads, 
                           const std::vector<double>& speedups,
                           const std::string& filename);
    
private:
    /**
     * @brief Сохраняет данные в файл
     * @param x Вектор значений x
     * @param y Вектор значений y
     * @param filename Имя файла
     */
    static void save_data_to_file(const std::vector<double>& x, 
                                const std::vector<double>& y, 
                                const std::string& filename);
};