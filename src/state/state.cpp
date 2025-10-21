#include "state.hpp"

void State::init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	Mix_Init(MIX_INIT_MP3);

	window = SDL_CreateWindow("Arms Race", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
	SDL_GetWindowSize(window, &res.x, &res.y);
	SDL_SetWindowResizable(window, SDL_FALSE);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	TTF_Font* font = TTF_OpenFont("assets/fonts/unifont.ttf", 16);
	for (int i = 0; i < 0xFF; i++) {
		SDL_Surface* s = TTF_RenderGlyph_Solid(font, i, { 255, 255, 255, 255 });
		fontAtlas[i].texture = SDL_CreateTextureFromSurface(renderer, s);
		SDL_FreeSurface(s);
		SDL_QueryTexture(fontAtlas[i].texture, NULL, NULL, &fontAtlas[i].dim.x, &fontAtlas[i].dim.y);
	}

	TTF_CloseFont(font);

	loadTexture(baseTextures[0], "assets/textures/city.png");
	loadTexture(baseTextures[1], "assets/textures/build.png");

	mouseState = {
		{0, 0},
		{0, 0},
		false,
		false,
		0,
		0
	};

	keyboardState = SDL_GetKeyboardState(NULL);

	std::filesystem::path savesPath = "saves";
	if (std::filesystem::is_directory(savesPath)) {
		std::filesystem::create_directory(savesPath);
	}

	mode = MAIN_MENU;
	running = true;
}

void State::quit() {
	for (int i = 0; i < 2; i++) {
		SDL_DestroyTexture(baseTextures[i].texture);
	}

	for (int i = 0; i < 0xFF; i++) {
		SDL_DestroyTexture(fontAtlas[i].texture);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	Mix_Quit();
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