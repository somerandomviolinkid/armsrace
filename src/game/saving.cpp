#include "game.hpp"

int loadGame(std::string path) {
	std::chrono::high_resolution_clock::time_point loadStart = std::chrono::high_resolution_clock::now();

	std::string line;

	//load meta data
	std::ifstream metaFile(path + "/meta.txt");
	if (!metaFile.good()) {
		return -2;
	}

	std::getline(metaFile, line);
	std::string version = line;
	if (line != "Nov 8 Patch 1") {
		return -1;
	}

	int ticks = 0;
	int running = 0;
	int selectedSpeed = 0;
	int playingCountry = 0;

	std::getline(metaFile, line);
	if (sscanf(line.c_str(), "%d %d %d %d", &ticks, &running, &selectedSpeed, &playingCountry) != 4) {
		return -3;
	}

	metaFile.close();

	game.ticks = ticks;
	game.running = (bool)running;
	game.selectedSpeed = selectedSpeed;
	game.playingCountry = playingCountry;

	//load country data
	std::ifstream countryFile(path + "/country.txt");
	if (!countryFile.good()) {
		return -2;
	}

	while (std::getline(countryFile, line)) {
		if (line == "END") {
			countryFile.close();
			break;
		}

		std::string name = line;

		float money = 0.0f;
		int color = 0;
		std::getline(countryFile, line);
		if (sscanf(line.c_str(), "%f %d", &money, &color) != 2) {
			return -3;
		}

		game.countries.push_back(country(name, money, int2Color(color)));

		std::getline(countryFile, line);
		if (line == "END") {
			countryFile.close();
			break;
		}
	}

	countryFile.close();

	//load city data
	std::ifstream cityFile(path + "/city.txt");
	if (!cityFile.good()) {
		return -2;
	}

	while (std::getline(cityFile, line)) {
		if (line == "END") {
			cityFile.close();
			break;
		}

		std::string name = line;

		int owner = 0;
		int population = 0;
		int capitol = 0;
		v2<float> pos = { 0.0f, 0.0f };
		std::getline(cityFile, line);
		if (sscanf(line.c_str(), "%d %d %d %f %f", &owner, &population, &capitol, &pos.x, &pos.y) != 5) {
			return -3;
		}

		game.cities.push_back(city(name, pos, owner, capitol));
		game.cities[game.cities.size() - 1].population = population;

		std::getline(cityFile, line);
		if (line == "END") {
			cityFile.close();
			break;
		}
	}

	cityFile.close();

	for (int i = 1; i < game.cities.size(); i++) {
		game.cities[i].pos = game.cities[i].pos - game.cities[0].pos;
	}

	game.cities[0].pos = game.cities[0].pos - game.cities[0].pos;

	//load industry data
	std::ifstream industryFile(path + "/industry.txt");
	if (!industryFile.good()) {
		return -2;
	}

	while (std::getline(industryFile, line)) {
		if (line == "END") {
			industryFile.close();
			break;
		}

		int ownerCity = 0;
		int type = 0;
		int workers = 0;
		if (sscanf(line.c_str(), "%d %d %d", &ownerCity, &type, &workers) != 3) {
			return -3;
		}

		game.cities[ownerCity].industries.push_back(industry(type, ownerCity));
		game.cities[ownerCity].industries[game.cities[ownerCity].industries.size() - 1].workers = workers;

		std::getline(industryFile, line);
		if (line == "END") {
			industryFile.close();
			break;
		}
	}

	industryFile.close();

	//load storage data
	std::ifstream storageFile(path + "/storage.txt");
	if (!storageFile.good()) {
		return -2;
	}

	while (std::getline(storageFile, line)) {
		if (line == "END") {
			storageFile.close();
			break;
		}

		int ownerCity = 0;
		int type = 0;
		if (sscanf(line.c_str(), "%d %d", &ownerCity, &type) != 2) {
			return -3;
		}

		game.cities[ownerCity].storages.push_back(storage(type));

		std::getline(storageFile, line);
		if (line == "END") {
			storageFile.close();
			break;
		}
	}

	storageFile.close();

	//load mine data
	std::ifstream mineFile(path + "/mine.txt");
	if (!mineFile.good()) {
		return -2;
	}

	while (std::getline(mineFile, line)) {
		if (line == "END") {
			mineFile.close();
			break;
		}

		int owner = 0;
		int type = 0;
		int workerCity = 0;
		int workers = 0;
		v2<float> pos = { 0.0f, 0.0f };
		if (sscanf(line.c_str(), "%d %d %d %d %f %f", &owner, &type, &workerCity, &workers, &pos.x, &pos.y) != 6) {
			return -3;
		}

		game.mines.push_back(mine(type, owner, pos));
		game.mines[game.mines.size() - 1].workerCity = workerCity;
		game.mines[game.mines.size() - 1].workers = workers;

		std::getline(mineFile, line);
		if (line == "END") {
			mineFile.close();
			break;
		}
	}

	storageFile.close();

	//load natural resource data
	std::ifstream resourceFile(path + "/resource.txt");
	if (!resourceFile.good()) {
		return -2;
	}

	while (std::getline(resourceFile, line)) {
		if (line == "END") {
			resourceFile.close();
			break;
		}

		int type = 0;
		v2<float> pos = { 0.0f, 0.0f };
		if (sscanf(line.c_str(), "%d %f %f", &type, &pos.x, &pos.y) != 3) {
			return -3;
		}

		game.naturalResources.push_back(naturalResource(type, pos));

		std::getline(resourceFile, line);
		if (line == "END") {
			resourceFile.close();
			break;
		}
	}

	resourceFile.close();

	resetGameSettings();
	game.mode = NORMAL;
	printf("Loaded game in %lld microseconds.\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - loadStart).count());
	return 0;
}

void saveGame() {
	time_t currentTime;
	std::time(&currentTime);
	std::string currentTimeString = ctime(&currentTime);
	std::replace(currentTimeString.begin(), currentTimeString.end(), ' ', '_');
	std::replace(currentTimeString.begin(), currentTimeString.end(), ':', '_');
	currentTimeString.pop_back(); //remove trailing \n

	std::filesystem::path savePath = "saves/" + currentTimeString;
	if (!std::filesystem::is_directory(savePath)) {
		std::filesystem::create_directory(savePath);
	}

	std::ofstream metaFile(savePath.string() + "/meta.txt");
	std::ofstream countryFile(savePath.string() + "/country.txt");
	std::ofstream cityFile(savePath.string() + "/city.txt");
	std::ofstream industryFile(savePath.string() + "/industry.txt");
	std::ofstream storageFile(savePath.string() + "/storage.txt");
	std::ofstream mineFile(savePath.string() + "/mine.txt");
	std::ofstream resourceFile(savePath.string() + "/resource.txt");

	metaFile << "Nov 8 Patch 1\n" << game.ticks << " " << game.running << " " << game.selectedSpeed << " " << game.playingCountry;

	for (int c = 0; c < game.countries.size(); c++) {
		countryFile << game.countries[c].name << "\n";
		countryFile << game.countries[c].money << " " << color2Int(game.countries[c].color) << "\n\n";
	}

	for (int c = 0; c < game.cities.size(); c++) {
		cityFile << game.cities[c].name << "\n";
		cityFile << game.cities[c].owner << " " << game.cities[c].population << " " << game.cities[c].capital << " " << game.cities[c].pos.x << " " << game.cities[c].pos.y << "\n\n";

		for (industry& i : game.cities[c].industries) {
			industryFile << c << " " << i.type << " " << i.workers << "\n\n";
		}

		for (storage& s : game.cities[c].storages) {
			storageFile << c << " " << s.type << "\n\n";
		}
	}

	for (int m = 0; m < game.mines.size(); m++) {
		mineFile << game.mines[m].owner << " " << game.mines[m].type << " " << game.mines[m].workerCity << " " << game.mines[m].workers << " " << game.mines[m].pos.x << " " << game.mines[m].pos.y << "\n\n";
	}

	for (int r = 0; r < game.naturalResources.size(); r++) {
		resourceFile << game.naturalResources[r].type << " " << game.naturalResources[r].pos.x << " " << game.naturalResources[r].pos.y << "\n\n";
	}

	countryFile.close();
	removeLastByte(savePath.string() + "/country.txt", 1);
	std::fstream countryEnd(savePath.string() + "/country.txt");
	countryEnd.seekp(0, std::ios::end);
	countryEnd << "END";

	cityFile.close();
	removeLastByte(savePath.string() + "/city.txt", 1);
	std::fstream cityEnd(savePath.string() + "/city.txt");
	cityEnd.seekp(0, std::ios::end);
	cityEnd << "END";

	industryFile.close();
	removeLastByte(savePath.string() + "/industry.txt", 1);
	std::fstream industryEnd(savePath.string() + "/industry.txt");
	industryEnd.seekp(0, std::ios::end);
	industryEnd << "END";

	storageFile.close();
	removeLastByte(savePath.string() + "/storage.txt", 1);
	std::fstream storageEnd(savePath.string() + "/storage.txt");
	storageEnd.seekp(0, std::ios::end);
	storageEnd << "END";

	mineFile.close();
	removeLastByte(savePath.string() + "/mine.txt", 1);
	std::fstream mineEnd(savePath.string() + "/mine.txt");
	mineEnd.seekp(0, std::ios::end);
	mineEnd << "END";

	resourceFile.close();
	removeLastByte(savePath.string() + "/resource.txt", 1);
	std::fstream resourceEnd(savePath.string() + "/resource.txt");
	resourceEnd.seekp(0, std::ios::end);
	resourceEnd << "END";

	metaFile.close();
	countryEnd.close();
	cityEnd.close();
	industryEnd.close();
	storageEnd.close();
	mineEnd.close();
	resourceEnd.close();
}