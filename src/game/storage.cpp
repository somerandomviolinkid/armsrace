#include "game.hpp"

void storage::tick() {

}

void storage::drawMenu() {
	drawRect(v2ToRect({ (state.res.x * 3) / 4, 64 }, { state.res.x / 4, state.res.y - 64}), { 0, 0, 0, 255 }, { 192, 192, 192, 255 });

	int yOffset = 96;
	float scale = gameData.storageDatas[type].name.length() > 18 ? 2.0f : 3.0f;
	drawText(gameData.storageDatas[type].name, { (state.res.x * 7) / 8, yOffset }, scale, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	yOffset += 48;
	drawText("Capacity Filled:", { state.res.x * 3 / 4 + 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
	drawText(std::format("{:.1f}%", ratioStored() * 100.0f), { state.res.x - 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);

	yOffset += 24;
	SDL_Rect inventoryHeaderRect = v2ToRect({ (state.res.x * 3) / 4 , yOffset }, { state.res.x / 4, 64 });
	drawRect(inventoryHeaderRect, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 255 });
	if (mouseInRect(inventoryHeaderRect) && state.mouseState.click) {
		if (game.storageInventoryMenuOpen) {
			game.storageInventoryMenuOpen = false;
		} else {
			game.storageInventoryMenuOpen = true;
		}
	}

	yOffset += 32;
	drawText("Inventory", { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	if (game.storageInventoryMenuOpen) {
		drawTexture(state.baseTextures[MENU_OPEN], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	} else {
		drawTexture(state.baseTextures[MENU_CLOSED], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	}

	if (game.storageInventoryMenuOpen) {
		yOffset += 40;
		for (const std::pair<int, float>& pair : inventory) {
			SDL_Rect outline = v2ToRect({ (state.res.x * 3) / 4 + 8, yOffset }, { 32, 32 });
			drawRect(outline, { 0, 0, 0, 255 }, { 255, 255, 255, 255 });
			drawTexture(gameData.resourceDatas[pair.first].texture, { (state.res.x * 3) / 4 + 8, yOffset }, 2.0f, LEFT, BOTTOM);
			drawRect(outline, { 0, 0, 0, 255 }, { 255, 255, 255, 0 });

			yOffset += 16;
			drawText(gameData.resourceDatas[pair.first].name, { (state.res.x * 3) / 4 + 48, yOffset }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
			drawText(std::format("{:.2f} / {:.2f}", pair.second, gameData.storageDatas[type].maxCapacity), { state.res.x - 8, yOffset }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);

			yOffset += 24;
		}

		v2<int> dim = queryText("Back to City Menu", 2.0f);
		SDL_Rect r = v2ToRect({ ((state.res.x * 7) / 8) - (dim.x / 2) - 8, state.res.y - dim.y - 16 }, { dim.x + 16, dim.y + 8 });
		drawRect(r, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 255 });
		drawText("Back to City Menu", { (state.res.x * 7) / 8, state.res.y - 12 - (dim.y / 2) }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

		if (mouseInRect(r) && state.mouseState.click) {
			game.selectedStorage = -1;
		}
	}
}

float storage::totalStored() {
	float f = 0.0f;
	for (const std::pair<int, float>& pair : inventory) {
		f += pair.second;
	}

	return f;
}

float storage::ratioStored() {
	return totalStored() / gameData.storageDatas[type].maxCapacity;
}