#include "game.hpp"

v2<float> project(v2<float> p) {
	return ((p - game.camera.pos) * game.camera.zoom) + v2<float>{ ((float)state.res.x / (float)state.res.y) / 2.0f, 0.5f };
}

v2<float> aproject(v2<float> p) {
	return (p - v2<float>{ ((float)state.res.x / (float)state.res.y) / 2.0f, 0.5f } / game.camera.zoom) + game.camera.pos;
}

Game::Game() {
	cities.push_back(city("Bobbyville", { 0.0f, 0.0f }, 50000));
	cities[0].industries.push_back(industry(0));
	cities[ 0 ].industries.push_back(industry(1));

	cities[0].industries[0].workers = 50;
	cities[ 0 ].industries[ 1 ].workers = 25;

	cities[ 0 ].industries[ 0 ].inventory[0] = 5.0f;
	cities[ 0 ].industries[ 1 ].inventory[2] = 5.0f;

	camera = {
		{0.0f, 0.0f},
		1.0f
	};

	tickStart = std::chrono::high_resolution_clock::now();
}

void Game::frame() {
	std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();
	SDL_SetRenderDrawColor(state.renderer, 0, 127, 0, 255);
	SDL_RenderClear(state.renderer);

	for (city &c : cities) {
		c.draw();
	}

	SDL_RenderPresent(state.renderer);

	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	std::chrono::duration elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - tickStart);
	if (elapsed.count() > 1000) {
		tick();
	}

	std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration frameTime = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart);
	float dt = (float)frameTime.count() / 1000000.0f;

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

void Game::tick() {
	tickStart = std::chrono::high_resolution_clock::now();

	for (city& c : cities) {
		c.tick();
	}
}

Game game;