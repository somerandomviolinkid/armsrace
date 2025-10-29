#include "util.hpp"

v2<float> makeVector(float direction) {
	return { sinf(direction), cosf(direction) };
}

v2<float> makeVector(float direction, float magnitude) {
	return makeVector(direction) * magnitude;
}

v2<float> makeVector(v2<float> pos, float direction, float magnitude) {
	return makeVector(direction, magnitude) + pos;
}

v2<int> v2fTov2i(v2<float> v) {
	return { (int)v.x, (int)v.y };
}

v2<float> v2iTov2f(v2<int> v) {
	return { (float)v.x, (float)v.y };
}

float dist2d(v2<float> a, v2<float> b) {
	return std::sqrtf(std::powf(b.x - a.x, 2.0f) + std::powf(b.y - a.y, 2.0f));
}

std::string formatNumber(float number) {
	std::string suffixes[4] = { "", "K", "M", "B" };
	int digits = (int)log10f(number);
	int suffix = std::clamp(digits / 3, 0, 3);
	return std::format("{:.3f}", number / std::powf(10.0f, (float)suffix * 3.0f)) + suffixes[suffix];
}

std::string makePNGFilePath(std::string name) {
	name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
	name[0] = std::tolower(name[0]);
	name = "assets/textures/" + name + ".png";
	return name;
}

float randf(std::mt19937& gen, float min, float max) {
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}