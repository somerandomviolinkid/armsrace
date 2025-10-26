#include "game.hpp"

void drawMainMenu() {
	drawText("Arms Race", { state.res.x / 2 , 144 }, 5.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Pre-Alpha 0.0.-1", { state.res.x / 2 , 216 }, 3.0f, {255, 255, 255, 255}, MIDDLE, CENTER);

	drawRect(v2ToRect({ (state.res.x / 2) - 108, (state.res.y / 2) - 28}, {216, (48 * 8) + 8}), {255, 255, 255, 255}, {64, 64, 64, 255});

	for (int i = 0; i < 8; i++) {
		drawRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20 + (48 * i)}, {200, 40}), {255, 255, 255, 255}, {128, 128, 128, 255}, {64, 64, 96, 255});
	}

	drawText("Create Game", { state.res.x / 2 , state.res.y / 2 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Load Game", { state.res.x / 2,  (state.res.y / 2) + 48 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Settings", { state.res.x / 2,  (state.res.y / 2) + 96 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Wiki", { state.res.x / 2,  (state.res.y / 2) + 144 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Credits", { state.res.x / 2,  (state.res.y / 2) + 192 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Discord", { state.res.x / 2,  (state.res.y / 2) + 240 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Github", { state.res.x / 2,  (state.res.y / 2) + 288 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("Quit", { state.res.x / 2,  (state.res.y / 2) + 336 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
}

void mainMenuTick() {
	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20}, { 200, 48 })) && state.mouseState.click) {
		state.mode = IN_GAME;
		newGame();
		return;
	}

	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20 + 48 }, { 200, 48 })) && state.mouseState.click) {
		state.mode = LOAD_GAME;
		return;
	}

	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20 + (48 * 3)}, {200, 48})) && state.mouseState.click) {
		SDL_OpenURL("https://github.com/somerandomviolinkid/armsrace/wiki");
		return;
	}

	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20 + (48 * 4) }, { 200, 48 })) && state.mouseState.click) {
		state.mode = CREDITS;
		return;
	}

	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20 + (48 * 5) }, { 200, 48 })) && state.mouseState.click) {
		SDL_OpenURL("https://discord.gg/NTmdbAKqEt");
		return;
	}

	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20 + (48 * 6) }, { 200, 48 })) && state.mouseState.click) {
		SDL_OpenURL("https://github.com/somerandomviolinkid/armsrace");
		return;
	}

	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 2) - 20 + (48 * 7)}, {200, 48})) && state.mouseState.click) {
		state.running = false;
		return;
	}
}

void drawLoadGameMenu() {
	drawText("Load Game", { state.res.x / 2 , 100 }, 3.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	const std::filesystem::path saves = "saves";
	int count = 0;
	for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(saves)) {
		count++;
	}

	int i = 0;
	drawRect(v2ToRect({ (state.res.x / 2) - 108, state.res.y / 4 - 28 }, { 216, 8 + (48 * count) }), { 255, 255, 255, 255 }, { 64, 64, 64, 255 });
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(saves)) {
		drawRect(v2ToRect({ (state.res.x / 2) - 100, (state.res.y / 4) - 20 + (48 * i) }, { 200, 40 }), { 255, 255, 255, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
		drawText(entry.path().string().substr(6), {state.res.x / 2 , (state.res.y / 4) + (48 * i)}, 2.0f, {255, 255, 255, 255}, MIDDLE, CENTER);
		i++;
	}

	drawRect(v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 - 20 }, { 300, 40 }), { 255, 255, 255, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Back to Main Menu", {state.res.x / 2 , (state.res.y * 7) / 8 }, 2.0f, {255, 255, 255, 255}, MIDDLE, CENTER);
}

void loadGameMenuTick() {
	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 - 20 }, { 300, 40 })) && state.mouseState.click) {
		state.mode = MAIN_MENU;
		return;
	}
}

void drawCreditsMenu() {
	drawText("Credits", { state.res.x / 2 , 100 }, 3.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	drawText("Lead Developer", { state.res.x / 2 - 32, 200 }, 2.0f, { 255, 255, 255, 255 }, RIGHT, CENTER);
	drawText("-", { state.res.x / 2, 200 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("bob", { state.res.x / 2 + 32, 200 }, 2.0f, { 255, 255, 255, 255 }, LEFT, CENTER);

	drawText("Lead Artist", { state.res.x / 2 - 32, 232 }, 2.0f, { 255, 255, 255, 255 }, RIGHT, CENTER);
	drawText("-", { state.res.x / 2, 232 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
	drawText("bob", { state.res.x / 2 + 32, 232 }, 2.0f, { 255, 255, 255, 255 }, LEFT, CENTER);

	SDL_Rect back = v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 - 20 }, { 300, 40 });
	drawRect(back, { 255, 255, 255, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Back to Main Menu", { state.res.x / 2 , (state.res.y * 7) / 8 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	if (mouseInRect(back) && state.mouseState.click) {
		state.mode = MAIN_MENU;
		return;
	}
}

void drawPauseMenu() {
	drawRect(v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 - 20 }, { 300, 40 }), { 255, 255, 255, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Back to Game", { state.res.x / 2 , (state.res.y * 7) / 8 }, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);

	drawRect(v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 + 28 }, { 300, 40 }), { 255, 255, 255, 255 }, { 128, 128, 128, 255 }, { 64, 64, 96, 255 });
	drawText("Back to Main Menu", { state.res.x / 2 , (state.res.y * 7) / 8 + 48}, 2.0f, { 255, 255, 255, 255 }, MIDDLE, CENTER);
}

void pauseMenuTick() {
	if (mouseInRect(v2ToRect({ (state.res.x / 2) - 150, (state.res.y * 7) / 8 - 20 }, { 300, 40 })) && state.mouseState.click) {
		game.mode = NORMAL;
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

void saveGameMenuTick() {

}