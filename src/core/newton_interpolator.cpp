#include "newton_interpolator.h"
#include <omp.h>
#include <stdexcept>
#include <cmath>
#include <iostream>

NewtonInterpolator::NewtonInterpolator() {}

NewtonInterpolator::NewtonInterpolator(const std::vector<double>& x, const std::vector<double>& y) {
    set_data(x, y);
}

void NewtonInterpolator::set_data(const std::vector<double>& x, const std::vector<double>& y) {
    if (x.size() != y.size()) {
        throw std::invalid_argument("Размеры векторов x и y должны совпадать");
    }
    if (x.size() < 2) {
        throw std::invalid_argument("Необходимо как минимум 2 точки для интерполяции");
    }
    
    x_values = x;
    y_values = y;
    compute_divided_differences();
}

void NewtonInterpolator::compute_divided_differences() {
    int n = static_cast<int>(x_values.size());
    divided_differences.clear();
    divided_differences.resize(n, std::vector<double>(n, 0.0));

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        divided_differences[i][0] = y_values[i];
    }

    for (int j = 1; j < n; j++) {
        #pragma omp parallel for
        for (int i = 0; i < n - j; i++) {
            double denominator = x_values[i+j] - x_values[i];
            if (std::abs(denominator) < 1e-10) {
                #pragma omp critical
                {
                    throw std::runtime_error("Деление на ноль при вычислении разделенных разностей");
                }
            }
            divided_differences[i][j] = (divided_differences[i+1][j-1] - divided_differences[i][j-1]) / denominator;
        }
    }
}

double NewtonInterpolator::interpolate(double point) const {
    if (x_values.empty()) {
        throw std::runtime_error("Данные не инициализированы");
    }
    
    double result = divided_differences[0][0];
    double product_term = 1.0;
    
    for (size_t i = 1; i < x_values.size(); i++) {
        product_term *= (point - x_values[i-1]);
        result += divided_differences[0][i] * product_term;
    }
    
    return result;
}

std::vector<double> NewtonInterpolator::interpolate_multiple(const std::vector<double>& points) const {
    std::vector<double> results(points.size());
    int num_points = static_cast<int>(points.size());

    #pragma omp parallel for schedule(dynamic, 100)
    for (int i = 0; i < num_points; i++) {
        results[i] = interpolate(points[i]);
    }
    
    return results;
}

size_t NewtonInterpolator::get_node_count() const {
    return x_values.size();
}

std::vector<double> NewtonInterpolator::generate_points(double start, double end, size_t num_points) {
    if (num_points < 2) {
        throw std::invalid_argument("Количество точек должно быть не менее 2");
    }
    
    std::vector<double> points(num_points);
    double step = (end - start) / (num_points - 1);
    int n = static_cast<int>(num_points);
    
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        points[i] = start + i * step;
    }
    
    return points;
}