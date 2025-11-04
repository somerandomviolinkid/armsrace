#include "game.hpp"

void naturalResource::draw(int i) {
	v2<float> np = project(pos);
	v2<int> sp = { int(np.x * (float)state.res.y), int(np.y * (float)state.res.y) };

	float weight = std::clamp(10.0f * game.camera.zoom, 0.0f, 10.0f);
	SDL_Rect outline = v2ToRect(sp - v2<int>{(int)(weight * 8.0f), (int)(weight * 8.0f)}, { (int)(weight * 16.0f), (int)(weight * 16.0f) });

	if (weight >= 1.0f) {
		drawTexture(gameData.resourceDatas[type].texture, sp, weight, MIDDLE, CENTER);
	} else {
		drawRect(outline, { 0, 0, 0, 255 }, { 0, 0, 255, 255 });
	}

	if (mouseInRect(outline) && state.mouseState.click && !game.selectingSomething()) {
		game.selectedResource = i;
	}

	if (game.selectedResource == i) {
		drawRect(outline, { 0, 0, 0, 255 }, { 255, 255, 255, 0 });
	}
}

void naturalResource::drawMenu() {
	drawRect(v2ToRect({ (state.res.x * 3) / 4, 64 }, { state.res.x / 4, state.res.y -64 }), { 0, 0, 0, 255 }, { 192, 192, 192, 255 });

	int yOffset = 96;
	drawText(gameData.resourceDatas[type].name, {(state.res.x * 7) / 8, yOffset }, 3.0f, {0, 0, 0, 255}, MIDDLE, CENTER);

	int mineIndex = std::distance(gameData.rawResources.begin(), std::find(gameData.rawResources.begin(), gameData.rawResources.end(), type));

	yOffset += 64;
	v2<int> dim = queryText("Build " + gameData.mineDatas[mineIndex].name, 2.0f);
	SDL_Rect r = v2ToRect({ ((state.res.x * 7) / 8) - (dim.x / 2) - 8, yOffset - (dim.y / 2) - 4 }, dim + v2<int>{16, 8});
	drawRect(r, { 0, 0, 0, 255 }, { 128, 128, 128, 255 });
	drawText("Build " + gameData.mineDatas[mineIndex].name, { (state.res.x * 7) / 8, yOffset }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);
	drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 0 }, { 128, 128, 192, 64 });

	if (mouseInRect(r) && state.mouseState.click) {
		game.mines.push_back(mine(mineIndex, 0, pos));
		game.naturalResources.erase(game.naturalResources.begin() + game.selectedResource);
		game.selectedResource = -1;
		game.selectedMine = game.mines.size() - 1;
	}
}