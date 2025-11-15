#include "game.hpp"

void industry::tick() {
	//allocate workers
	game.cities[ownerCity].employed -= workers;
	workers += std::clamp(game.cities[ownerCity].population / 2 - game.cities[ownerCity].employed, 0, gameData.industryDatas[type].maxWorkers - workers);
	game.cities[ownerCity].employed += workers;

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

	for (std::pair<const int, float>& pair : gameData.industryDatas[type].inputs) {
		inventory[pair.first] -= finalEfficiency * pair.second;
	}

	for (std::pair<const int, float>& pair : gameData.industryDatas[type].outputs) {
		inventory[pair.first] += finalEfficiency * pair.second;

		//handle exports
		if (exportDatas[pair.first].size() == 0) {
			continue;
		}

		float exportDivide = 1.0f;
		if (exportModes[pair.first]) {
			exportDivide = (float)exportDatas[pair.first].size();
		}

		for (const exportData& e : exportDatas[pair.first]) {
			switch (e.targetType) {
			case 0:
			{
				//storage
				float amountToExport = std::clamp(
					inventory[pair.first],
					0.0f,
					(gameData.storageDatas[game.cities[e.targetCity].storages[e.targetIndex].type].maxCapacity -
						game.cities[e.targetCity].storages[e.targetIndex].totalStored()) / exportDivide
				);

				game.cities[e.targetCity].storages[e.targetIndex].inventory[pair.first] += amountToExport;
				inventory[pair.first] -= amountToExport;
				break;
			}
			case 1:
			{
				//industry
				float amountToExport = std::clamp(
					inventory[pair.first],
					0.0f,
					(gameData.industryDatas[game.cities[e.targetCity].industries[e.targetIndex].type].storage[pair.first] - 
						game.cities[e.targetCity].industries[e.targetIndex].inventory[pair.first]) / exportDivide
				);

				game.cities[e.targetCity].industries[e.targetIndex].inventory[pair.first] += amountToExport;
				inventory[pair.first] -= amountToExport;
				break;
			}
			}
		}
	}
}

void industry::drawMenu() {
	drawRect(v2ToRect({ (state.res.x * 3) / 4, 64 }, { state.res.x / 4, state.res.y - 64 }), { 0, 0, 0, 255 }, { 192, 192, 192, 255 });

	int yOffset = 96;
	float scale = gameData.industryDatas[type].name.length() > 18 ? 2.0f : 3.0f;
	drawText(gameData.industryDatas[type].name, { (state.res.x * 7) / 8, yOffset }, scale, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	yOffset += 48;
	drawText("Workers:", { state.res.x * 3 / 4 + 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
	drawText(std::format("{} / {}", workers, gameData.industryDatas[type].maxWorkers), { state.res.x - 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);

	yOffset += 40;
	drawText("Worker Efficiency:", { state.res.x * 3 / 4 + 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
	drawText(std::format("{:.1f}%", ((float)workers * 100.0f) / (float)gameData.industryDatas[type].maxWorkers), { state.res.x - 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);

	yOffset += 32;
	SDL_Rect inventoryHeaderRect = v2ToRect({ (state.res.x * 3) / 4 , yOffset }, { state.res.x / 4, 64 });
	drawRect(inventoryHeaderRect, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 255 });
	if (mouseInRect(inventoryHeaderRect) && state.mouseState.click) {
		if (game.industryInventoryMenuOpen) {
			game.industryInventoryMenuOpen = false;
		} else {
			game.industryInventoryMenuOpen = true;
		}
	}

	yOffset += 32;
	drawText("Inventory", { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	if (game.industryInventoryMenuOpen) {
		drawTexture(state.baseTextures[MENU_OPEN], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	} else {
		drawTexture(state.baseTextures[MENU_CLOSED], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	}

	if (game.industryInventoryMenuOpen) {
		yOffset += 40;
		for (const std::pair<int, float>& pair : inventory) {
			SDL_Rect outline = v2ToRect({ (state.res.x * 3) / 4 + 8, yOffset }, { 32, 32 });
			drawRect(outline, { 0, 0, 0, 255 }, { 255, 255, 255, 255 });
			drawTexture(gameData.resourceDatas[pair.first].texture, { (state.res.x * 3) / 4 + 8, yOffset }, 2.0f, LEFT, BOTTOM);
			drawRect(outline, { 0, 0, 0, 255 }, { 255, 255, 255, 0 });

			yOffset += 16;
			drawText(gameData.resourceDatas[pair.first].name, { (state.res.x * 3) / 4 + 48, yOffset }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
			drawText(std::format("{:.1f} / {:.1f}", pair.second, gameData.industryDatas[type].storage[pair.first]), { state.res.x - 8, yOffset }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);

			yOffset += 24;
		}

		yOffset += 16;
		drawText("Resource Efficiency: ", { (state.res.x * 3) / 4 + 8, yOffset }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
		drawText(std::format("{:.1f}%", resourceEfficiency * 100.0f), { state.res.x - 8, yOffset }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);
	}

	//imports section

	yOffset += 32;
	SDL_Rect importsHeaderRect = v2ToRect({ (state.res.x * 3) / 4 , yOffset }, { state.res.x / 4, 64 });
	drawRect(importsHeaderRect, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 255 });
	if (mouseInRect(importsHeaderRect) && state.mouseState.click) {
		if (game.industryImportsMenuOpen) {
			game.industryImportsMenuOpen = false;
		} else {
			game.industryImportsMenuOpen = true;
		}
	}

	yOffset += 32;
	drawText("Imports", { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	if (game.industryImportsMenuOpen) {
		drawTexture(state.baseTextures[MENU_OPEN], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	} else {
		drawTexture(state.baseTextures[MENU_CLOSED], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	}

	int importHover = -1;
	if (game.industryImportsMenuOpen) {
		int i = 0;

		yOffset += 40;
		for (const std::pair<int, float> p : gameData.industryDatas[type].inputs) {
			SDL_Rect r = v2ToRect({ ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + yOffset }, { 64, 64 });
			drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 255 });
			drawTexture(gameData.resourceDatas[p.first].texture, { ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + yOffset }, 4.0f, LEFT, BOTTOM);
			drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 0 }, { 128, 128, 192, 64 });

			if (mouseInRect(r)) {
				importHover = p.first;
			}

			i++;
		}

		yOffset += (((i / 5) + 1) * 80);
	} else {
		yOffset += 32;
	}

	//exports section
	SDL_Rect exportsHeaderRect = v2ToRect({ (state.res.x * 3) / 4 , yOffset }, { state.res.x / 4, 64 });
	drawRect(exportsHeaderRect, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 255 });
	if (mouseInRect(exportsHeaderRect) && state.mouseState.click) {
		if (game.industryExportsMenuOpen) {
			game.industryExportsMenuOpen = false;
		} else {
			game.industryExportsMenuOpen = true;
		}
	}

	yOffset += 32;
	drawText("Exports", { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	if (game.industryExportsMenuOpen) {
		drawTexture(state.baseTextures[MENU_OPEN], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	} else {
		drawTexture(state.baseTextures[MENU_CLOSED], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	}

	int exportHover = -1;
	if (game.industryExportsMenuOpen) {
		int i = 0;

		yOffset += 40;
		for (const std::pair<int, float> p : gameData.industryDatas[type].outputs) {
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
	if (importHover != -1) {
		std::string n = gameData.resourceDatas[importHover].name;
		v2<int> dim = queryText(n, 1.0f);
		int xOffset = 16;
		if (state.mouseState.pos.x + +xOffset + 16 > state.res.x) {
			xOffset = -dim.x - 16;
		}

		SDL_Rect textOutline = v2ToRect(state.mouseState.pos + v2<int>{xOffset, 16}, v2<int>{dim.x, 18} + v2<int>{8, 8});
		drawRect(textOutline, { 0, 0, 0, 255 }, { 32, 32, 32, 192 });
		drawText(n, state.mouseState.pos + v2<int>{xOffset + 4, 20}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);
	}

	if (exportHover != -1) {
		std::string n = gameData.resourceDatas[exportHover].name;
		v2<int> dim = queryText(n, 1.0f);
		int xOffset = 16;
		if (state.mouseState.pos.x + +xOffset + 16 > state.res.x) {
			xOffset = -dim.x - 16;
		}

		SDL_Rect textOutline = v2ToRect(state.mouseState.pos + v2<int>{xOffset, 16}, v2<int>{dim.x, 18} + v2<int>{8, 8});
		drawRect(textOutline, { 0, 0, 0, 255 }, { 32, 32, 32, 192 });
		drawText(n, state.mouseState.pos + v2<int>{xOffset + 4, 20}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);
	}

	v2<int> dim = queryText("Back to City Menu", 2.0f);
	SDL_Rect r = v2ToRect({ ((state.res.x * 7) / 8) - (dim.x / 2) - 8, state.res.y - dim.y - 16 }, { dim.x + 16, dim.y + 8 });
	drawRect(r, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 255 });
	drawText("Back to City Menu", { (state.res.x * 7) / 8, state.res.y - 12 - (dim.y / 2) }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	if (state.mouseState.click) {
		if (mouseInRect(r)) {
			game.selectedIndustry = -1;
		}

		if (importHover != -1) {
			game.industryImportResourceSelected = importHover;
			game.industryExportResourceSelected = -1;
		}

		if (exportHover != -1) {
			game.industryImportResourceSelected = -1;
			game.industryExportResourceSelected = exportHover;
		}
	}
}

void industry::drawImportMenu(int i) {
	drawRect(v2ToRect({ (state.res.x * 3) / 4, 64 }, { state.res.x / 4, state.res.y - 64 }), { 0, 0, 0, 255 }, { 192, 192, 192, 255 });

	int yOffset = 96;
	std::string menuTitle = "Import " + gameData.resourceDatas[i].name;
	float scale = menuTitle.length() > 18 ? 2.0f : 3.0f;
	drawText(menuTitle, { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	yOffset += 48;
	drawText("Storages", { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	v2<int> dim = queryText("Back to Industry Menu", 2.0f);
	SDL_Rect r = v2ToRect({ ((state.res.x * 7) / 8) - (dim.x / 2) - 8, state.res.y - dim.y - 16 }, { dim.x + 16, dim.y + 8 });
	drawRect(r, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 255 });
	drawText("Back to Industry Menu", { (state.res.x * 7) / 8, state.res.y - 12 - (dim.y / 2) }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	if (mouseInRect(r) && state.mouseState.click) {
		game.industryImportResourceSelected = -1;
		game.industryImportsMenuOpen = false;
	}
}

void industry::drawExportMenu(int i) {
	drawRect(v2ToRect({ (state.res.x * 3) / 4, 64 }, { state.res.x / 4, state.res.y - 64 }), { 0, 0, 0, 255 }, { 192, 192, 192, 255 });

	int yOffset = 96;
	std::string menuTitle = "Export " + gameData.resourceDatas[i].name;
	float scale = menuTitle.length() > 18 ? 2.0f : 3.0f;
	drawText(menuTitle, { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	yOffset += 48;
	drawText("Storages", { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	int targetStorageHover[2] = { -1, -1 };
	int targetStorageHoverIndex = -1;
	int cityCounter = -1;

	int squareCount = 0;
	yOffset += 40;
	for (city& c : game.cities) {
		cityCounter++;

		if (c.owner != game.playingCountry) {
			//don't export to foreigners (yet)
			continue;
		}

		int cityStorageCounter = -1;
		for (storage& s : c.storages) {
			cityStorageCounter++;
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
			for (exportData& e : exportDatas[i]) {
				if (e.targetType == 0 && e.targetCity == cityCounter && e.targetIndex == cityStorageCounter) {
					fill = { 64, 64, 192, 255 };
					break;
				}
			}

			SDL_Rect r = v2ToRect({ ((state.res.x * 3) / 4) + ((squareCount % 5) * 80) + 16, ((squareCount / 5) * 80) + yOffset }, { 64, 64 });
			drawRect(r, { 0, 0, 0, 255 }, fill);
			drawTexture(gameData.storageDatas[s.type].texture, { ((state.res.x * 3) / 4) + ((squareCount % 5) * 80) + 16, ((squareCount / 5) * 80) + yOffset }, 4.0f, LEFT, BOTTOM);
			drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 0 }, { 64, 64, 192, 128 });

			if (mouseInRect(r)) {
				targetStorageHover[0] = cityCounter;
				targetStorageHover[1] = cityStorageCounter;
			}

			squareCount++;
		}
	}

	yOffset += (((squareCount / 5) + 1) * 80);

	yOffset += 48;
	drawText("Industries", { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	int targetIndustryHover[2] = { -1, -1 };
	int targetIndustryHoverIndex = -1;
	int industryCounter = -1;
	cityCounter = -1;
	squareCount = 0;

	yOffset += 40;
	for (city& c : game.cities) {
		cityCounter++;

		if (c.owner != game.playingCountry) {
			//don't export to foreigners (yet)
			continue;
		}

		int cityIndustryCounter = -1;
		for (industry& s : c.industries) {
			industryCounter++;
			cityIndustryCounter++;
			bool canExport = false;
			for (const std::pair<int, float> p : gameData.industryDatas[s.type].inputs) {
				if (p.first == i) {
					canExport = true;
					break;
				}
			}

			if (!canExport) {
				continue;
			}

			SDL_Color fill = { 255, 255, 255, 255 };
			for (exportData& e : exportDatas[i]) {
				if (e.targetType == 1 && e.targetCity == cityCounter && e.targetIndex == cityIndustryCounter) {
					fill = { 64, 64, 192, 255 };
					break;
				}
			}

			SDL_Rect r = v2ToRect({ ((state.res.x * 3) / 4) + ((squareCount % 5) * 80) + 16, ((squareCount / 5) * 80) + yOffset }, { 64, 64 });
			drawRect(r, { 0, 0, 0, 255 }, fill);
			drawTexture(gameData.industryDatas[s.type].texture, { ((state.res.x * 3) / 4) + ((squareCount % 5) * 80) + 16, ((squareCount / 5) * 80) + yOffset }, 4.0f, LEFT, BOTTOM);
			drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 0 }, { 64, 64, 192, 128 });

			if (mouseInRect(r)) {
				targetIndustryHover[0] = cityCounter;
				targetIndustryHover[1] = cityIndustryCounter;
				targetIndustryHoverIndex = industryCounter;
			}

			squareCount++;
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

			int eCount = -1;
			for (exportData& e : exportDatas[i]) {
				eCount++;
				if (e.targetType == 0 && e.targetCity == targetStorageHover[0] && e.targetIndex == targetStorageHover[1]) {
					isAlreadySelected = eCount;
					break;
				}
			}

			if (isAlreadySelected == -1) {
				exportDatas[i].push_back({targetStorageHover[0], 0, targetStorageHover[1]});
			} else {
				exportDatas[i].erase(exportDatas[i].begin() + isAlreadySelected);
			}
		}
	}

	if (targetIndustryHover[0] != -1 && targetIndustryHover[1] != -1) {
		std::string n = game.cities[targetIndustryHover[0]].name;
		std::string s = gameData.industryDatas[game.cities[targetIndustryHover[0]].industries[targetIndustryHover[1]].type].name;
		std::string c = std::format("{:.1f}% Filled", game.cities[targetIndustryHover[0]].industries[targetIndustryHover[1]].ratioStored(i) * 100.0f);

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

			int eCount = -1;
			for (exportData& e : exportDatas[i]) {
				eCount++;
				if (e.targetType == 1 && e.targetCity == targetIndustryHover[0] && e.targetIndex == targetIndustryHover[1]) {
					isAlreadySelected = eCount;
					break;
				}
			}

			if (isAlreadySelected == -1) {
				exportDatas[i].push_back({ targetIndustryHover[0], 1, targetIndustryHover[1]});
			} else {
				exportDatas[i].erase(exportDatas[i].begin() + isAlreadySelected);
			}
		}
	}

	v2<int> dim = queryText("Back to Industry Menu", 2.0f);
	SDL_Rect r = v2ToRect({ ((state.res.x * 7) / 8) - (dim.x / 2) - 8, state.res.y - dim.y - 16 }, { dim.x + 16, dim.y + 8 });
	drawRect(r, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 255 });
	drawText("Back to Industry Menu", { (state.res.x * 7) / 8, state.res.y - 12 - (dim.y / 2) }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	if (mouseInRect(r) && state.mouseState.click) {
		game.industryExportResourceSelected = -1;
		game.industryExportsMenuOpen = false;
	}
}

float industry::ratioStored(int resourceIndex) {
	return inventory[resourceIndex] / gameData.industryDatas[type].storage[resourceIndex];
}