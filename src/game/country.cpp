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