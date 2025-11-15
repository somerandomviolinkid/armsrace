#include "game.hpp"

int countryPopulation(int i) {
	int res = 0;
	for (city& c : game.cities) {
		if (c.owner == i) {
			res += c.population;
		}
	}

	return res;
}

void country::drawMenu(int i) {
	drawRect(v2ToRect({ (state.res.x * 3) / 4, 64 }, { state.res.x / 4, state.res.y - 64 }), { 0, 0, 0, 255 }, { 192, 192, 192, 255 });

	int yOffset = 96;
	float scale = name.length() > 18 ? 2.0f : 3.0f;
	drawText(name, { (state.res.x * 7) / 8, yOffset }, scale, color, MIDDLE, CENTER);

	yOffset += 48;
	drawText("Population: ", { ((state.res.x * 3) / 4) + 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
	drawText(formatNumber((float)countryPopulation(i)), { state.res.x - 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);
}

v2<float> country::getCenter(int i) {
	v2<float> sum = { 0.0f, 0.0f };
	int count = 0;
	for (const city& c : game.cities) {
		if (c.owner != i) {
			continue;
		}

		sum = sum + c.pos;
		count++;
	}

	return sum / (float)count;
}

v2<float> country::getPopulationCenter(int i) {
	v2<float> sum = { 0.0f, 0.0f };
	int count = 0;

	for (const city& c : game.cities) {
		if (c.owner != i) {
			continue;
		}

		sum = sum + (c.pos * (float)c.population);
		count += c.population;
	}

	return sum / (float)count;
}

v2<float> country::getWesternMost(int i) {
	v2<float> pos = { 0.0f, 0.0f };
	int count = 0;
	for (const city& c : game.cities) {
		if (c.owner != i) {
			continue;
		}

		if (count == 0) {
			pos = c.pos;
		}

		if (c.pos.x < pos.x) {
			pos = c.pos;
		}

		count++;
	}

	return pos;
}

v2<float> country::getEasternMost(int i) {
	v2<float> pos = { 0.0f, 0.0f };
	int count = 0;
	for (const city& c : game.cities) {
		if (c.owner != i) {
			continue;
		}

		if (count == 0) {
			pos = c.pos;
		}

		if (c.pos.x > pos.x) {
			pos = c.pos;
		}

		count++;
	}

	return pos;
}

float country::getLength(int i) {
	v2<float> min = getWesternMost(i);
	v2<float> max = getEasternMost(i);
	return max.x - min.x;
}