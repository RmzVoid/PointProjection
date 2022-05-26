#pragma once
#include <type_traits>
#include <array>
#include <iostream>
#include <numeric>
#include <sstream>

template <typename T>
concept FloatingPoint = std::is_floating_point<T>::value;

template <FloatingPoint T, std::size_t N>
class vec
{
private:
	using vector = vec<T, N>;
	std::array<T, N> v;

public:
	vec() = default;

	template<FloatingPoint... Ts>
	requires (sizeof...(Ts) == N)
	vec(Ts... t)
		: v{ t... }
	{}

	vec(const std::array<T, N>& a)
		: v{ a }
	{};

	vec(const std::array<T, N>&& a)
		: v{ a }
	{};

	vec(const vector& other)
		: v{ other.v }
	{}

	vec(vector&& other) noexcept
		: v{ other.v }
	{}

	vector& operator=(const vector& other)
	{
		v = other.v;
		return *this;
	}

	vector& operator=(const vector&& other)
	{
		v = other.v;
		return *this;
	}

	operator std::string() const
	{
		std::stringstream ss;
		ss << '(';

		for (std::size_t i = 0; i < N; ++i)
			ss << v[i] << (i != N - 1 ? ", " : "");

		ss << ')';
		return ss.str();
	}

	T operator[](std::size_t i) const { return v[i]; }
	T& operator[](std::size_t i) { return v[i]; }

	vector& operator+=(const vector& other)
	{
		for (std::size_t i = 0; i < N; ++i)
			v[i] += other.v[i];

		return *this;
	}

	vector& operator-=(const vector& other)
	{
		for (std::size_t i = 0; i < N; ++i)
			v[i] -= other.v[i];

		return *this;
	}

	T operator*(const vector& other)
	{
		return std::inner_product(v.begin(), v.end(), other.v.begin(), T{});
	}

	vector operator/(T d)
	{
		vec<T, N> result = *this;
		for (std::size_t i = 0; i < N; ++i)
			result.v[i] /= d;

		return result;
	}
};

template <FloatingPoint T, std::size_t N>
vec<T, N> operator+(const vec<T, N>& v0, const vec<T, N>& v1)
{
	vec<T, N> result = v0;
	result += v1;
	return result;
}

template <FloatingPoint T, std::size_t N>
vec<T, N> operator-(const vec<T, N>& v0, const vec<T, N>& v1)
{
	vec<T, N> result = v0;
	result -= v1;
	return result;
}