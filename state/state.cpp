#include "state.hpp"

State::State() {
	printf("Started loading state\n");
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	res = { 1280, 720 };
	window = SDL_CreateWindow("Arms Race", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, res.x, res.y, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	font = TTF_OpenFont("assets/unifont.ttf", 16);

	baseTextures.push_back(IMG_LoadTexture(renderer, "assets/textures/debug.png"));
	baseTextures.push_back(IMG_LoadTexture(renderer, "assets/textures/city.png"));

	mouseState = {
		{0, 0},
		false,
		0,
		0
	};

	keyboardState = SDL_GetKeyboardState(NULL);

	running = true;
}

State::~State() {
	for (SDL_Texture*& t : baseTextures) {
		SDL_DestroyTexture(t);
	}

	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void State::handleEvents() {
	SDL_PumpEvents();
	SDL_GetMouseState(&mouseState.pos.x, &mouseState.pos.y);
	mouseState.click = false;
	mouseState.scroll = 0;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouseState.click = true;
				mouseState.button = event.button.button;
				break;
			case SDL_MOUSEWHEEL:
				mouseState.scroll = event.wheel.y;
				break;
		}
	}
}

State state;