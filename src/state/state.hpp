#pragma once
#include "../util/util.hpp"

#include <SDL.h>
#include <SDL_misc.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <fstream>
#include <vector>
#include <map>
#include <filesystem>

struct tex_t {
	SDL_Texture* texture;
	v2<int> dim;
};

enum STATE_MODES {
	MAIN_MENU,
	LOAD_GAME,
	SETTINGS,
	CREDITS,
	IN_GAME
};

//state portion

class State {
public:
	SDL_Window* window;
	SDL_Renderer* renderer;
	v2<int> res;

	tex_t fontAtlas[256];
	tex_t baseTextures[3];

	struct {
		v2<int> pos;
		v2<int> motion;

		bool click;
		bool down;

		int button;
		int scroll;
	} mouseState;

	const uint8_t* keyboardState;

	void init();
	void quit();
	void handleEvents();

	STATE_MODES mode;
	bool running;
};

extern State state;

//utils portion

SDL_Point v2ToPoint(v2<int> point);

v2<int> pointToV2(SDL_Point point);

bool mouseInRect(SDL_Rect r);

void drawLine(v2<int> a, v2<int> b, SDL_Color c);

void drawRect(SDL_Rect r, SDL_Color border, SDL_Color fill);

void drawRect(SDL_Rect r, SDL_Color border, SDL_Color fill, SDL_Color hover);

//texture portion

SDL_Rect v2ToRect(v2<int> pos, v2<int> dim);

enum H_ALIGN {
	LEFT,
	MIDDLE,
	RIGHT
};

enum V_ALIGN {
	BOTTOM,
	CENTER,
	TOP
};

v2<int> calculateAlign(v2<int> pos, v2<int> dim, H_ALIGN hAlign, V_ALIGN vAlign);

void loadTexture(tex_t &t, std::string path);

void drawTexture(tex_t texture, v2<int> pos, float scale, H_ALIGN hAlign, V_ALIGN vAlign);

//text portion

v2<int> queryText(std::string text, float scale);

void drawText(std::string text, v2<int> pos, float scale, SDL_Color color, H_ALIGN hAlign, V_ALIGN vAlign);