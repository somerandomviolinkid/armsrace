#include "game.hpp"

void industry::tick() {
	//allocate workers
	game.cities[game.selectedCity].employed -= workers;
	workers += std::clamp(game.cities[game.selectedCity].population / 2 - game.cities[game.selectedCity].employed, 0, gameData.industryDatas[type].maxWorkers - workers);
	game.cities[game.selectedCity].employed += workers;

	//turn inputs into outputs!
	float baseEfficiency = (float)workers / (float)gameData.industryDatas[type].maxWorkers;
	std::vector<float> efficiencies = {};

	//get resource input efficiency
	for (const std::pair<int, float>& pair : gameData.industryDatas[type].inputs) {
		efficiencies.push_back(inventory[pair.first] / pair.second);
	}

	//stops overflow
	for (const std::pair<int, float>& pair : gameData.industryDatas[type].outputs) {
		efficiencies.push_back((gameData.industryDatas[type].storage[pair.first] - inventory[pair.first]) / pair.second);
	}

	float min = *std::min_element(efficiencies.begin(), efficiencies.end());
	resourceEfficiency = min;
	float finalEfficiency = min * baseEfficiency;

	for (const std::pair<int, float>& pair : gameData.industryDatas[type].inputs) {
		inventory[pair.first] -= finalEfficiency * pair.second;
	}

	for (const std::pair<int, float>& pair : gameData.industryDatas[type].outputs) {
		inventory[pair.first] += finalEfficiency * pair.second;
	}
}

void industry::drawMenu() {
	drawRect(v2ToRect({ (state.res.x * 3) / 4, 0 }, { state.res.x / 4, state.res.y }), { 0, 0, 0, 255 }, { 192, 192, 192, 255 });

	float scale = gameData.industryDatas[type].name.length() > 18 ? 2.0f : 3.0f;

	drawText(gameData.industryDatas[type].name, { (state.res.x * 7) / 8, 32 }, scale, { 0, 0, 0, 255 }, MIDDLE, CENTER);
	drawText("Workers:", { state.res.x * 3 / 4 + 16, 80 }, 2.0f, {0, 0, 0, 255}, LEFT, CENTER);
	drawText(std::format("{} / {}", workers, gameData.industryDatas[type].maxWorkers), {state.res.x - 16, 80}, 2.0f, {0, 0, 0, 255}, RIGHT, CENTER);
	drawText("Worker Efficiency:", { state.res.x * 3 / 4 + 16, 120 }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
	drawText(std::format("{:.1f}%", ((float)workers * 100.0f) / (float)gameData.industryDatas[type].maxWorkers), { state.res.x - 16, 120 }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);

	drawLine({ (state.res.x * 3) / 4, 144 }, { state.res.x, 144 }, { 0, 0, 0, 255 });
	drawText("Inventory", { (state.res.x * 7) / 8, 176 }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	int i = 0;
	for (const std::pair<int, float>& pair : inventory) {
		SDL_Rect outline = v2ToRect({ (state.res.x * 3) / 4 + 8, 208 + (40 * i) }, { 32, 32 });
		drawRect(outline, {0, 0, 0, 255}, {255, 255, 255, 255});
		drawTexture(gameData.resourceDatas[pair.first].texture, { (state.res.x * 3) / 4 + 8, 208 + (40 * i) }, 2.0f, LEFT, BOTTOM);
		drawRect(outline, { 0, 0, 0, 255 }, { 255, 255, 255, 0 });

		drawText(gameData.resourceDatas[pair.first].name, { (state.res.x * 3) / 4 + 48, 208 + (40 * i) + 16 }, 2.0f, {0, 0, 0, 255}, LEFT, CENTER);
		drawText(std::format("{:.2f} / {:.2f}", pair.second, gameData.industryDatas[type].storage[pair.first]), { state.res.x -8, 208 + (40 * i) + 16 }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);
		i++;
	}

	drawText("Resource Efficiency: ", {(state.res.x * 3) / 4 + 8, 208 + (40 * i) + 16}, 2.0f, {0, 0, 0, 255}, LEFT, CENTER);
	drawText(std::format("{:.1f}%", resourceEfficiency * 100.0f), { state.res.x - 8, 208 + (40 * i) + 16 }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);



	v2<int> dim = queryText("Back to City Menu", 2.0f);
	SDL_Rect r = v2ToRect({ ((state.res.x * 7) / 8) - (dim.x / 2) - 8, state.res.y - dim.y - 16 }, { dim.x + 16, dim.y + 8 });
	drawRect(r, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, {128, 128, 192, 255});
	drawText("Back to City Menu", { (state.res.x * 7) / 8, state.res.y - 12 - (dim.y / 2) }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	if (mouseInRect(r) && state.mouseState.click) {
		game.selectedIndustry = -1;
	}
}