#include "game.hpp"

int loadGame(std::string path) {
	std::cout << path << "\n";
	std::chrono::high_resolution_clock::time_point loadStart = std::chrono::high_resolution_clock::now();

	std::string line;

	//load meta data
	std::ifstream metaFile(path + "/meta.txt");
	if (!metaFile.good()) {
		return -1;
	}

	int ticks = 0;
	int running = 0;
	int selectedSpeed = 0;

	std::getline(metaFile, line);
	if (sscanf(line.c_str(), "%d %d %d", &ticks, &running, &selectedSpeed) != 3) {
		return -2;
	}

	metaFile.close();

	//load country data
	std::ifstream countryFile(path + "/country.txt");
	if (!countryFile.good()) {
		return -1;
	}

	while (std::getline(countryFile, line)) {
		std::string name = line;

		float money = 0.0f;
		int color = 0;
		std::getline(countryFile, line);
		if (sscanf(line.c_str(), "%f %d", &money, &color) != 2) {
			return -2;
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
		return -1;
	}

	while (std::getline(cityFile, line)) {
		std::string name = line;

		int owner = 0;
		int population = 0;
		int capitol = 0;
		v2<float> pos = { 0.0f, 0.0f };
		std::getline(cityFile, line);
		if (sscanf(line.c_str(), "%d %d %d %f %f", &owner, &population, &capitol, &pos.x, &pos.y) != 5) {
			return -2;
		}

		game.cities.push_back(city(name, pos, owner, capitol));
		game.cities[game.cities.size() - 1].population = population;

		std::getline(cityFile, line);
		if (line == "END") {
			countryFile.close();
			break;
		}
	}

	cityFile.close();

	//load industry data
	std::ifstream industryFile(path + "/industry.txt");
	if (!industryFile.good()) {
		return -1;
	}

	while (std::getline(industryFile, line)) {
		int ownerCity = 0;
		int type = 0;
		int workers = 0;
		if (sscanf(line.c_str(), "%d %d %d", &ownerCity, &type, &workers) != 3) {
			return -2;
		}

		game.cities[ownerCity].industries.push_back(industry(type));
		game.cities[ownerCity].industries[game.cities[ownerCity].industries.size() - 1].workers = workers;

		std::getline(industryFile, line);
		if (line == "END") {
			countryFile.close();
			break;
		}
	}

	industryFile.close();

	resetGameSettings();
	game.mode = NORMAL;
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

	metaFile << game.ticks << " " << game.running << " " << game.selectedSpeed;

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

	metaFile.close();
	countryEnd.close();
	cityEnd.close();
	industryEnd.close();
}