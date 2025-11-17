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

		if (line[0] == 'r') {
			rawResources.push_back(rCounter);
		}

		resourceDatas.push_back(resourceData(line.substr(2)));
		rCounter++;
	}

	resourceFile.close();

	printf("Loaded %d resource datas.\n", (int)resourceDatas.size());

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

	printf("Loaded %d industry datas.\n", (int)industryDatas.size());

	//load storage data
	std::ifstream storageFile("assets/data/storage.txt");
	if (!storageFile.good()) {
		state.running = false;
		return;
	}

	while (std::getline(storageFile, line)) {
		std::string name = line;

		std::getline(storageFile, line);
		float maximumCapacity = std::stof(line);

		std::vector<int> resourcesToStore = {};
		while (std::getline(storageFile, line)) {
			int i;
			if (sscanf(line.c_str(), "%d", &i) == 1) {
				resourcesToStore.push_back(i);
			}

			if (line.empty() || line == "END") {
				storageDatas.push_back(storageData(name, maximumCapacity, resourcesToStore));
				if (line == "END") {
					storageFile.close();
					break;
				}

				break;
			}
		}
	}

	printf("Loaded %d storage datas.\n", (int)storageDatas.size());

	//load mine data
	std::ifstream mineFile("assets/data/mine.txt");
	if (!mineFile.good()) {
		state.running = false;
		return;
	}

	while (std::getline(mineFile, line)) {
		std::string name = line;

		std::getline(mineFile, line);
		int workers = std::stoi(line);

		std::map<int, float> o, s;
		while (std::getline(mineFile, line)) {
			char c;
			int a;
			float f;

			if (sscanf(line.c_str(), "%c %d %f", &c, &a, &f) == 3) {
				switch (c) {
				case 'o':
					o.insert({ a, f });
					break;
				case 's':
					s.insert({ a, f });
					break;
				}
			}

			if (line.empty() || line == "END") {
				mineDatas.push_back(mineData(name, workers, o, s));
				if (line == "END") {
					mineFile.close();
					break;
				}

				break;
			}
		}
	}

	printf("Loaded %d mine datas.\n", (int)mineDatas.size());

	//load scenario textures
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator("assets/scenarios")) {
		scenarioTextures.insert({ entry.path().string().substr(17), {}});
		for (const std::filesystem::directory_entry& subentry : std::filesystem::directory_iterator(entry.path().string())) {
			scenarioTextures[entry.path().string().substr(17)].push_back({});
			loadTexture(scenarioTextures[entry.path().string().substr(17)][scenarioTextures[entry.path().string().substr(17)].size() - 1], subentry.path().string());
		}
	}

	//load splashes
	std::vector<std::string> splashes = {};
	std::ifstream splashFile("assets/data/splash.txt");
	if (!splashFile.good()) {
		state.running = false;
		return;
	}

	while (std::getline(splashFile, line)) {
		splashes.push_back(line);
	}

	int s = (int)randf(game.gen, 0.0f, (float)splashes.size() - 0.1f);
	splashText = splashes[s];
	printf("Loaded game data in %lld microseconds.\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - loadStart).count());
}

void GameData::quit() {
	for (resourceData& r : resourceDatas) {
		SDL_DestroyTexture(r.texture.texture);
	}

	for (industryData& i : industryDatas) {
		SDL_DestroyTexture(i.texture.texture);
	}

	for (storageData& s : storageDatas) {
		SDL_DestroyTexture(s.texture.texture);
	}

	for (mineData& m : mineDatas) {
		SDL_DestroyTexture(m.texture.texture);
	}

	for (std::pair<const std::string, std::vector<tex_t>> pair : scenarioTextures) {
		for (tex_t& t : pair.second) {
			SDL_DestroyTexture(t.texture);
		}
	}
}

GameData gameData;