#include "game.hpp"

void mine::tick() {
	//allocate workers
	if (workerCity != -1) {
		game.cities[workerCity].employed -= workers;
		workers += std::clamp(game.cities[workerCity].population / 2 - game.cities[workerCity].employed, 0, gameData.mineDatas[type].maxWorkers - workers);
		game.cities[workerCity].employed += workers;
	}

	float baseEfficiency = (float)workers / (float)gameData.mineDatas[type].maxWorkers;
	for (std::pair<const int, float>& pair : gameData.mineDatas[type].outputs) {
		inventory[pair.first] = std::clamp(inventory[pair.first] + (baseEfficiency * pair.second), inventory[pair.first], gameData.mineDatas[type].storage[pair.first]);
	}

	for (std::pair<const int, float>& pair : gameData.mineDatas[type].outputs) {
		for (const exportData& e : exportDatas) {
			float amountToExport = std::clamp(
				inventory[pair.first], 
				0.0f, 
				gameData.storageDatas[game.cities[e.targetCity].storages[e.targetIndex].type].maxCapacity - game.cities[e.targetCity].storages[e.targetIndex].totalStored()
			);
			game.cities[e.targetCity].storages[e.targetIndex].inventory[pair.first] += amountToExport;
			inventory[pair.first] -= amountToExport;
		}
	}
}

void mine::draw(int i) {
	v2<float> np = project(pos);
	v2<int> sp = { int(np.x * (float)state.res.y), int(np.y * (float)state.res.y) };

	float weight = std::clamp(10.0f * game.camera.zoom, 0.0f, 10.0f);
	SDL_Rect outline = v2ToRect(sp - v2<int>{(int)(weight * 8.0f), (int)(weight * 8.0f)}, { (int)(weight * 16.0f), (int)(weight * 16.0f) });

	if (weight >= 1.0f) {
		drawTexture(gameData.mineDatas[type].texture, sp, weight, MIDDLE, CENTER);
	}

	if (state.mouseState.click && !game.selectingSomething() && !mouseInRect({ 0, 0, state.res.x, 64 })) {
		if (mouseInRect(outline)) {
			game.selectedMine = i;
		}
	}

	if (game.selectedMine == i) {
		drawRect(outline, { 0, 0, 0, 255 }, { 255, 255, 255, 0 });
	}
}

void mine::drawMenu() {
	drawRect(v2ToRect({ (state.res.x * 3) / 4, 64 }, { state.res.x / 4, state.res.y - 64 }), { 0, 0, 0, 255 }, { 192, 192, 192, 255 });

	int yOffset = 96;
	float scale = gameData.mineDatas[type].name.length() > 18 ? 2.0f : 3.0f;
	drawText(gameData.mineDatas[type].name, { (state.res.x * 7) / 8, yOffset }, scale, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	yOffset += 48;
	drawText("Owner: ", { ((state.res.x * 3) / 4) + 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
	drawText(game.countries[owner].name, { state.res.x - 16, yOffset }, 2.0f, game.countries[owner].color, RIGHT, CENTER);

	yOffset += 40;
	drawText("Workers:", { state.res.x * 3 / 4 + 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
	drawText(std::format("{} / {}", workers, gameData.mineDatas[type].maxWorkers), { state.res.x - 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);

	yOffset += 40;
	drawText("Worker Efficiency:", { state.res.x * 3 / 4 + 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
	drawText(std::format("{:.1f}%", ((float)workers * 100.0f) / (float)gameData.mineDatas[type].maxWorkers), { state.res.x - 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);

	if (owner != 0) {
		//if it aint yours dont look inside
		return;
	}

	yOffset += 24;
	SDL_Rect allocateWorkersHeaderRect = v2ToRect({ (state.res.x * 3) / 4 , yOffset }, { state.res.x / 4, 64 });
	drawRect(allocateWorkersHeaderRect, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 255 });
	if (mouseInRect(allocateWorkersHeaderRect) && state.mouseState.click) {
		if (game.mineAllocateWorkersMenuOpen) {
			game.mineAllocateWorkersMenuOpen = false;
		} else {
			game.mineAllocateWorkersMenuOpen = true;
		}
	}

	yOffset += 32;
	drawText("Allocate Workers", { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	if (game.mineAllocateWorkersMenuOpen) {
		drawTexture(state.baseTextures[MENU_OPEN], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	} else {
		drawTexture(state.baseTextures[MENU_CLOSED], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	}

	int cityAllocateHover = -1;
	int cityCheckHover = -1;
	if (game.mineAllocateWorkersMenuOpen) {
		yOffset += 48;
		drawText("Select City", { (state.res.x * 7) / 8, yOffset }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

		int i = 0;
		yOffset += 24;
		for (const city& c : game.cities) {
			if (c.owner != 0) {
				continue;
			}

			SDL_Rect textOutline = v2ToRect({ (state.res.x * 3) / 4 + 8, yOffset }, queryText(c.name, 2.0f));
			if (mouseInRect(textOutline)) {
				cityAllocateHover = i;
			}

			drawText(c.name, { (state.res.x * 3) / 4 + 8, yOffset + 16 }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);

			SDL_Color fill = { 255, 255, 255, 255 };
			if (workerCity == i) {
				fill = { 64, 64, 192, 255 };
			}

			SDL_Rect checkOutline = v2ToRect({ state.res.x - 40, yOffset }, { 32, 32 });
			drawRect(checkOutline, { 0, 0, 0, 255 }, fill);
			drawTexture(state.baseTextures[CHECK], { state.res.x - 8, yOffset }, 2.0f, RIGHT, BOTTOM);

			drawRect(checkOutline, { 0, 0, 0, 255 }, { 255, 255, 255, 0 }, { 64, 64, 192, 128 });
			if (mouseInRect(checkOutline)) {
				cityCheckHover = i;
			}

			yOffset += 40;
			i++;
		}
	} else {
		yOffset += 32;
	}


	SDL_Rect inventoryHeaderRect = v2ToRect({ (state.res.x * 3) / 4 , yOffset }, { state.res.x / 4, 64 });
	drawRect(inventoryHeaderRect, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 255 });
	if (mouseInRect(inventoryHeaderRect) && state.mouseState.click) {
		if (game.mineInventoryMenuOpen) {
			game.mineInventoryMenuOpen = false;
		} else {
			game.mineInventoryMenuOpen = true;
		}
	}

	yOffset += 32;
	drawText("Inventory", { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	if (game.mineInventoryMenuOpen) {
		drawTexture(state.baseTextures[MENU_OPEN], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	} else {
		drawTexture(state.baseTextures[MENU_CLOSED], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	}

	if (game.mineInventoryMenuOpen) {
		yOffset += 40;
		for (const std::pair<int, float>& pair : inventory) {
			SDL_Rect outline = v2ToRect({ (state.res.x * 3) / 4 + 8, yOffset }, { 32, 32 });
			drawRect(outline, { 0, 0, 0, 255 }, { 255, 255, 255, 255 });
			drawTexture(gameData.resourceDatas[pair.first].texture, { (state.res.x * 3) / 4 + 8, yOffset }, 2.0f, LEFT, BOTTOM);
			drawRect(outline, { 0, 0, 0, 255 }, { 255, 255, 255, 0 });

			drawText(gameData.resourceDatas[pair.first].name, { (state.res.x * 3) / 4 + 48, yOffset + 16 }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
			drawText(std::format("{:.2f} / {:.2f}", pair.second, gameData.mineDatas[type].storage[pair.first]), { state.res.x - 8, yOffset + 16 }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);
			yOffset += 40;
		}
	} else {
		yOffset += 32;
	}

	SDL_Rect exportsHeaderRect = v2ToRect({ (state.res.x * 3) / 4 , yOffset }, { state.res.x / 4, 64 });
	drawRect(exportsHeaderRect, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 255 });
	if (mouseInRect(exportsHeaderRect) && state.mouseState.click) {
		if (game.mineExportsMenuOpen) {
			game.mineExportsMenuOpen = false;
		} else {
			game.mineExportsMenuOpen = true;
		}
	}

	yOffset += 32;
	drawText("Exports", { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	if (game.mineExportsMenuOpen) {
		drawTexture(state.baseTextures[MENU_OPEN], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	} else {
		drawTexture(state.baseTextures[MENU_CLOSED], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	}

	int exportHover = -1;
	if (game.mineExportsMenuOpen) {
		int i = 0;

		yOffset += 40;
		for (const std::pair<int, float> p : inventory) {
			SDL_Rect r = v2ToRect({ ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + yOffset }, { 64, 64 });
			drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 255 });
			drawTexture(gameData.resourceDatas[p.first].texture, { ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + yOffset }, 4.0f, LEFT, BOTTOM);
			drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 0 }, { 128, 128, 192, 64 });

			if (mouseInRect(r)) {
				exportHover = p.first;
			}

			i++;
		}
	}

	//handle hovers
	if (cityAllocateHover != -1) {
		std::string w = formatNumber((float)game.cities[cityAllocateHover].population / 2.0f - (float)game.cities[cityAllocateHover].employed) + " available workers";
		v2<int> dim = queryText(w, 1.0f);
		int xOffset = 16;
		if (state.mouseState.pos.x + +xOffset + 16 > state.res.x) {
			xOffset = -dim.x - 16;
		}

		SDL_Rect textOutline = v2ToRect(state.mouseState.pos + v2<int>{xOffset, 16}, v2<int>{dim.x, 18} + v2<int>{8, 8});
		drawRect(textOutline, { 0, 0, 0, 255 }, { 32, 32, 32, 192 });
		drawText(w, state.mouseState.pos + v2<int>{xOffset + 4, 20}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);
	}

	if (state.mouseState.click) {
		bool reset = false;
		if ((cityCheckHover == workerCity)) {
			//reset workers
			game.cities[workerCity].employed -= workers;
			workers = 0;
			workerCity = -1;
			reset = true;
		}

		if (cityCheckHover != -1 && !reset) {
			//set new city
			workerCity = cityCheckHover;
		}

		if (exportHover != -1) {
			game.mineExportResourceSelected = exportHover;
		}
	}
}

void mine::drawExportMenu(int i) {
	drawRect(v2ToRect({ (state.res.x * 3) / 4, 64 }, { state.res.x / 4, state.res.y - 64 }), { 0, 0, 0, 255 }, { 192, 192, 192, 255 });

	int yOffset = 96;
	std::string menuTitle = "Export " + gameData.resourceDatas[i].name;
	float scale = menuTitle.length() > 18 ? 2.0f : 3.0f;
	drawText(menuTitle, { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	yOffset += 48;
	drawText("Storages", { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	int targetStorageHover[2] = { -1, -1 };
	int targetStorageHoverIndex = -1;
	int storageCounter = 0;
	int cityCounter = -1;

	yOffset += 40;
	for (city& c : game.cities) {
		cityCounter++;

		if (c.owner != 0) {
			//don't export to foreigners (yet)
			continue;
		}

		int cityStorageCounter = 0;
		for (storage& s : c.storages) {
			bool canExport = false;
			for (const std::pair<int, float> p : s.inventory) {
				if (p.first == i) {
					canExport = true;
					break;
				}
			}

			if (!canExport) {
				continue;
			}

			SDL_Color fill = { 255, 255, 255, 255 };
			for (exportData& e : exportDatas) {
				if (e.index == storageCounter) {
					fill = { 64, 64, 192, 255 };
					break;
				}
			}

			SDL_Rect r = v2ToRect({ ((state.res.x * 3) / 4) + ((storageCounter % 5) * 80) + 16, ((storageCounter / 5) * 80) + yOffset }, { 64, 64 });
			drawRect(r, { 0, 0, 0, 255 }, fill);
			drawTexture(gameData.storageDatas[s.type].texture, { ((state.res.x * 3) / 4) + ((storageCounter % 5) * 80) + 16, ((storageCounter / 5) * 80) + yOffset }, 4.0f, LEFT, BOTTOM);
			drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 0 }, { 64, 64, 192, 128 });

			if (mouseInRect(r)) {
				targetStorageHover[0] = cityCounter;
				targetStorageHover[1] = cityStorageCounter;
				targetStorageHoverIndex = storageCounter;
			}

			storageCounter++;
			cityStorageCounter++;
		}
	}

	if (targetStorageHover[0] != -1 && targetStorageHover[1] != -1) {
		std::string n = game.cities[targetStorageHover[0]].name;
		std::string s = gameData.storageDatas[game.cities[targetStorageHover[0]].storages[targetStorageHover[1]].type].name;
		std::string c = std::format("{:.1f}% Filled", game.cities[targetStorageHover[0]].storages[targetStorageHover[1]].ratioStored() * 100.0f);

		std::vector<int> widths = { queryText(n, 1.0f).x, queryText(s, 1.0f).x, queryText(c, 1.0f).x };
		int max = *std::max_element(widths.begin(), widths.end());

		int xOffset = 16;
		if (state.mouseState.pos.x + max + xOffset + 16 > state.res.x) {
			xOffset = -max - 16;
		}

		SDL_Rect textOutline = v2ToRect(state.mouseState.pos + v2<int>{xOffset, 16}, v2<int>{max, 18} + v2<int>{8, 60});
		drawRect(textOutline, { 0, 0, 0, 255 }, { 32, 32, 32, 192 });
		drawText(n, state.mouseState.pos + v2<int>{xOffset + 4, 20}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);
		drawText(s, state.mouseState.pos + v2<int>{xOffset + 4, 44}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);
		drawText(c, state.mouseState.pos + v2<int>{xOffset + 4, 68}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);

		if (state.mouseState.click) {
			int isAlreadySelected = -1;
			for (exportData& e : exportDatas) {
				if (e.index == targetStorageHoverIndex) {
					isAlreadySelected = e.index;
					break;
				}
			}

			if (isAlreadySelected == -1) {
				exportDatas.push_back({ targetStorageHoverIndex, targetStorageHover[0], 0, targetStorageHover[1] });
			} else {
				exportDatas.erase(exportDatas.begin() + isAlreadySelected);
			}
		}
	}

	v2<int> dim = queryText("Back to Mine Menu", 2.0f);
	SDL_Rect r = v2ToRect({ ((state.res.x * 7) / 8) - (dim.x / 2) - 8, state.res.y - dim.y - 16 }, { dim.x + 16, dim.y + 8 });
	drawRect(r, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 255 });
	drawText("Back to Mine Menu", { (state.res.x * 7) / 8, state.res.y - 12 - (dim.y / 2) }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	if (mouseInRect(r) && state.mouseState.click) {
		game.mineExportResourceSelected = -1;
	}
}