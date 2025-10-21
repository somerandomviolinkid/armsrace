#pragma once
#include "../state/state.hpp"

#include <chrono>
#include <iostream>
#include <ctime>

//menu portion

void drawMainMenu();
void mainMenuTick();

void drawLoadGameMenu();
void loadGameMenuTick();

//data portion

struct resourceData {
	std::string name;
	tex_t texture;

	resourceData(
		std::string resourceName
	) {
		name = resourceName;
		loadTexture(texture, makePNGFilePath(resourceName));
	}
};

struct industryData {
	std::string name;
	tex_t texture;
	int maxWorkers;
	std::map<int, float> inputs, outputs, storage;

	industryData(
		std::string industryName,
		int w,
		std::map<int, float> i,
		std::map<int, float> o,
		std::map<int, float> s
	) {
		name = industryName;
		loadTexture(texture, makePNGFilePath(industryName));
		maxWorkers = w;
		inputs = i;
		outputs = o;
		storage = s;
	}
};

class GameData {
public:
	std::vector<resourceData> resourceDatas;
	std::vector<industryData> industryDatas;

	void init();
	void quit();
};

extern GameData gameData;

//structs portion

struct industry {
	int type;
	int workers;
	std::map<int, float> inventory;

	industry(
		int t
	) {
		type = t;
		workers = 0;
		inventory = gameData.industryDatas[t].storage;
		for (std::pair<const int, float> &pair : inventory) {
			pair.second = 0.0f;
		}
	}

	void tick();
	void drawMenu();
};

struct city {
	std::string name;
	int population;
	v2<float> pos;
	std::vector<industry> industries;
	int owner;
	bool buildIndustryMenuOpen;

	city(
		std::string n,
		v2<float> p,
		int o
	) {
		name = n;
		population = 0;
		pos = p;
		industries = {};
		owner = o;
		buildIndustryMenuOpen = false;
	}

	void drawMenu();
	void drawBuildIndustryMenu();
	void draw(int i);
	void tick();
};

struct country {
	std::string name;

	country(
		std::string n
	) {
		name = n;
	}
};

//game menu portion

enum GAME_MODES {
	NORMAL,
	PAUSE_MENU,
	SAVE_GAME
};

void drawPauseMenu();
void pauseMenuTick();

void drawSaveGameMenu();
void saveGameMenuTick();

void drawCreditsMenu();

//game portion

class Game {
public:
	std::vector<country> countries;
	std::vector<city> cities;

	int ticks;
	
	void draw();
	void tick();

	struct {
		v2<float> pos;
		float zoom;
	} camera;

	void updateCamera();

	int mode;

	int selectedCity;
	int selectedIndustry;

	std::vector<SDL_Rect> occludeRects;

	std::chrono::high_resolution_clock::time_point tickStart;
	std::mt19937 gen;
};

extern Game game;

void newGame();
void clearGame();
void loadGame();
void saveGame();

//render portion
v2<float> project(v2<float> p);