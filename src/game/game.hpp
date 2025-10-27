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

struct storageData {
	std::string name;
	tex_t texture;
	float maxCapacity;
	std::vector<int> resourcesToStore;

	storageData(
		std::string storageName,
		float f,
		std::vector<int> i
	) {
		name = storageName;
		loadTexture(texture, makePNGFilePath(storageName));
		maxCapacity = f;
		resourcesToStore = i;
	}
};

struct mineData {
	std::string name;
	tex_t texture;
	int maxWorkers;
	std::map<int, float> outputs, storage;

	mineData(
		std::string mineName,
		int w,
		std::map<int, float> o,
		std::map<int, float> s
	) {
		name = mineName;
		loadTexture(texture, makePNGFilePath(mineName));
		maxWorkers = w;
		outputs = o;
		storage = s;
	}
};

class GameData {
public:
	std::vector<resourceData> resourceDatas;
	std::vector<industryData> industryDatas;
	std::vector<storageData> storageDatas;
	std::vector<mineData> mineDatas;

	std::vector<int> rawResources;

	void init();
	void quit();
};

extern GameData gameData;

//structs portion

struct industry {
	int type;
	int workers;
	std::map<int, float> inventory;
	float resourceEfficiency;

	industry(
		int t
	) {
		type = t;
		workers = 0;
		inventory = gameData.industryDatas[t].storage;
		for (std::pair<const int, float> &pair : inventory) {
			pair.second = 0.0f;
		}

		resourceEfficiency = 0.0f;
	}

	void tick();
	void drawMenu();
};

struct storage {
	int type;
	std::map<int, float> inventory;

	storage(
		int t
	) {
		type = t;
		for (int &i : gameData.storageDatas[t].resourcesToStore) {
			inventory.insert({ i, 0.0f });
		}
	}

	void tick();
	void drawMenu();

	float totalStored();
	float ratioStored();
};

struct exportData {
	int index;
	int targetCity;
	int targetType;
	int targetIndex;
};

struct mine {
	int type;
	int workers;
	int workerCity;
	std::map<int, float> inventory;
	int owner;
	v2<float> pos;
	std::vector<exportData> exportDatas;

	mine(
		int t,
		int o,
		v2<float> p
	) {
		type = t;
		workers = 0;
		workerCity = -1;
		inventory = gameData.mineDatas[t].storage;
		for (std::pair<const int, float>& pair : inventory) {
			pair.second = 0.0f;
		}

		owner = o;
		pos = p;

		exportDatas = {};
	}

	void tick();
	void draw(int i);
	void drawMenu();

	void drawExportMenu(int i);
};

struct city {
	std::string name;
	int population;
	int employed;

	v2<float> pos;
	int owner;

	std::vector<industry> industries;
	std::vector<storage> storages;

	bool buildIndustryMenuOpen;
	bool buildStorageMenuOpen;

	city(
		std::string n,
		v2<float> p,
		int o
	) {
		name = n;
		population = 0;
		employed = 0;
		pos = p;
		industries = {};
		storages = {};
		owner = o;
		buildIndustryMenuOpen = false;
		buildStorageMenuOpen = false;
	}

	void drawMenu();
	void drawBuildIndustryMenu();
	void drawBuildStorageMenu();

	void draw(int i);
	void tick();
};

struct country {
	std::string name;
	SDL_Color color;

	country(
		std::string n
	) {
		name = n;
		color = { 0, 0, 0, 255 };
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

void drawTutorialMenu();

//natural resource portion
struct naturalResource {
	int type;
	v2<float> pos;

	naturalResource(
		int i,
		v2<float> p
	) {
		type = i;
		pos = p;
	}

	void draw(int i);
	void drawMenu();
};

//game portion

class Game {
public:
	std::vector<country> countries;
	std::vector<city> cities;
	std::vector<mine> mines;
	std::vector<naturalResource> naturalResources;

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
	int selectedResource;
	int selectedStorage;
	int selectedMine;
	bool selectingSomething();

	bool cityIndustryMenuOpen;
	bool cityStorageMenuOpen;

	bool industryInventoryMenuOpen;
	bool storageInventoryMenuOpen;
	bool mineInventoryMenuOpen;
	
	bool mineAllocateWorkersMenuOpen;
	bool mineExportsMenuOpen;
	int mineExportResourceSelected;

	void drawTopMenu();

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