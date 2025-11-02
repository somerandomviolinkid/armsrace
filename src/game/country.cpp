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