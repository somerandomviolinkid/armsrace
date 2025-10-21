#include "game.hpp"

void GameData::init() {
	std::string line;

	std::ifstream resourceFile("assets/data/resource.txt");
	if (!resourceFile.good()) {
		state.running = false;
		return;
	}

	while (std::getline(resourceFile, line)) {
		if (line[0] == '#' || line.empty()) {
			continue;
		}

		resourceDatas.push_back(resourceData(line));
	}

	resourceFile.close();

	std::ifstream industryFile("assets/data/industry.txt");
	if (!industryFile.good()) {
		state.running = false;
		return;
	}

	while (std::getline(industryFile, line)) {
		std::string name = line;

		std::getline(industryFile, line);
		int workers = std::stoi(line);

		std::map<int, float> i, o, s;
		while (std::getline(industryFile, line)) {
			char c;
			int a;
			float f;

			if (sscanf(line.c_str(), "%c %d %f", &c, &a, &f) == 3) {
				switch (c) {
				case 'i':
					i.insert({ a, f });
					break;
				case 'o':
					o.insert({ a, f });
					break;
				case 's':
					s.insert({ a, f });
					break;
				}
			}

			if (line.empty() || line == "END") {
				industryDatas.push_back(industryData(name, workers, i, o, s));
				if (line == "END") {
					industryFile.close();
					return;
				}

				break;
			}
		}
	}
}

void GameData::quit() {
	for (resourceData& r : resourceDatas) {
		SDL_DestroyTexture(r.texture.texture);
	}

	for (industryData& i : industryDatas) {
		SDL_DestroyTexture(i.texture.texture);
	}
}

GameData gameData;