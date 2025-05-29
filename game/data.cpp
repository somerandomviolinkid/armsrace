#include "game.hpp"

GameData::GameData() {
	printf("Started loading game data\n");
	std::string line;

	std::ifstream resourceFile("assets/data/resource.txt");
	while (std::getline(resourceFile, line)) {
		std::ifstream currentResourceFile("assets/data/" + line + ".txt");

		std::getline(currentResourceFile, line);
		std::string name = line;

		std::getline(currentResourceFile, line);
		std::string texturePath = line;

		resourceDatas.push_back(resourceData(name, texturePath));
		currentResourceFile.close();
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

GameData gameData();