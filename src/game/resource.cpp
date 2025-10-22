#include "game.hpp"

void naturalResource::draw(int i) {
	v2<float> np = project(pos);
	v2<int> sp = { int(np.x * (float)state.res.y), int(np.y * (float)state.res.y) };

	float weight = std::clamp(10.0f * game.camera.zoom, 0.0f, 10.0f);
	SDL_Rect outline = v2ToRect(sp - v2<int>{(int)(weight * 8.0f), (int)(weight * 8.0f)}, { (int)(weight * 16.0f), (int)(weight * 16.0f) });

	if (weight >= 1.0f) {
		drawTexture(gameData.resourceDatas[type].texture, sp, weight, MIDDLE, CENTER);
	}

	if (mouseInRect(outline) && state.mouseState.click && !game.selectingSomething()) {
		game.selectedResource = i;
	}

	if (game.selectedResource == i) {
		drawRect(outline, { 0, 0, 0, 255 }, { 255, 255, 255, 0 });
	}
}

void naturalResource::drawMenu() {
	drawRect(v2ToRect({ (state.res.x * 3) / 4, 0 }, { state.res.x / 4, state.res.y }), { 0, 0, 0, 255 }, { 192, 192, 192, 255 });

	drawText(gameData.resourceDatas[type].name, {(state.res.x * 7) / 8, 32}, 3.0f, {0, 0, 0, 255}, MIDDLE, CENTER);

	v2<int> dim = queryText("Build " + gameData.improvementDatas[type].name, 2.0f);
	//SDL_Rect r = v2ToRect({});
}