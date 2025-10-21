#pragma once
#include <string>
#include <format>
#include <algorithm>
#include <random>

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

//string utils

std::string formatNumber(float number);

std::string makePNGFilePath(std::string name);

//random

float randf(std::mt19937& gen, float min, float max);