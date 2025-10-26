#include "game.hpp"
void Game::updateCamera() {
	if (state.mouseState.down) {
		v2<float> mouseMove = { (float)state.mouseState.motion.x / (float)state.res.y, (float)state.mouseState.motion.y / (float)state.res.y };
		game.camera.pos = game.camera.pos - (mouseMove / camera.zoom);
	}

	if (state.keyboardState[SDL_SCANCODE_W]) {
		game.camera.pos.y -= camera.zoom;
	}

	if (state.keyboardState[SDL_SCANCODE_A]) {
		game.camera.pos.x -= camera.zoom;
	}

	if (state.keyboardState[SDL_SCANCODE_S]) {
		game.camera.pos.y += camera.zoom;
	}

	if (state.keyboardState[SDL_SCANCODE_D]) {
		game.camera.pos.x += camera.zoom;
	}

	camera.zoom *= powf(1.05f, (float)state.mouseState.scroll);
}

void Game::tick() {
	for (city& c : cities) {
		c.tick();
		for (industry& i : c.industries) {
			i.tick();
		}
	}

	for (mine& i : mines) {
		i.tick();
	}

	ticks++;
	tickStart = std::chrono::high_resolution_clock::now();
}

void Game::draw() {
	switch (mode) {
	case NORMAL:
	{
		if (state.keyboardState[SDL_SCANCODE_ESCAPE]) {
			mode = PAUSE_MENU;
			break;
		}

		SDL_SetRenderDrawColor(state.renderer, 0, 192, 0, 255);
		SDL_RenderClear(state.renderer);

		occludeRects.push_back({ 0, 0, state.res.x, 64 });

		if (selectedCity != -1 || selectedResource != -1 || selectedMine != -1) {
			occludeRects.push_back(v2ToRect({ (state.res.x * 3) / 4, 0 }, { state.res.x / 4, state.res.y }));
		}

		bool mouseOccluded = false;
		for (SDL_Rect& r : occludeRects) {
			if (mouseInRect(r)) {
				mouseOccluded = true;
				break;
			}
		}

		if (!mouseOccluded) {
			updateCamera();
			if (state.mouseState.click) {
				if (selectedCity != -1) {
					cities[selectedCity].buildIndustryMenuOpen = false;
					cities[selectedCity].buildStorageMenuOpen = false;
				}

				selectedCity = -1;

				selectedIndustry = -1;

				selectedStorage = -1;

				selectedResource = -1;

				selectedMine = -1;
				mineExportResourceSelected = -1;
			}
		}

		for (int i = 0; i < cities.size(); i++) {
			cities[i].draw(i);
		}

		for (int i = 0; i < mines.size(); i++) {
			mines[i].draw(i);
		}

		for (int i = 0; i < naturalResources.size(); i++) {
			naturalResources[i].draw(i);
		}

		drawTopMenu();

		if (selectedCity != -1) {
			if (cities[selectedCity].buildIndustryMenuOpen) {
				cities[selectedCity].drawBuildIndustryMenu();
			} else if (cities[selectedCity].buildStorageMenuOpen) {
				cities[selectedCity].drawBuildStorageMenu();
			} else if (selectedIndustry != -1) {
				cities[selectedCity].industries[selectedIndustry].drawMenu();
			} else if (selectedStorage != -1) {
				cities[selectedCity].storages[selectedStorage].drawMenu();
			} else {
				cities[selectedCity].drawMenu();
			}
		} else if (selectedResource != -1) {
			naturalResources[selectedResource].drawMenu();
		} else if (selectedMine != -1) {
			if (mineExportResourceSelected != -1) {
				mines[selectedMine].drawExportMenu(mineExportResourceSelected);
			} else {
				mines[selectedMine].drawMenu();
			}
		}

		drawText(std::format("Camera Position: ({:.3f},{:.3f})", camera.pos.x, camera.pos.y), { state.res.x / 2, state.res.y - 32 }, 1.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);
		drawText(std::format("Camera Zoom: {:.3f}", camera.zoom), { state.res.x / 2, state.res.y - 16 }, 1.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);
		occludeRects.clear();

		break;
	}
	case PAUSE_MENU:
		drawPauseMenu();
		pauseMenuTick();
		break;
	case SAVE_GAME:
		break;
	}

}

void Game::drawTopMenu() {
	SDL_Rect r = { 0, 0, state.res.x, 64 };
	std::string d = std::format("Day {}, {:02}:00", ticks / 24, ticks % 24);
	drawRect(r, { 0, 0, 0, 255 }, { 192, 192, 192, 255 });
	drawText(d, { 16, 32 }, 3.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
}

bool Game::selectingSomething() {
	return selectedCity != -1 || selectedIndustry != -1 || selectedResource != -1 || selectedMine != -1;
}

Game game;

void newGame() {
	std::chrono::high_resolution_clock::time_point loadStart = std::chrono::high_resolution_clock::now();

	game.gen = std::mt19937((uint32_t)time(0));
	game.mode = NORMAL;

	int cityCounter = 0;
	for (int i = 0; i < 5; i++) {
		game.countries.push_back(country("Country " + std::to_string(i)));
		game.countries[i].color = { (uint8_t)randf(game.gen, 0.0f, 255.0f), (uint8_t)randf(game.gen, 0.0f, 255.0f), (uint8_t)randf(game.gen, 0.0f, 255.0f), 255 };

		v2<float> capitolPos = { randf(game.gen, -40.0f, 40.0f), (randf(game.gen, -40.0f, 40.0f)) };
		game.cities.push_back(city("Capitol", capitolPos, i));
		game.cities[cityCounter].population = 1000000;
		cityCounter++;

		for (int j = 0; j < 4; j++) {
			v2<float> pos = makeVector(capitolPos, randf(game.gen, 0.0f, 6.28f), randf(game.gen, 3.0f, 4.0f));
			game.cities.push_back(city("City " + std::to_string(j), pos, i));
			game.cities[cityCounter].population = (int)randf(game.gen, 100000.0f, 250000.0f);
			cityCounter++;
		}
	}

	for (int i = 0; i < gameData.rawResources.size(); i++) {
		for (int j = 0; j < 3; j++) {
			v2<float> startPos = { randf(game.gen, -25.0f, 25.0f), (randf(game.gen, -25.0f, 25.0f)) };
			int count = (int)randf(game.gen, 1.0f, 6.0f);
			for (int k = 0; k < count; k++) {
				v2<float> pos = makeVector(startPos, randf(game.gen, 0.0f, 6.28f), randf(game.gen, 1.0f, 2.0f));
				game.naturalResources.push_back(naturalResource(gameData.rawResources[i], pos));
			}
		}
	}

	game.countries[0].color = { 0, 0, 0, 255 };
	for (int i = 0; i < gameData.mineDatas.size(); i++) { game.mines.push_back(mine(i, 0, makeVector(game.cities[0].pos, randf(game.gen, 0.0f, 6.28f), randf(game.gen, 1.0f, 2.0f)))); }

	game.camera.pos = game.cities[0].pos;
	game.camera.zoom = 0.2f;

	game.selectedCity = -1;
	game.selectedIndustry = -1;
	game.selectedResource = -1;
	game.selectedStorage = -1;
	game.selectedMine = -1;

	game.cityIndustryMenuOpen = false;
	game.cityStorageMenuOpen = false;

	game.industryInventoryMenuOpen = false;
	game.storageInventoryMenuOpen = false;
	game.mineInventoryMenuOpen = false;

	game.mineAllocateWorkersMenuOpen = false;
	game.mineExportsMenuOpen = false;
	game.mineExportResourceSelected = -1;

	game.occludeRects = {};

	game.ticks = 0;
	game.tickStart = std::chrono::high_resolution_clock::now();

	printf("Created new game in %lld microseconds.\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - loadStart).count());
}

void clearGame() {
	game.countries.clear();
	game.cities.clear();
	game.mines.clear();
	game.naturalResources.clear();
}

void loadGame() {

}

void saveGame() {

}