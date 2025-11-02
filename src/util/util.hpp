#pragma once
#include <string>
#include <format>
#include <algorithm>
#include <random>
#include <fstream>
#include <sstream>

//vec2

template <typename T>
struct v2 {
	T x, y;
};

template <typename T>
v2<T> operator+(v2<T> a, v2<T> b) {
	return { a.x + b.x, a.y + b.y };
}

template <typename T>
v2<T> operator-(v2<T> a, v2<T> b) {
	return { a.x - b.x, a.y - b.y };
}

template <typename T, typename U>
v2<T> operator*(v2<T> v, U s) {
	return { (T)((float)v.x * (float)s), (T)((float)v.y * (float)s) };
}

template <typename T, typename U>
v2<T> operator/(v2<T> v, U s) {
	return { (T)((float)v.x / (float)s), (T)((float)v.y / (float)s) };
}

v2<int> v2fTov2i(v2<float> v);
v2<float> v2iTov2f(v2<int> v);

float dist2d(v2<float> a, v2<float> b);

v2<float> makeVector(float direction);
v2<float> makeVector(float direction, float magnitude);
v2<float> makeVector(v2<float> pos, float direction, float magnitude);

//string utils

std::string formatNumber(float number);

std::string makePNGFilePath(std::string name);

//file utils
//removes last byte of text file
void removeLastByte(std::string path, int count);

//random

float randf(std::mt19937& gen, float min, float max);

// number stuff

// automatically flips m0 and m1 if they are mismatched
template <typename T>
bool isInRange(T n, T m0, T m1) {
	float min = (float)m0;
	float max = (float)m1;

	if (m0 > m1) {
		min = (float)m1;
		max = (float)m0;
	}

	//true if n is equal to both
	float errorMargin = 0.0001f;
	if (fabsf(n - min) < errorMargin && fabsf(n - max)) {
		return true;
	}

	return (n >= min) && (n <= max);
}