#include "game.hpp"

void improvement::tick() {
	//allocate workers
	if (workerCity != -1) {
		game.cities[workerCity].employed -= workers;
		workers += std::clamp(game.cities[workerCity].population / 2 - game.cities[workerCity].employed, 0, gameData.improvementDatas[type].maxWorkers - workers);
		game.cities[workerCity].employed += workers;
	}

	float baseEfficiency = (float)workers / (float)gameData.improvementDatas[type].maxWorkers;
	for (std::pair<const int, float>& pair : gameData.improvementDatas[type].outputs) {
		inventory[pair.first] = std::clamp(inventory[pair.first] + (baseEfficiency * pair.second), inventory[pair.first], gameData.improvementDatas[type].storage[pair.first]);
	}
}

void improvement::draw(int i) {
	v2<float> np = project(pos);
	v2<int> sp = { int(np.x * (float)state.res.y), int(np.y * (float)state.res.y) };

	float weight = std::clamp(10.0f * game.camera.zoom, 0.0f, 10.0f);
	SDL_Rect outline = v2ToRect(sp - v2<int>{(int)(weight * 8.0f), (int)(weight * 8.0f)}, { (int)(weight * 16.0f), (int)(weight * 16.0f) });

	if (weight >= 1.0f) {
		drawTexture(gameData.improvementDatas[type].texture, sp, weight, MIDDLE, CENTER);
	}
	if (state.mouseState.click && !game.selectingSomething()) {
		if (mouseInRect(outline)) {
			game.selectedImprovement = i;
		}
	}

	if (game.selectedImprovement == i) {
		drawRect(outline, { 0, 0, 0, 255 }, { 255, 255, 255, 0 });
	}
}

void improvement::drawMenu() {
	drawRect(v2ToRect({ (state.res.x * 3) / 4, 0 }, { state.res.x / 4, state.res.y }), { 0, 0, 0, 255 }, { 192, 192, 192, 255 });

	int yOffset = 32;
	float scale = gameData.improvementDatas[type].name.length() > 18 ? 2.0f : 3.0f;
	drawText(gameData.improvementDatas[type].name, { (state.res.x * 7) / 8, yOffset }, scale, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	yOffset += 48;
	drawText("Owner: ", { ((state.res.x * 3) / 4) + 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
	drawText(game.countries[owner].name, { state.res.x - 16, yOffset }, 2.0f, game.countries[owner].color, RIGHT, CENTER);

	yOffset += 40;
	drawText("Workers:", { state.res.x * 3 / 4 + 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
	drawText(std::format("{} / {}", workers, gameData.improvementDatas[type].maxWorkers), { state.res.x - 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);

	yOffset += 40;
	drawText("Worker Efficiency:", { state.res.x * 3 / 4 + 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
	drawText(std::format("{:.1f}%", ((float)workers * 100.0f) / (float)gameData.improvementDatas[type].maxWorkers), { state.res.x - 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);

	if (owner != 0) {
		//if it aint yours dont look inside
		return;
	}

	yOffset += 24;
	drawLine({ (state.res.x * 3) / 4, yOffset }, { state.res.x, yOffset }, { 0, 0, 0, 255 });

	yOffset += 32;
	drawText("Allocate Workers", { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	yOffset += 48;
	drawText("Select City", { (state.res.x * 7) / 8, yOffset }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	int i = 0;
	int cityHover = -1;
	int checkHover = -1;

	yOffset += 24;
	for (const city& c : game.cities) {
		if (c.owner != 0) {
			continue;
		}

		SDL_Rect textOutline = v2ToRect({ (state.res.x * 3) / 4 + 8, yOffset }, queryText(c.name, 2.0f));
		if (mouseInRect(textOutline)) {
			cityHover = i;
		}

		drawText(c.name, { (state.res.x * 3) / 4 + 8, yOffset + 16 }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);

		SDL_Color fill = { 255, 255, 255, 255 };
		if (workerCity == i) {
			fill = { 64, 64, 192, 255 };
		}

		SDL_Rect checkOutline = v2ToRect({ state.res.x - 40, yOffset }, { 32, 32 });
		drawRect(checkOutline, { 0, 0, 0, 255 }, fill);
		drawTexture(state.baseTextures[2], {state.res.x - 8, yOffset }, 2.0f, RIGHT, BOTTOM);

		drawRect(checkOutline, { 0, 0, 0, 255 }, { 255, 255, 255, 0 }, { 64, 64, 192, 128 });
		if (mouseInRect(checkOutline)) {
			checkHover = i;
		}

		yOffset += 40;
		i++;
	}

	if (cityHover != -1) {
		std::string w = formatNumber((float)game.cities[cityHover].population / 2.0f - (float)game.cities[cityHover].employed) + " available workers";
		v2<int> dim = queryText(w, 1.0f);
		int xOffset = 16;
		if (state.mouseState.pos.x +  + xOffset + 16 > state.res.x) {
			xOffset = -dim.x - 16;
		}

		SDL_Rect textOutline = v2ToRect(state.mouseState.pos + v2<int>{xOffset, 16}, v2<int>{dim.x, 18} + v2<int>{8, 8});
		drawRect(textOutline, { 0, 0, 0, 255 }, { 32, 32, 32, 192 });
		drawText(w, state.mouseState.pos + v2<int>{xOffset + 4, 20}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);
	}

	if (checkHover != -1 && state.mouseState.click) {
		workerCity = i;
	}

	yOffset += 24;
	drawLine({ (state.res.x * 3) / 4, yOffset }, { state.res.x, yOffset }, { 0, 0, 0, 255 });

	yOffset += 32;
	drawText("Inventory", { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	yOffset += 24;
	for (const std::pair<int, float>& pair : inventory) {
		SDL_Rect outline = v2ToRect({ (state.res.x * 3) / 4 + 8, yOffset }, { 32, 32 });
		drawRect(outline, { 0, 0, 0, 255 }, { 255, 255, 255, 255 });
		drawTexture(gameData.resourceDatas[pair.first].texture, { (state.res.x * 3) / 4 + 8, yOffset }, 2.0f, LEFT, BOTTOM);
		drawRect(outline, { 0, 0, 0, 255 }, { 255, 255, 255, 0 });

		drawText(gameData.resourceDatas[pair.first].name, { (state.res.x * 3) / 4 + 48, yOffset + 16 }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
		drawText(std::format("{:.2f} / {:.2f}", pair.second, gameData.improvementDatas[type].storage[pair.first]), { state.res.x - 8, yOffset + 16 }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);
		yOffset += 40;
	}
}