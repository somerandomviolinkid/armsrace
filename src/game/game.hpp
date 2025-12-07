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

struct constructionData {
	int manHours;
	std::map<int, float> resourcesNeeded;
};

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
	//constructionData constructionReqs;

	industryData(
		std::string industryName,
		int w,
		std::map<int, float> i,
		std::map<int, float> o,
		std::map<int, float> s
		//int m,
		//std::map<int, float> c
	) {
		name = industryName;
		loadTexture(texture, makePNGFilePath(industryName));
		maxWorkers = w;
		inputs = i;
		outputs = o;
		storage = s;
		//constructionReqs = { m, c };
	}
};

struct storageData {
	std::string name;
	tex_t texture;
	float maxCapacity;
	std::vector<int> resourcesToStore;
	//constructionData constructionReqs;

	storageData(
		std::string storageName,
		float f,
		std::vector<int> i
		//int m,
		//std::map<int, float> c
	) {
		name = storageName;
		loadTexture(texture, makePNGFilePath(storageName));
		maxCapacity = f;
		resourcesToStore = i;
		//constructionReqs = { m, c };
	}
};

struct mineData {
	std::string name;
	tex_t texture;
	int maxWorkers;
	std::map<int, float> outputs, storage;
	//constructionData constructionReqs;

	mineData(
		std::string mineName,
		int w,
		std::map<int, float> o,
		std::map<int, float> s
		//int m,
		//std::map<int, float> c
	) {
		name = mineName;
		loadTexture(texture, makePNGFilePath(mineName));
		maxWorkers = w;
		outputs = o;
		storage = s;
		//constructionReqs = { m, c };
	}
};

class GameData {
public:
	std::vector<resourceData> resourceDatas;
	std::vector<industryData> industryDatas;
	std::vector<storageData> storageDatas;
	std::vector<mineData> mineDatas;

	std::vector<int> rawResources;

	std::string splashText;

	std::map<std::string, std::vector<tex_t>> scenarioTextures;

	void init();
	void quit();
};

extern GameData gameData;

// map

struct river {
	v2<int> a;
	v2<int> b;
	float width;
};

struct borderData {
	v2<int> pos;
	SDL_Color color;
};

struct map {
	std::vector<river> rivers;
};

//structs portion

struct exportData {
	int targetCity;
	int targetType;
	int targetIndex;
};

struct industry {
	int type;
	int workers;
	std::map<int, float> inventory;
	float resourceEfficiency;
	int ownerCity;

	std::map<int, std::vector<exportData>> exportDatas;
	std::map<int, bool> exportModes; //false = concentrate, true = disperse

	industry(
		int t,
		int o
	) {
		type = t;
		workers = 0;
		inventory = gameData.industryDatas[t].storage;
		for (std::pair<const int, float> &pair : inventory) {
			pair.second = 0.0f;
		}

		for (std::pair<const int, float>& pair : gameData.industryDatas[t].outputs) {
			exportDatas.insert({ pair.first, {} });
			exportModes[pair.first] = false;
		}

		resourceEfficiency = 0.0f;
		ownerCity = o;
	}

	void tick();
	void drawMenu();
	void drawImportMenu(int i);
	void drawExportMenu(int i);
	float ratioStored(int resourceIndex);
};

struct storage {
	int type;
	std::map<int, float> inventory;
	std::map<int, std::vector<exportData>> exportDatas;
	std::map<int, bool> exportModes;

	storage(
		int t
	) {
		type = t;
		for (int &i : gameData.storageDatas[t].resourcesToStore) {
			inventory.insert({ i, 0.0f });
			exportDatas.insert({ i, {} });
			exportModes[i] = false;
		}
	}

	void tick();
	void drawMenu();
	void drawExportMenu(int i);

	float totalStored();
	float ratioStored();
};

struct mine {
	int type;
	int workers;
	int workerCity;
	std::map<int, float> inventory;
	int owner;
	v2<float> pos;
	std::map<int, std::vector<exportData>> exportDatas;
	std::map<int, bool> exportModes;

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

		for (std::pair<const int, float>& pair : gameData.mineDatas[t].outputs) {
			exportDatas.insert({ pair.first, {} });
			exportModes[pair.first] = false;
		}

		owner = o;
		pos = p;
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
	bool capital;

	std::vector<industry> industries;
	std::vector<storage> storages;

	bool buildIndustryMenuOpen;
	bool buildStorageMenuOpen;

	float food;
	float water;

	city(
		std::string n,
		v2<float> p,
		int o,
		bool b
	) {
		name = n;
		population = 0;
		employed = 0;
		pos = p;
		industries = {};
		storages = {};
		owner = o;
		capital = b;
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
	float money;
	SDL_Color color;

	country(
		std::string n,
		float f,
		SDL_Color c
	) {
		name = n;
		money = f;
		color = c;
	}

	void drawMenu(int i);
	v2<float> getCenter(int i);
	v2<float> getPopulationCenter(int i);
	v2<float> getWesternMost(int i);
	v2<float> getEasternMost(int i);
	float getLength(int i);
};

int countryPopulation(int i);

//game menu portion

enum GAME_MODES {
	NORMAL,
	PAUSE_MENU,
	SAVE_GAME
};

void drawPauseMenu();
void pauseMenuTick();

void drawSaveGameMenu();

void drawCreateGameMenu();
void drawCreateRandomGameMenu();
void drawCreateScenarioGameMenu();
void drawChooseScenarioCountryGameMenu(std::string name);

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

	SDL_Texture* mapTexture;
	uint32_t* mapPixelArray;
	uint32_t* mapPixelBufferArray;
	
	void draw();
	void tick();

	struct {
		v2<float> pos;
		float zoom;
	} camera;

	void updateCamera();

	int playingCountry;

	bool cameraMoved;

	int mode;
	int selectedSpeed;

	bool running;
	int speedChangeTimer;

	int selectedCountry;

	int selectedCity;
	int selectedIndustry;
	int selectedResource;
	int selectedStorage;
	int selectedMine;
	bool selectingSomething();

	bool cityIndustryMenuOpen;
	bool cityStorageMenuOpen;

	bool industryInventoryMenuOpen;
	bool industryImportsMenuOpen;
	bool industryExportsMenuOpen;
	int industryImportResourceSelected;
	int industryExportResourceSelected;

	bool storageInventoryMenuOpen;
	bool storageExportsMenuOpen;
	int storageExportResourceSelected;

	bool mineInventoryMenuOpen;
	bool mineAllocateWorkersMenuOpen;
	int mineAllocateWorkersMenuScroll;
	bool mineExportsMenuOpen;
	int mineExportResourceSelected;

	void drawTopMenu();
	void drawMinimap();
	void generateAlerts();

	std::vector<SDL_Rect> occludeRects;

	int ticks;
	int frames;

	std::chrono::high_resolution_clock::time_point tickStart;
	std::chrono::high_resolution_clock::time_point frameStart;

	long long tickTime;
	long long frameTime;
	std::mt19937 gen;

	std::string selectedScenario;
};

extern Game game;

void newGame();
void clearGame();
int loadGame(std::string path);
void saveGame();
void resetGameSettings();

//render portion
v2<float> project(v2<float> p);
v2<float> aproject(v2<float> p);
v2<float> project(v2<float> p, v2<float> c, v2<int> res, float z);
v2<float> aproject(v2<float> p, v2<float> c, v2<int> res, float z);