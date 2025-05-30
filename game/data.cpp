#include "game.hpp"

GameData::GameData() {
	std::string line;

	std::ifstream resourceFile("assets/data/resource.txt");
	while (std::getline(resourceFile, line)) {
		std::string name = line;

		std::getline(resourceFile, line);
		std::string texturePath = "assets/textures/" + line;

		resourceDatas.push_back(resourceData(name, texturePath));

		//organization, might remove later
		std::getline(resourceFile, line);
	}

	resourceFile.close();

	std::ifstream industryFile("assets/data/industry.txt");
	while (std::getline(industryFile, line)) {
		std::ifstream currentIndustryFile("assets/data/" + line + ".txt");

		std::getline(currentIndustryFile, line);
		std::string name = line;

		std::getline(currentIndustryFile, line);
		std::string texturePath = "assets/textures/" + line;

		std::getline(currentIndustryFile, line);
		uint32_t workers = std::stoi(line);

		std::map<uint32_t, float> i, o, s;
		while (std::getline(currentIndustryFile, line)) {
			char c;
			uint32_t u;
			float f;

			if (sscanf(line.c_str(), "%c %u %f", &c, &u, &f) == 3) {
				switch (c) {
					case 'i':
						i.insert({ u, f });
						break;
					case 'o':
						o.insert({ u, f });
						break;
					case 's':
						s.insert({ u, f });
						break;
				}
			}
		}

		industryDatas.push_back(industryData(name, texturePath, workers, i, o, s));
		currentIndustryFile.close();
	}

	industryFile.close();
}

GameData::~GameData() {
	for (resourceData& r : resourceDatas) {
		SDL_DestroyTexture(r.texture);
	}

	for (industryData& i : industryDatas) {
		SDL_DestroyTexture(i.texture);
	}
}

void GameData::loadTextures() {
	for (resourceData &r : resourceDatas) {
		r.texture = loadTexture_s(r.texturePath);
	}

	for (industryData& i : industryDatas) {
		i.texture = loadTexture_s(i.texturePath);
	}
}

GameData gameData;