/**
 * @file newton_interpolator.h
 * @brief Класс для интерполяции функции методом Ньютона
 */
#include <vector>
#include <string>

/**
 * @class NewtonInterpolator
 * @brief Класс для выполнения интерполяции методом Ньютона с поддержкой OpenMP
 */
class NewtonInterpolator {
private:
    std::vector<double> x_values;  ///< Значения x из таблицы
    std::vector<double> y_values;  ///< Значения y из таблицы
    std::vector<std::vector<double>> divided_differences;  ///< Таблица разделенных разностей
    
    /**
     * @brief Вычисляет разделенные разности для интерполяции
     */
    void compute_divided_differences();
    
public:
    /**
     * @brief Конструктор по умолчанию
     */
    NewtonInterpolator();
    
    /**
     * @brief Конструктор с инициализацией данными
     * @param x Вектор значений x
     * @param y Вектор значений y
     */
    NewtonInterpolator(const std::vector<double>& x, const std::vector<double>& y);
    
    /**
     * @brief Устанавливает данные для интерполяции
     * @param x Вектор значений x
     * @param y Вектор значений y
     */
    void set_data(const std::vector<double>& x, const std::vector<double>& y);
    
    /**
     * @brief Выполняет интерполяцию в заданной точке
     * @param point Точка для интерполяции
     * @return Интерполированное значение
     */
    double interpolate(double point) const;
    
    /**
     * @brief Выполняет интерполяцию в нескольких точках с использованием OpenMP
     * @param points Вектор точек для интерполяции
     * @return Вектор интерполированных значений
     */
    std::vector<double> interpolate_multiple(const std::vector<double>& points) const;
    
    /**
     * @brief Возвращает количество узлов интерполяции
     * @return Количество узлов
     */
    size_t get_node_count() const;
    
    /**
     * @brief Генерирует равномерно распределенные точки на интервале
     * @param start Начало интервала
     * @param end Конец интервала
     * @param num_points Количество точек
     * @return Вектор сгенерированных точек
     */
    static std::vector<double> generate_points(double start, double end, size_t num_points);
};