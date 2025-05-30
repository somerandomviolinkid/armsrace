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

void industry::drawMenu() {
	int yOffset = 0;
	SDL_Rect r = {};
	textTag tag = { 1.0f, {0, 0, 0, 255}, LEFT, MIDDLE };

	for (std::pair<const uint32_t, float>& pair : inventory) {
		r = { ((state.res.x * 3) / 4) + 8, yOffset - 8, 16, 16 };
		drawRectOutline(r, { 255, 255, 255, 255 }, { 0, 0, 0, 255 });

		drawTexture(gameData.resourceDatas[ pair.first ].texture, { ((state.res.x * 3) / 4) + 8, yOffset }, 1.0f, LEFT, MIDDLE);

		drawText(makeTaggedText(std::format("{}:", gameData.resourceDatas[ pair.first ].name), { ((state.res.x * 3) / 4) + 32, yOffset }, tag));
		drawText(makeTaggedText(std::format("{:.2f} / {:.2f}", pair.second, gameData.industryDatas[ type ].storage[ pair.first ]), { ((state.res.x * 7) / 8) + 8, yOffset }, tag));
		yOffset += 24;
	}

	yOffset -= 8;
	SDL_RenderDrawLine(state.renderer, (state.res.x * 3) / 4, yOffset, state.res.x, yOffset);
	yOffset += 24;
}