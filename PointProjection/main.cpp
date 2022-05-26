#include <functional>
#include <numbers>
#include <iostream>
#include <cmath>
#include <string>

#include "vector.h"
#include "parametric_function.h"

template <typename T, std::size_t N>
void fun(const std::string& name, 
    const parametric_function<T, N> ft,
    const vec<T, N> p0,
    const std::pair<T, T> range,
    const double epsilon = 0.000001);

int main()
{
    const auto pi = std::numbers::pi;

    const auto circle = parametric_function<double, 2>{
        [](double t) { return 2 * std::cos(t); },
        [](double t) { return 2 * std::sin(t); }};

    const auto ellipse = parametric_function<double, 2>{
        [](double t) { return 3 * std::cos(t); },
        [](double t) { return 2 * std::sin(t); }};

    const auto line = parametric_function<double, 2>{
        [](double t) { return 3 + 2 * t;   },
        [](double t) { return 2 - 0.5 * t; }};

    const auto limacon = parametric_function<double, 2>{
        [](double t) { return 2*3*std::cos(t) - std::cos(2*t); },
        [](double t) { return 2*3*std::sin(t) - std::sin(2*t); }};

    fun("circle",  circle,  {1.0, 1.0}, {-pi,  pi});
    fun("ellipse", ellipse, {1.0, 1.0}, {-pi,  pi});
    fun("line",    line,    {1.0, 1.0}, { 0.0, 1.0});
    fun("line",    line,    {3.5, 1.0}, { 0.0, 1.0});
    fun("limacon", limacon, {2.,  2.},  {-pi,  pi});
}

template <typename T, std::size_t N>
void fun(const std::string& name,
    const parametric_function<T, N> ft,
    const vec<T, N> p0,
    const std::pair<T, T> range,
    const double epsilon)
{
    const auto [from, to] = range;
    const auto results = ft.get_point_projection(p0, from, to, epsilon);

    std::cout << "Projection(s) of point " << std::string(p0)
        << " to " << name << ":\n";
    if (results.empty())
        std::cout << "\tno solution" << '\n';
    else
        for (const auto& projections : results)
            std::cout << "\t" << std::string(projections) << '\n';

    std::cout << std::endl;
};
