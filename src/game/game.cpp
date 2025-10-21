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

		if (selectedCity != -1) {
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
				}

				selectedCity = -1;
				selectedIndustry = -1;
			}
		}

		for (int i = 0; i < cities.size(); i++) {
			cities[i].draw(i);
		}

		if (selectedCity != -1) {
			if (cities[selectedCity].buildIndustryMenuOpen) {
				cities[selectedCity].drawBuildIndustryMenu();
			} else if (selectedIndustry == -1) {
				cities[selectedCity].drawMenu();
			} else {
				cities[selectedCity].industries[selectedIndustry].drawMenu();
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

Game game;

void newGame() {
	game.gen = std::mt19937((uint32_t)time(0));

	game.mode = NORMAL;

	for (int i = 0; i < 5; i++) {
		game.countries.push_back(country("Bobbystan" + std::to_string(i)));
		game.cities.push_back(city("Capitol", { randf(game.gen, -10.0f, 10.0f), (randf(game.gen, -10.0f, 10.0f)) }, i));
	}

	for (city& c : game.cities) {
		c.population = 1000000;
	}

	game.camera.pos = game.cities[0].pos;
	game.camera.zoom = 1.0f;

	game.selectedCity = -1;
	game.selectedIndustry = -1;

	game.occludeRects = {};

	game.tickStart = std::chrono::high_resolution_clock::now();
}

void clearGame() {
	game.countries.clear();
	game.cities.clear();
}

void loadGame() {

}

void saveGame() {

}