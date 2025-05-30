#include "game.hpp"

v2<float> project(v2<float> p) {
	return ((p - game.camera.pos) * game.camera.zoom) + v2<float>{ ((float)state.res.x / (float)state.res.y) / 2.0f, 0.5f };
}

Game::Game() {
	cities.push_back(city("Bobbyville", { 0.0f, 0.0f }, 50000));

	cities[ 0 ].industries.push_back(industry(0));
	cities[ 0 ].industries.push_back(industry(1));
	cities[ 0 ].industries.push_back(industry(2));
	cities[ 0 ].industries.push_back(industry(3));

	cities[ 0 ].industries[ 0 ].workers = 50;
	cities[ 0 ].industries[ 1 ].workers = 25;

	cities[ 0 ].industries[ 0 ].inventory[ 0 ] = 5.0f;
	cities[ 0 ].industries[ 1 ].inventory[ 2 ] = 5.0f;

	menuOpen = 0;

	camera = {
		{0.0f, 0.0f},
		1.0f
	};

	tickStart = std::chrono::high_resolution_clock::now();
	fps = 0;
	lastFps = 0;
}

void Game::updateCamera(float dt, std::vector<SDL_Rect>& occludeRects) {
	bool notOccluded = true;
	SDL_Point p = { state.mouseState.pos.x, state.mouseState.pos.y };
	for (SDL_Rect r : occludeRects) {
		if (SDL_PointInRect(&p, &r)) {
			notOccluded = false;
			break;
		}
	}

	if (state.mouseState.down && notOccluded) {
		v2<float> mouseMove = { (float)state.mouseState.motion.x / (float)state.res.y, (float)state.mouseState.motion.y / (float)state.res.y };
		game.camera.pos = game.camera.pos - (mouseMove / camera.zoom);
	}

	if (state.keyboardState[ SDL_SCANCODE_W ]) {
		game.camera.pos.y -= dt / camera.zoom;
	}

	if (state.keyboardState[ SDL_SCANCODE_A ]) {
		game.camera.pos.x -= dt / camera.zoom;
	}

	if (state.keyboardState[ SDL_SCANCODE_S ]) {
		game.camera.pos.y += dt / camera.zoom;
	}

	if (state.keyboardState[ SDL_SCANCODE_D ]) {
		game.camera.pos.x += dt / camera.zoom;
	}

	camera.zoom *= powf(1.05f, (float)state.mouseState.scroll);
}

void Game::drawMenu() {
	SDL_Rect r = { 100, 100, state.res.x - 200, state.res.y - 200 };
	drawRectOutline(r, { 127, 127, 127, 255 }, { 0, 0, 0, 255 });

	SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
	SDL_RenderDrawLine(state.renderer, 100, 196, state.res.x - 100, 196);

	textTag tag = { 1.0f, {255, 255, 255, 255}, LEFT, MIDDLE };

	switch (menuOpen) {
		case 0: //arms race menu
			drawText({ "Welcome to Arms Race!", {state.res.x / 2, 148}, 4.0f, {255, 255, 255, 255}, CENTER, MIDDLE });
			drawText(makeTaggedText("Press ESCAPE or click anywhere outside this menu to close it. Press I to reopen this menu.", {108, 212}, tag));
			break;
		case 1: //country menu
			drawText({ "Country", {state.res.x / 2, 148}, 4.0f, {255, 255, 255, 255}, CENTER, MIDDLE });
			break;
		case 2: //economy menu
			drawText({ "Economy", {state.res.x / 2, 148}, 4.0f, {255, 255, 255, 255}, CENTER, MIDDLE });
			break;
		case 3: //military menu
			drawText({ "Military", {state.res.x / 2, 148}, 4.0f, {255, 255, 255, 255}, CENTER, MIDDLE });
			break;
	}
}

void Game::frame() {
	fps++;
	std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();

	SDL_SetRenderDrawColor(state.renderer, 0, 127, 0, 255);
	SDL_RenderClear(state.renderer);

	for (city& c : cities) {
		c.draw();
	}

	std::vector<SDL_Rect> occludeRects = {};
	for (city& c : cities) {
		if (c.menuOpen) {
			c.drawMenu();
			occludeRects.push_back({ (state.res.x * 3) / 4, 0, state.res.x / 4, state.res.y });
			break;
		}
	}

	if (state.keyboardState[ SDL_SCANCODE_ESCAPE ]) {
		menuOpen = -1;
	}

	if (state.keyboardState[ SDL_SCANCODE_I ]) {
		menuOpen = 0;
	}

	if (state.keyboardState[ SDL_SCANCODE_C ]) {
		menuOpen = 1;
	}

	if (state.keyboardState[ SDL_SCANCODE_E ]) {
		menuOpen = 2;
	}

	if (state.keyboardState[ SDL_SCANCODE_M ]) {
		menuOpen = 3;
	}

	if (menuOpen != -1) {
		drawMenu();
		SDL_Rect r = { 100, 100, state.res.x - 200, state.res.y - 200 };
		occludeRects.push_back(r);
		if (state.mouseState.click) {
			SDL_Point p = { state.mouseState.pos.x, state.mouseState.pos.y };
			if (!SDL_PointInRect(&p, &r)) {
				menuOpen = -1;
			}
		}
	}

	textTag tag = { 1.0f, {0, 0, 0, 255}, LEFT, MIDDLE };
	drawText(makeTaggedText(std::format("FPS: {}", game.lastFps), { 8, state.res.y - 48 }, tag));
	drawText(makeTaggedText(std::format("Camera location: ({:.2f},{:.2f})", game.camera.pos.x, game.camera.pos.y), { 8, state.res.y - 32 }, tag));
	drawText(makeTaggedText(std::format("Camera zoom: {:.3f}x", game.camera.zoom), { 8, state.res.y - 16 }, tag));

	SDL_RenderPresent(state.renderer);

	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	std::chrono::duration elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - tickStart);
	if (elapsed.count() > 1000) {
		tick();
	}

	std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration frameTime = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart);
	updateCamera((float)frameTime.count() / 1000000.0f, occludeRects);
}

void Game::tick() {
	tickStart = std::chrono::high_resolution_clock::now();

	for (city& c : cities) {
		c.tick();
	}

	lastFps = fps;
	fps = 0;
}

Game game;