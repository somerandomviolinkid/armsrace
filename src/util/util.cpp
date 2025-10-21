#include "util.hpp"

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