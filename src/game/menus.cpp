#include "game.hpp"

void drawMainMenu() {
	//splash texture
	SDL_RenderCopy(state.renderer, state.baseTextures[SPLASH].texture, NULL, NULL);

	drawText("Arms Race", { state.res.x / 2 , 144 }, 5.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);
	drawText("Pre-Alpha v11/5.2", { state.res.x / 2 , 216 }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	int width = queryText(gameData.splashText, 1.0f).x + 32;
	float scale = std::clamp(((float)state.res.x / 2.0f) / (float)width, 1.0f, 4.0f);
	drawText(gameData.splashText, { (state.res.x * 3) / 4 , state.res.y / 3 }, scale, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	drawRect(v2ToRect({ (state.res.x / 2) - 108, (state.res.y / 2) - 28 }, { 216, (48 * 9) + 8 }), { 0, 0, 0, 255 }, { 64, 64, 64, 255 });

	for (int i = 0; i < 9; i++) {
		drawRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20 + (48 * i) }, { 200, 40 }), { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	}

	drawText("Create Game", { state.res.x / 2 , state.res.y / 2 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Load Game", { state.res.x / 2,  (state.res.y / 2) + 48 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Settings", { state.res.x / 2,  (state.res.y / 2) + 96 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Wiki", { state.res.x / 2,  (state.res.y / 2) + 144 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Tutorial", { state.res.x / 2,  (state.res.y / 2) + 192 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Credits", { state.res.x / 2,  (state.res.y / 2) + 240 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Discord", { state.res.x / 2,  (state.res.y / 2) + 288 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Github", { state.res.x / 2,  (state.res.y / 2) + 336 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Quit", { state.res.x / 2,  (state.res.y / 2) + 384 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
}

void mainMenuTick() {
	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20 + (48 * 0) }, { 200, 48 })) && state.mouseState.click) {
		state.mode = CREATE_GAME;
		return;
	}

	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20 + (48 * 1) }, { 200, 48 })) && state.mouseState.click) {
		state.mode = LOAD_GAME;
		return;
	}

	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20 + (48 * 3) }, { 200, 48 })) && state.mouseState.click) {
		SDL_OpenURL("https://github.com/somerandomviolinkid/armsrace/wiki");
		return;
	}

	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20 + (48 * 4) }, { 200, 48 })) && state.mouseState.click) {
		state.mode = TUTORIAL;
		return;
	}

	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20 + (48 * 5) }, { 200, 48 })) && state.mouseState.click) {
		state.mode = CREDITS;
		return;
	}

	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20 + (48 * 6) }, { 200, 48 })) && state.mouseState.click) {
		SDL_OpenURL("https://discord.gg/NTmdbAKqEt");
		return;
	}

	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20 + (48 * 7) }, { 200, 48 })) && state.mouseState.click) {
		SDL_OpenURL("https://github.com/somerandomviolinkid/armsrace");
		return;
	}

	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20 + (48 * 8) }, { 200, 48 })) && state.mouseState.click) {
		state.running = false;
		return;
	}
}

void drawLoadGameMenu() {
	SDL_RenderCopy(state.renderer, state.baseTextures[SPLASH].texture, NULL, NULL);
	drawText("Load Game", { state.res.x / 2 , 100 }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	const std::filesystem::path saves = "saves";
	int count = 0;
	std::vector<int> widths = {};
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(saves)) {
		widths.push_back(queryText(entry.path().string().substr(6), 2.0f).x);
		count++;
	}

	if (widths.size() == 0) {
		drawText("No savegames yet!", { state.res.x / 2 , state.res.y / 4 }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);
		drawRect(v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 - 20 }, { 300, 40 }), { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
		drawText("Back to Main Menu", { state.res.x / 2 , (state.res.y * 7) / 8 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
		return;
	}

	int max = *std::max_element(widths.begin(), widths.end());
	max += 16;

	drawRect(v2ToRect({ (state.res.x / 2) - (max / 2) - 8, state.res.y / 4 - 28 }, { max + 16, 8 + (48 * count) }), { 0, 0, 0, 255 }, { 64, 64, 64, 255 });

	int i = 0;
	std::string hoverPath = "";
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(saves)) {
		int w = queryText(entry.path().string().substr(6), 2.0f).x;
		SDL_Rect r = v2ToRect({ (state.res.x / 2) - (w / 2) - 8, (state.res.y / 4) - 20 + (48 * i) }, { w + 16, 40 });
		drawRect(r, { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
		drawText(entry.path().string().substr(6), { state.res.x / 2 , (state.res.y / 4) + (48 * i) }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
		i++;

		if (mouseInRect(r)) {
			hoverPath = entry.path().string();
		}
	}

	if (!hoverPath.empty()) {
		std::ifstream metaFile(hoverPath + "/meta.txt");
		if (metaFile.good()) {
			std::string line = "";

			std::getline(metaFile, line);
			std::string version = line;
			version = "Version: " + version;

			std::getline(metaFile, line);
			int ticks = 0;
			bool goodMetaFile = true;
			if (sscanf(line.c_str(), "%d", &ticks) != 1) {
				std::string errorText = "Incompatible meta file";
				int w = queryText(errorText, 1.0f).x;
				SDL_Rect textOutline = v2ToRect(state.mouseState.pos + v2<int>{16, 16}, v2<int>{w, 18} + v2<int>{8, 8});
				drawRect(textOutline, { 0, 0, 0, 255 }, { 32, 32, 32, 192 });
				drawText(errorText, state.mouseState.pos + v2<int>{20, 20}, 1.0f, { 255, 0, 0, 255 }, LEFT, BOTTOM);
				goodMetaFile = false;
			}

			if (goodMetaFile) {
				std::string suffix = ticks % 24 >= 12 ? "PM" : "AM";
				int normalizedTime = ticks % 24 >= 12 ? ticks % 24 - 12 : ticks % 24;
				if (normalizedTime == 0) {
					normalizedTime = 12;
				}

				std::string day = std::format("Day {}, {:02}:00 ", ticks / 24, normalizedTime) + suffix;
				std::vector<int> widths = { queryText(version, 1.0f).x , queryText(day, 1.0f).x };
				int maxWidth = *std::max_element(widths.begin(), widths.end());

				SDL_Rect textOutline = v2ToRect(state.mouseState.pos + v2<int>{16, 16}, v2<int>{maxWidth, 18} + v2<int>{8, 36});
				drawRect(textOutline, { 0, 0, 0, 255 }, { 32, 32, 32, 192 });
				drawText(version, state.mouseState.pos + v2<int>{20, 20}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);
				drawText(day, state.mouseState.pos + v2<int>{20, 44}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);
			}
		} else {
			std::string errorText = "Bad meta file";
			int w = queryText(errorText, 1.0f).x;
			SDL_Rect textOutline = v2ToRect(state.mouseState.pos + v2<int>{16, 16}, v2<int>{w, 18} + v2<int>{8, 8});
			drawRect(textOutline, { 0, 0, 0, 255 }, { 32, 32, 32, 192 });
			drawText(errorText, state.mouseState.pos + v2<int>{20, 20}, 1.0f, {255, 0, 0, 255}, LEFT, BOTTOM);
		}


		metaFile.close();
	}

	if (state.mouseState.click && !hoverPath.empty()) {
		int loadStatus = loadGame(hoverPath);
		switch (loadStatus) {
		case 0:
			state.mode = IN_GAME;
			break;
		case -1:
			printf("Loading error: wrong version\n");
			break;
		case -2:
			printf("Loading error: files missing\n");
			break;
		case -3:
			printf("Loading error: bad savedata\n");
			break;
		}
	}

	drawRect(v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 - 20 }, { 300, 40 }), { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Back to Main Menu", { state.res.x / 2 , (state.res.y * 7) / 8 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
}

void loadGameMenuTick() {
	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 - 20 }, { 300, 40 })) && state.mouseState.click) {
		state.mode = MAIN_MENU;
		return;
	}
}

void drawCreditsMenu() {
	drawText("Credits", { state.res.x / 2 , 100 }, 3.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	int yOffset = 200;
	drawText("Lead Developer", { state.res.x / 2 - 32, yOffset }, 2.0f, { 255, 255, 255, 255 }, RIGHT, CENTER);
	drawText("-", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Bob", { state.res.x / 2 + 32, yOffset }, 2.0f, { 255, 255, 255, 255 }, LEFT, CENTER);

	yOffset += 32;
	drawText("Lead Artist", { state.res.x / 2 - 32, yOffset }, 2.0f, { 255, 255, 255, 255 }, RIGHT, CENTER);
	drawText("-", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Bob", { state.res.x / 2 + 32, yOffset }, 2.0f, { 255, 255, 255, 255 }, LEFT, CENTER);

	yOffset += 64;
	drawText("Alpha Playtesters", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	yOffset += 32;
	drawText("Wayalon", { state.res.x / 2 - 32, yOffset }, 1.0f, { 255, 255, 255, 255 }, RIGHT, CENTER);
	drawText("ExpertBowen", { state.res.x / 2 + 32, yOffset }, 1.0f, { 255, 255, 255, 255 }, LEFT, CENTER);

	yOffset += 64;
	drawText("Special Thanks", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	yOffset += 32;
	drawText("Digdogg", { state.res.x / 2 - 32, yOffset }, 1.0f, { 255, 255, 255, 255 }, RIGHT, CENTER);

	SDL_Rect back = v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 - 20 }, { 300, 40 });
	drawRect(back, { 255, 255, 255, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Back to Main Menu", { state.res.x / 2 , (state.res.y * 7) / 8 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	if (mouseInRect(back) && state.mouseState.click) {
		state.mode = MAIN_MENU;
		return;
	}
}

void drawPauseMenu() {
	SDL_RenderCopy(state.renderer, state.baseTextures[SPLASH].texture, NULL, NULL);

	drawRect(v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 - 68 }, { 300, 40 }), { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Save Game", { state.res.x / 2 , (state.res.y * 7) / 8 - 48 }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	drawRect(v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 - 20 }, { 300, 40 }), { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Back to Game", { state.res.x / 2 , (state.res.y * 7) / 8 }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	drawRect(v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 + 28 }, { 300, 40 }), { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Back to Main Menu", { state.res.x / 2 , (state.res.y * 7) / 8 + 48 }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);
}

void pauseMenuTick() {
	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 - 68 }, { 300, 40 })) && state.mouseState.click) {
		saveGame();
		return;
	}

	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 - 20 }, { 300, 40 })) && state.mouseState.click) {
		game.mode = NORMAL;
		game.running = true;
		return;
	}

	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 + 28 }, { 300, 40 })) && state.mouseState.click) {
		clearGame();
		state.mode = MAIN_MENU;
		return;
	}
}

void drawSaveGameMenu() {

}

void drawTutorialMenu() {
	drawText("Tutorial", { state.res.x / 2 , 100 }, 3.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	int yOffset = 200;
	drawText("Welcome to Arms Race! Here you can find everything you need to know about starting the game. If you want", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	yOffset += 40;
	drawText("to learn more about the inner machinations of the game, head to the wiki. You can build industries by", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	yOffset += 40;
	drawText("clicking on a city, and clicking on the Industry drop-down menu. This will list the currently built", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	yOffset += 40;
	drawText("industries, and a hammer icon that you can click to build new industries. The same applies to storages.", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	yOffset += 40;
	drawText("Workers will automatically fill up in industries. However, in order to get your industries up and running,", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	yOffset += 40;
	drawText("you need raw resources. Currently, you can't build mines, but one of each type of mine in the game is near", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	yOffset += 40;
	drawText("your capital. To give a mine workers, click on the Allocate Workers drop-down menu and select which city to", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	yOffset += 40;
	drawText("import workers from. This will make the mine start running, and its inventory will begin to fill up. To", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	yOffset += 40;
	drawText("export resources to storages, select the Export Resources drop-down menu and select which resource you want", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	yOffset += 40;
	drawText("to export. Then, select which storages you want to export to. Exports work from left to right, so the mine", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	yOffset += 40;
	drawText(" will export all of its resources to the left-most storage until it fills up, then it will start filling", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	yOffset += 40;
	drawText("up the next storage on its list. Lastly, if you get lost, you can press C to focus the camera back on your", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	yOffset += 40;
	drawText("capitol. Press the pause button to pause/resume, and press - or = to decrease/increase the game speed.", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	yOffset += 64;
	drawText("Happy industrializing!", { state.res.x / 2, yOffset }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	SDL_Rect back = v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 - 20 }, { 300, 40 });
	drawRect(back, { 255, 255, 255, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Back to Main Menu", { state.res.x / 2 , (state.res.y * 7) / 8 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	if (mouseInRect(back) && state.mouseState.click) {
		state.mode = MAIN_MENU;
		return;
	}
}

void drawCreateGameMenu() {
	SDL_RenderCopy(state.renderer, state.baseTextures[SPLASH].texture, NULL, NULL);
	drawText("Create Game", { state.res.x / 2 , 100 }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	SDL_Rect random = v2ToRect({ (state.res.x / 4) - 100, 220 }, { 200, 96 });
	drawRect(random, { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Random Game", {state.res.x / 4, 244}, 2.0f, {255, 255, 255, 255}, MIDDLE, CENTER);

	SDL_Rect senario = v2ToRect({ (state.res.x * 3 / 4) - 100, 220 }, { 200, 96 });
	drawRect(senario, { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Scenario", { (state.res.x * 3 / 4), 244 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	SDL_Rect back = v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 - 20 }, { 300, 40 });
	drawRect(back, { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Back to Main Menu", { state.res.x / 2 , (state.res.y * 7) / 8 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	if (mouseInRect(random) && state.mouseState.click) {
		state.mode = CREATE_GAME_RANDOM;
		return;
	}

	if (mouseInRect(senario) && state.mouseState.click) {
		state.mode = CREATE_GAME_SCENARIO;
		return;
	}

	if (mouseInRect(back) && state.mouseState.click) {
		state.mode = MAIN_MENU;
		return;
	}
}

void drawCreateRandomGameMenu() {
	SDL_RenderCopy(state.renderer, state.baseTextures[SPLASH].texture, NULL, NULL);
	drawText("Create Random Game", { state.res.x / 2 , 100 }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	drawText("Enter your country name:", { state.res.x / 2 , 148 }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	std::string s = "Create Game";
	v2<int> dim = queryText(s, 2.0f);
	SDL_Rect createGameButton = v2ToRect({ (state.res.x - dim.x - 16) / 2, (state.res.y - dim.y - 8) / 2 }, dim + v2<int>{16, 8});
	drawRect(createGameButton, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 64, 64, 96, 255 });
	drawText(s, { state.res.x / 2 , state.res.y / 2 }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	if (mouseInRect(createGameButton) && state.mouseState.click) {
		newGame();
		state.mode = IN_GAME;
		game.mode = NORMAL;
		return;
	}

	SDL_Rect backCreateGame = v2ToRect({ (state.res.x / 2) - 200, (state.res.y * 7) / 8 - 68 }, { 400, 40 });
	drawRect(backCreateGame, { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Back to Create Game Menu", { state.res.x / 2 , (state.res.y * 7) / 8 - 48 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	if (mouseInRect(backCreateGame) && state.mouseState.click) {
		state.mode = CREATE_GAME;
		return;
	}

	SDL_Rect backMainMenu = v2ToRect({ (state.res.x / 2) - 200, (state.res.y * 7) / 8 - 20 }, { 400, 40 });
	drawRect(backMainMenu, { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Back to Main Menu", { state.res.x / 2 , ((state.res.y * 7) / 8) }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	if (mouseInRect(backMainMenu) && state.mouseState.click) {
		state.mode = MAIN_MENU;
		return;
	}
}

void drawCreateScenarioGameMenu() {
	SDL_RenderCopy(state.renderer, state.baseTextures[SPLASH].texture, NULL, NULL);
	drawText("Select Scenario", { state.res.x / 2 , 100 }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	const std::filesystem::path scenarios = "assets/scenarios";
	int count = 0;
	std::vector<int> widths = {};
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(scenarios)) {
		widths.push_back(queryText(entry.path().string().substr(17), 2.0f).x);
		count++;
	}

	if (widths.size() == 0) {
		drawText("No scenarios yet!", { state.res.x / 2 , state.res.y / 4 }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);
		drawRect(v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 - 20 }, { 300, 40 }), { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
		drawText("Back to Main Menu", { state.res.x / 2 , (state.res.y * 7) / 8 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
		return;
	}

	int max = *std::max_element(widths.begin(), widths.end());
	max += 16;

	drawRect(v2ToRect({ (state.res.x / 2) - (max / 2) - 8, state.res.y / 4 - 28 }, { max + 16, 8 + (48 * count) }), { 0, 0, 0, 255 }, { 64, 64, 64, 255 });

	int i = 0;
	std::string hoverPath = "";
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(scenarios)) {
		int w = queryText(entry.path().string().substr(17), 2.0f).x;
		SDL_Rect r = v2ToRect({ (state.res.x / 2) - (w / 2) - 8, (state.res.y / 4) - 20 + (48 * i) }, { w + 16, 40 });
		drawRect(r, { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
		drawText(entry.path().string().substr(17), { state.res.x / 2 , (state.res.y / 4) + (48 * i) }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
		i++;

		if (mouseInRect(r)) {
			hoverPath = entry.path().string();
		}
	}

	if (!hoverPath.empty() && state.mouseState.click) {
		game.selectedScenario = hoverPath;
		state.mode = CREATE_GAME_SCENARIO_SELECT_COUNTRY;
	}

	SDL_Rect backCreateGame = v2ToRect({ (state.res.x / 2) - 200, (state.res.y * 7) / 8 - 68 }, { 400, 40 });
	drawRect(backCreateGame, { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Back to Create Game Menu", { state.res.x / 2 , (state.res.y * 7) / 8 - 48 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	if (mouseInRect(backCreateGame) && state.mouseState.click) {
		state.mode = CREATE_GAME;
		return;
	}

	SDL_Rect backMainMenu = v2ToRect({ (state.res.x / 2) - 200, (state.res.y * 7) / 8 - 20 }, { 400, 40 });
	drawRect(backMainMenu, { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Back to Main Menu", { state.res.x / 2 , ((state.res.y * 7) / 8) }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	if (mouseInRect(backMainMenu) && state.mouseState.click) {
		state.mode = MAIN_MENU;
		return;
	}
}

void drawChooseScenarioCountryGameMenu(std::string name) {
	if (name == "") {
		//should never happen but nothing is impossible i guess
		return;
	}

	SDL_RenderCopy(state.renderer, state.baseTextures[SPLASH].texture, NULL, NULL);
	drawText("Select " + name.substr(17) + " Country", {state.res.x / 2 , 100}, 3.0f, {0, 0, 0, 255}, MIDDLE, CENTER);
	std::vector<std::string> countryNames = {};
	std::vector<int> countryNameLengths = {};

	std::ifstream countryFile(name + "/country.txt");
	std::string line = "";
	while (std::getline(countryFile, line)) {
		if (line == "END") {
			countryFile.close();
			break;
		}

		countryNames.push_back(line);
		countryNameLengths.push_back(queryText(line, 2.0f).x);

		std::getline(countryFile, line);
		std::getline(countryFile, line);

		if (line == "END") {
			countryFile.close();
			break;
		}
	}

	countryFile.close();
	int maxLength = *std::max_element(countryNameLengths.begin(), countryNameLengths.end());

	//16 pixel pad around each rect
	int quota = state.res.x / (maxLength + 64);
	int initOffset = (state.res.x - (quota * (maxLength + 64))) / 2;

	int hover = -1;
	for (int i = 0; i < countryNames.size(); i++) {
		SDL_Rect r = v2ToRect({ initOffset + (i * (maxLength + 64)), 150 + ((i / quota) * 100) }, {maxLength + 32, 84});
		drawRect(r, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 192 });
		//drawText(countryNames[i], {}, 2.0f, {0, 0, 0, 255});
		//drawTexture(gameData.scenarioTextures[name][i], {}, 1.0f, MIDDLE, CENTER);

		if (mouseInRect(r)) {
			hover = i;
		}
	}

	if (hover != -1 && state.mouseState.click) {
		loadGame(name);
		game.playingCountry = hover;
		for (city c : game.cities) {
			if (c.owner == game.playingCountry && c.capital) {
				game.camera.pos = c.pos;
				game.camera.zoom = 0.1f;
				break;
			}
		}

		state.mode = IN_GAME;
		game.mode = NORMAL;
		return;
	}

	SDL_Rect backCreateGame = v2ToRect({ (state.res.x / 2) - 200, (state.res.y * 7) / 8 - 20 }, { 400, 40 });
	drawRect(backCreateGame, { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Back to Create Game Menu", { state.res.x / 2 , (state.res.y * 7) / 8 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	if (mouseInRect(backCreateGame) && state.mouseState.click) {
		state.mode = CREATE_GAME;
		game.selectedScenario = "";
		return;
	}

	SDL_Rect backMainMenu = v2ToRect({ (state.res.x / 2) - 200, (state.res.y * 7) / 8 - 68 }, { 400, 40 });
	drawRect(backMainMenu, { 0, 0, 0, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Back to Main Menu", { state.res.x / 2 , ((state.res.y * 7) / 8) - 48 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	if (mouseInRect(backMainMenu) && state.mouseState.click) {
		state.mode = MAIN_MENU;
		game.selectedScenario = "";
		return;
	}
}