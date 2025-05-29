#include "game/game.hpp"

int main(int argc, char** argv) {
	gameData.loadTextures();
	while (state.running) {
		state.handleEvents();
		game.frame();
	}

	return 0;
}