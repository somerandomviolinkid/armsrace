#include "game/game.hpp"

void init() {
	state.init();
	game.gen = std::mt19937((uint32_t)time(0));
	gameData.init();

	game.mapPixelArray = (uint32_t*)malloc((size_t)(4 * state.res.x * state.res.y));
	if (game.mapPixelArray == 0) {
		printf("Fatal: could not allocate pixel array.");
		exit(-2);
	}

	game.mapPixelBufferArray = (uint32_t*)malloc((size_t)(4 * state.res.x * state.res.y));
	if (game.mapPixelBufferArray == 0) {
		printf("Fatal: could not allocate pixel buffer array.");
		exit(-2);
	}
}

void quit() {
	gameData.quit();
	state.quit();

	free(game.mapPixelArray);
	free(game.mapPixelBufferArray);
}

int main(int argc, char** argv) {
	init();

	while (state.running) {
		state.handleEvents();

		SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
		SDL_RenderClear(state.renderer);

		switch (state.mode) {
		case MAIN_MENU:
			drawMainMenu();
			mainMenuTick();
			break;
		case CREATE_GAME:
			drawCreateGameMenu();
			break;
		case CREATE_GAME_RANDOM:
			drawCreateRandomGameMenu();
			break;
		case CREATE_GAME_SCENARIO:
			drawCreateScenarioGameMenu();
			break;
		case CREATE_GAME_SCENARIO_SELECT_COUNTRY:
			drawChooseScenarioCountryGameMenu(game.selectedScenario);
			break;
		case LOAD_GAME:
			drawLoadGameMenu();
			loadGameMenuTick();
			break;
		case TUTORIAL:
			drawTutorialMenu();
			break;
		case CREDITS:
			drawCreditsMenu();
			break;
		case IN_GAME:
			float speeds[6] = { 2.0f, 1.0f, 0.5f, 0.25f, 0.1f, 0.0f };
			if (game.running && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - game.tickStart).count() > (long long)(1000.0f * speeds[game.selectedSpeed])) {
				game.tick();
			}

			game.draw();
			break;
		}

		SDL_RenderPresent(state.renderer);
	}

	quit();
	return 0;
}