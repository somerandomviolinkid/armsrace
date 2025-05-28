#pragma once
#include <cmath>

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
v2<T> operator*(v2<T> a, U b) {
	return { (T)((float)a.x * (float)b), (T)((float)a.y * (float)b) };
}

template <typename T, typename U>
v2<T> operator/(v2<T> a, U b) {
	return { (T)((float)a.x / (float)b), (T)((float)a.y / (float)b) };
}

template <typename T>
float dist2d(v2<T> a, v2<T> b) {
	return sqrtf(powf((float)b.x - (float)a.x, 2.0f) + powf((float)b.y - (float)a.y, 2.0f));
}