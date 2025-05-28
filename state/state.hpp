#pragma once
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "../util/util.hpp"

class State {
public:
	State();
	~State();
	void handleEvents();

	v2<int> res;
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	std::vector<SDL_Texture*> baseTextures;

	struct {
		v2<int> pos;
		bool click;
		int button;

		int scroll;
	} mouseState;

	const uint8_t* keyboardState;

	bool running;
};

extern State state;

enum H_ALIGN {
	LEFT,
	CENTER,
	RIGHT
};

enum V_ALIGN {
	BOTTOM,
	MIDDLE,
	TOP
};

v2<int> calculateAlign(v2<int> pos, v2<int> dim, H_ALIGN hAlign, V_ALIGN vAlign);

void drawTexture(SDL_Texture* texture, v2<int> pos, float scale, H_ALIGN hAlign, V_ALIGN vAlign);

struct text {
	std::string content;
	v2<int> pos;
	float textScale;
	SDL_Color color;

	H_ALIGN hAlign;
	V_ALIGN vAlign;
};

void drawText(text t);