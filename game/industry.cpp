#include "game.hpp"

void industry::tick() {
	float prodEfficiency = (float)workers / (float)gameData.industryDatas[ type ].maxWorkers;

	for (const std::pair<uint32_t, float> pair : gameData.industryDatas[ type ].inputs) {
		if (inventory[pair.first] * prodEfficiency < pair.second * prodEfficiency) {
			return;
		}
	}

	for (const std::pair<uint32_t, float> pair : gameData.industryDatas[ type ].outputs) {
		inventory[ pair.first ] += prodEfficiency * pair.second;
	}

	for (const std::pair<uint32_t, float> pair : gameData.industryDatas[ type ].inputs) {
		inventory[ pair.first ] -= prodEfficiency * pair.second;
	}
}