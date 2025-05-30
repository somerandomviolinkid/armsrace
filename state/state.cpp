#include "state.hpp"

State::State() {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	res = { 1280, 720 };
	window = SDL_CreateWindow("Arms Race", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, res.x, res.y, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	TTF_Font* font = TTF_OpenFont("assets/unifont.ttf", 16);
	for (int i = 0; i < 0xFF; i++) {
		SDL_Surface* surface = TTF_RenderGlyph_Solid(font, i, { 255, 255, 255, 255 });
		glyphAtlas[ i ] = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}

	TTF_CloseFont(font);

	baseTextures.push_back(IMG_LoadTexture(renderer, "assets/textures/debug.png"));
	baseTextures.push_back(IMG_LoadTexture(renderer, "assets/textures/city.png"));

	mouseState = {
		{0, 0},
		{0, 0},
		false,
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

	for (SDL_Texture*& t : glyphAtlas) {
		SDL_DestroyTexture(t);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void State::handleEvents() {
	SDL_PumpEvents();
	v2<int> tempMousePos = mouseState.pos;
	SDL_GetMouseState(&mouseState.pos.x, &mouseState.pos.y);
	mouseState.motion = mouseState.pos - tempMousePos;
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
				mouseState.down = true;
				mouseState.button = event.button.button;
				break;
			case SDL_MOUSEBUTTONUP:
				mouseState.down = false;
				break;
			case SDL_MOUSEWHEEL:
				mouseState.scroll = event.wheel.y;
				break;
		}
	}
}

State state;