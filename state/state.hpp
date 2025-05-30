#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

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

	SDL_Texture* glyphAtlas[255];

	std::vector<SDL_Texture*> baseTextures;

	struct {
		v2<int> pos;
		v2<int> motion;
		bool click;
		bool down;
		int button;
		int scroll;
	} mouseState;

	const uint8_t* keyboardState;

	bool running;
};

extern State state;

void drawRectOutline(SDL_Rect r, SDL_Color fill, SDL_Color outline);
//uint32_t color2int(SDL_Color c);
//SDL_Color int2color(uint32_t u);

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

SDL_Texture* loadTexture_s(std::string path);

void drawTexture(SDL_Texture* texture, v2<int> pos, float scale, H_ALIGN hAlign, V_ALIGN vAlign);

struct text {
	std::string content;
	v2<int> pos;
	float textScale;
	SDL_Color color;

	H_ALIGN hAlign;
	V_ALIGN vAlign;
};

struct textTag {
	float textScale;
	SDL_Color color;

	H_ALIGN hAlign;
	V_ALIGN vAlign;
};

text makeTaggedText(std::string content, v2<int> pos, textTag tag);

void drawText(text t);

struct button {
	text t;
	v2<int> pos;

	H_ALIGN hAlign;
	V_ALIGN vAlign;
};

void drawButton(button b);