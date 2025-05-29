#pragma once
#include "../state/state.hpp"

#include <algorithm>
#include <chrono>
#include <map>
#include <iostream>

v2<float> project(v2<float> p);

v2<float> aproject(v2<float> p);

class resourceData {
public:
	std::string name;
	SDL_Texture* texture;

	resourceData(
		std::string n,
		std::string p
	) : name{ n },
		texture{ loadTexture_s(p) } {
	}
};

class industryData {
public:
	std::string name;
	SDL_Texture* texture;
	uint32_t maxWorkers;
	std::map<uint32_t, float> inputs, outputs, storage;

	industryData(
		std::string n,
		std::string p,
		uint32_t w,
		std::map<uint32_t, float> i,
		std::map<uint32_t, float> o,
		std::map<uint32_t, float> s
	) : name{ n },
		texture{ loadTexture_s(p) },
		maxWorkers{ w },
		inputs{ i },
		outputs{ o },
		storage{ s } {
		std::cout << p << "\n";
	}
};

class GameData {
public:
	std::vector<resourceData> resourceDatas;
	std::vector<industryData> industryDatas;

	GameData();
	~GameData();
};

extern GameData gameData;

class industry {
public:
	uint32_t type;
	uint32_t workers;
	std::map<uint32_t, float> inventory;

	industry(
		uint32_t t
	) : type{ t } {
		workers = 0;
		inventory = gameData.industryDatas[ type ].storage;
		for (std::pair<const uint32_t, float>& pair : inventory) {
			pair.second = 0.0f;
		}
	}

	void tick();
};

class city {
public:
	std::string name;
	v2<float> pos;
	float population;
	std::vector<industry> industries;

	bool menuOpen;

	city(
		std::string n,
		v2<float> p,
		float pop
	) : name{ n },
		pos{ p },
		population{ pop } {
		industries = {};
		menuOpen = false;
	}

	void tick();
	void drawMenu();
	void draw();
};

class Game {
public:
	Game();

	struct {
		v2<float> pos;
		float zoom;
	} camera;

	std::vector<city> cities;

	void frame();
	void tick();

	std::chrono::high_resolution_clock::time_point tickStart;
};

extern Game game;