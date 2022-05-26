#pragma once
#include <type_traits>
#include <functional>
#include <algorithm>
#include <iterator>

#include "vector.h"

template<typename F>
concept ParametricFunction = requires
{
	std::is_function_v<F>;
	std::is_floating_point_v<F()>;
};

template<FloatingPoint T, std::size_t N>
class parametric_function
{
private:
	using vector = vec<T, N>;
	std::array<std::function<T(T)>, N> functions;

public:
	template<ParametricFunction... Fs>
	explicit parametric_function(Fs... fs)
		: functions{fs...}
	{}

	vec<T, N> get_value_at(T t) const
	{
		std::array<T, N> value;
		for (std::size_t i = 0; i < N; ++i)
			value[i] = functions[i](t);
		return vector(value);
	}

	// symmetric derivative
	vec<T, N> get_derivative_at(T t, T dt = 0.00001) const
	{
		const T left = t - dt;
		const T right = t + dt;

		std::array<T, N> left_value, right_value;

		for (std::size_t i = 0; i < N; ++i)
		{
			left_value[i] = functions[i](left);
			right_value[i] = functions[i](right);
		}

		std::array<T, N> derivative;

		for (std::size_t i = 0; i < N; ++i)
		{
			derivative[i] = (right_value[i] - left_value[i]) / (2 * dt);
		}

		return vector(derivative);
	}

	std::vector<vector> get_point_projection(vector p, T t0, T t1, T e) const
	{
		// isolate roots
		const int n = 10;

		T h = (t1 - t0) / n;

		std::vector<std::pair<T, T>> isolation_interval;

		T tl = t0;
		T tr = t0 + h;

		auto dl = get_derivative_at(tl);

		//           dc
		// (p - c(t))-- = 0
		//           dt
		T el = (p - get_value_at(tl)) * dl;

		for (int i = 0; i < n; ++i)
		{
			auto dr = get_derivative_at(tr);
			T er = (p - get_value_at(tr)) * dr;

			// (el * er) < 0
			if (std::signbit(el) != std::signbit(er))
				isolation_interval.emplace_back(tl, tr);

			tl = tr;
			tr += h;

			dl = dr;
			el = er;
		}

		std::vector<vector> result;

		for (auto [ta, tb] : isolation_interval)
		{
			// bisection method
			T ea = (p - get_value_at(ta)) * get_derivative_at(ta);
			T eb = (p - get_value_at(tb)) * get_derivative_at(tb);

			while (true)
			{
				T ti = (tb + ta) / 2;
				auto vi = get_value_at(ti);
				T ei = (p - vi) * get_derivative_at(ti);

				if (std::abs(ei) < e)
				{
					result.emplace_back(vi);
					break;
				}

				// (ea * ei) < 0
				if (std::signbit(ea) != std::signbit(ei))
				{
					tb = ti;
					eb = ei;
				}
				else
				{
					ta = ti;
					ea = ei;
				}
			}
		}

		return result;
	}
};
