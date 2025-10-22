#include "game.hpp"

void GameData::init() {
	std::chrono::high_resolution_clock::time_point loadStart = std::chrono::high_resolution_clock::now();

	std::string line;

	//load resource data
	std::ifstream resourceFile("assets/data/resource.txt");
	if (!resourceFile.good()) {
		state.running = false;
		return;
	}

	int rCounter = 0;
	while (std::getline(resourceFile, line)) {
		if (line[0] == '#' || line.empty()) {
			continue;
		}

		switch (line[0]) {
		case 'r':
			rawResources.push_back(rCounter);
		}

		resourceDatas.push_back(resourceData(line.substr(2)));
		rCounter++;
	}

	resourceFile.close();

	//load industry data
	std::ifstream industryFile("assets/data/industry.txt");
	if (!industryFile.good()) {
		state.running = false;
		return;
	}

	while (std::getline(industryFile, line)) {
		if (line[0] == '#') {
			continue;
		}

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
					break;
				}

				break;
			}
		}
	}

	//load improvement data
	std::ifstream improvementFile("assets/data/improvement.txt");
	if (!improvementFile.good()) {
		state.running = false;
		return;
	}

	while (std::getline(improvementFile, line)) {
		std::string name = line;

		std::getline(improvementFile, line);
		int workers = std::stoi(line);

		std::map<int, float> o, s;
		while (std::getline(improvementFile, line)) {
			char c;
			int a;
			float f;

			if (sscanf(line.c_str(), "%c %d %f", &c, &a, &f) == 3) {
				switch (c) {
				case 'o':
					o.insert({ a, f });
					printf("output %d %.2f\n", a, f);
					break;
				case 's':
					s.insert({ a, f });
					printf("storage %d %.2f\n", a, f);
					break;
				}
			}

			if (line.empty() || line == "END") {
				improvementDatas.push_back(improvementData(name, workers, o, s));
				if (line == "END") {
					improvementFile.close();
					break;
				}

				break;
			}
		}
	}

	printf("Loaded game data in %lld microseconds.\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - loadStart).count());
}

void GameData::quit() {
	for (resourceData& r : resourceDatas) {
		SDL_DestroyTexture(r.texture.texture);
	}

	for (industryData& i : industryDatas) {
		SDL_DestroyTexture(i.texture.texture);
	}

	for (improvementData& i : improvementDatas) {
		SDL_DestroyTexture(i.texture.texture);
	}
}

GameData gameData;