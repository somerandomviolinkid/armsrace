#include "game.hpp"

void city::drawMenu() {
	drawRect(v2ToRect({ (state.res.x * 3) / 4, 64 }, { state.res.x / 4, state.res.y - 64}), { 0, 0, 0, 255 }, { 192, 192, 192, 255 });

	int yOffset = 92;
	float scale = name.length() > 18 ? 2.0f : 3.0f;
	drawText(name, { (state.res.x * 7) / 8, yOffset }, scale, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	yOffset += 48;
	drawText("Owner: ", {((state.res.x * 3) / 4) + 16, yOffset }, 2.0f, {0, 0, 0, 255}, LEFT, CENTER);
	drawText(game.countries[owner].name, { state.res.x - 16, yOffset }, 2.0f, game.countries[owner].color, RIGHT, CENTER);

	yOffset += 40;
	drawText("Population: ", { ((state.res.x * 3) / 4) + 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
	drawText(formatNumber((float)population), { state.res.x - 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);

	yOffset += 40;
	drawText("Employed: ", { ((state.res.x * 3) / 4) + 16, yOffset }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
	drawText(formatNumber((float)employed) + " / " + formatNumber((float)population / 2.0f), {state.res.x - 16, yOffset }, 2.0f, {0, 0, 0, 255}, RIGHT, CENTER);

	yOffset += 24;
	SDL_Rect industryHeaderRect = v2ToRect({ (state.res.x * 3) / 4 , yOffset }, { state.res.x / 4, 64 });
	drawRect(industryHeaderRect, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 255 });
	if (mouseInRect(industryHeaderRect) && state.mouseState.click) {
		if (game.cityIndustryMenuOpen) {
			game.cityIndustryMenuOpen = false;
		} else {
			game.cityIndustryMenuOpen = true;
		}
	}

	yOffset += 32;
	drawText("Industries", { (state.res.x * 7) / 8, yOffset }, 3.0f, {0, 0, 0, 255}, MIDDLE, CENTER);

	if (game.cityIndustryMenuOpen) {
		drawTexture(state.baseTextures[MENU_OPEN], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	} else {
		drawTexture(state.baseTextures[MENU_CLOSED], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	}

	int industryHover = -1;
	bool buildIndustryHover = false;
	if (game.cityIndustryMenuOpen) {
		int i = 0;

		yOffset += 40;
		for (i = 0; i < industries.size(); i++) {
			SDL_Rect r = v2ToRect({ ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + yOffset }, { 64, 64 });
			drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 255 });
			drawTexture(gameData.industryDatas[industries[i].type].texture, { ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + yOffset }, 4.0f, LEFT, BOTTOM);
			drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 0 }, { 128, 128, 192, 64 });

			if (mouseInRect(r)) {
				industryHover = i;
			}
		}

		if (owner == 0) {
			SDL_Rect r = v2ToRect({ ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + yOffset }, { 64, 64 });
			drawRect(r, { 0, 0, 0, 255 }, { 64, 64, 64, 255 });
			drawTexture(state.baseTextures[BUILD], { ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + yOffset }, 4.0f, LEFT, BOTTOM);
			drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 0 }, { 128, 128, 192, 64 });

			yOffset += 80 * ((i / 5) + 1);

			if (mouseInRect(r)) {
				buildIndustryHover = true;
			}
		}
	} else {
		yOffset += 32;
	}

	SDL_Rect storageHeaderRect = v2ToRect({ (state.res.x * 3) / 4 , yOffset }, { state.res.x / 4, 64 });
	drawRect(storageHeaderRect, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 255 });
	if (mouseInRect(storageHeaderRect) && state.mouseState.click) {
		if (game.cityStorageMenuOpen) {
			game.cityStorageMenuOpen = false;
		} else {
			game.cityStorageMenuOpen = true;
		}
	}

	yOffset += 32;
	drawText("Storages", { (state.res.x * 7) / 8, yOffset }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	if (game.cityStorageMenuOpen) {
		drawTexture(state.baseTextures[MENU_OPEN], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	} else {
		drawTexture(state.baseTextures[MENU_CLOSED], { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, CENTER);
	}

	int storageHover = -1;
	bool buildStorageHover = false;
	if (game.cityStorageMenuOpen) {
		int i = 0;

		yOffset += 40;
		for (i = 0; i < storages.size(); i++) {
			SDL_Rect r = v2ToRect({ ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + yOffset }, { 64, 64 });
			drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 255 });
			drawTexture(gameData.storageDatas[storages[i].type].texture, { ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + yOffset }, 4.0f, LEFT, BOTTOM);
			drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 0 }, { 128, 128, 192, 64 });

			if (mouseInRect(r)) {
				storageHover = i;
			}
		}

		if (owner == 0) {
			SDL_Rect r = v2ToRect({ ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + yOffset }, { 64, 64 });
			drawRect(r, { 0, 0, 0, 255 }, { 64, 64, 64, 255 });
			drawTexture(state.baseTextures[BUILD], { ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + yOffset }, 4.0f, LEFT, BOTTOM);
			drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 0 }, { 128, 128, 192, 64 });

			if (mouseInRect(r)) {
				buildStorageHover = true;
			}
		}
	}

	if (industryHover != -1) {
		std::string n = gameData.industryDatas[industries[industryHover].type].name;
		std::string w = std::format("{} / {} workers", industries[industryHover].workers, gameData.industryDatas[industries[industryHover].type].maxWorkers);
		std::string e = std::format("{:.1f}% efficiency", ((float)industries[industryHover].workers * 100.0f * industries[industryHover].resourceEfficiency) / (float)gameData.industryDatas[industries[industryHover].type].maxWorkers);

		std::vector<int> widths = { queryText(n, 1.0f).x, queryText(e, 1.0f).x, queryText(w, 1.0f).x };
		int max = *std::max_element(widths.begin(), widths.end());

		int xOffset = 16;
		if (state.mouseState.pos.x + max + xOffset + 16 > state.res.x) {
			xOffset = -max - 16;
		}

		SDL_Rect textOutline = v2ToRect(state.mouseState.pos + v2<int>{xOffset, 16}, v2<int>{max, 18} + v2<int>{8, 60});
		drawRect(textOutline, { 0, 0, 0, 255 }, { 32, 32, 32, 192 });
		drawText(n, state.mouseState.pos + v2<int>{xOffset + 4, 20}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);
		drawText(w, state.mouseState.pos + v2<int>{xOffset + 4, 44}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);
		drawText(e, state.mouseState.pos + v2<int>{xOffset + 4, 68}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);

		if (state.mouseState.click) {
			game.selectedIndustry = industryHover;
		}
	}

	if (storageHover != -1) {
		std::string n = gameData.storageDatas[storages[storageHover].type].name;
		std::string s = std::format("{:.1f}% filled", storages[storageHover].ratioStored() * 100.0f);

		std::vector<int> widths = { queryText(n, 1.0f).x, queryText(s, 1.0f).x };
		int max = *std::max_element(widths.begin(), widths.end());

		int xOffset = 16;
		if (state.mouseState.pos.x + max + xOffset + 16 > state.res.x) {
			xOffset = -max - 16;
		}

		SDL_Rect textOutline = v2ToRect(state.mouseState.pos + v2<int>{xOffset, 16}, v2<int>{max, 18} + v2<int>{8, 36});
		drawRect(textOutline, { 0, 0, 0, 255 }, { 32, 32, 32, 192 });
		drawText(n, state.mouseState.pos + v2<int>{xOffset + 4, 20}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);
		drawText(s, state.mouseState.pos + v2<int>{xOffset + 4, 44}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);

		if (state.mouseState.click) {
			game.selectedStorage = storageHover;
		}
	}

	if (buildIndustryHover) {
		std::string buildText = "Build Industry";
		v2<int> dim = queryText(buildText, 1.0f);
		int xOffset = 16;
		if (state.mouseState.pos.x + dim.x + 16 > state.res.x) {
			xOffset = -dim.x - 16;
		}

		SDL_Rect textOutline = v2ToRect(state.mouseState.pos + v2<int>{xOffset, 16}, v2<int>{dim.x, 18} + v2<int>{8, 8});
		drawRect(textOutline, { 0, 0, 0, 255 }, { 32, 32, 32, 192 });
		drawText(buildText, state.mouseState.pos + v2<int>{xOffset + 4, 20}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);

		if (state.mouseState.click) {
			buildIndustryMenuOpen = true;
			return;
		}
	}

	if (buildStorageHover) {
		std::string buildText = "Build Storage";
		v2<int> dim = queryText(buildText, 1.0f);
		int xOffset = 16;
		if (state.mouseState.pos.x + dim.x + 16 > state.res.x) {
			xOffset = -dim.x - 16;
		}

		SDL_Rect textOutline = v2ToRect(state.mouseState.pos + v2<int>{xOffset, 16}, v2<int>{dim.x, 18} + v2<int>{8, 8});
		drawRect(textOutline, { 0, 0, 0, 255 }, { 32, 32, 32, 192 });
		drawText(buildText, state.mouseState.pos + v2<int>{xOffset + 4, 20}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);

		if (state.mouseState.click) {
			buildStorageMenuOpen = true;
			return;
		}
	}
}

void city::drawBuildIndustryMenu() {
	drawRect(v2ToRect({ (state.res.x * 3) / 4, 0 }, { state.res.x / 4, state.res.y }), { 0, 0, 0, 255 }, { 192, 192, 192, 255 });

	drawText("Build Industry", {(state.res.x * 7) / 8, 32}, 3.0f, {0, 0, 0, 255}, MIDDLE, CENTER);

	std::vector<bool> builtIndustries = {};
	for (int i = 0; i < gameData.industryDatas.size(); i++) {
		builtIndustries.push_back(false);
	}

	for (industry& i : industries) {
		builtIndustries[i.type] = true;
	}

	std::vector<int> unbuiltIndustries = {};
	for (int i = 0; i < builtIndustries.size(); i++) {
		if (!builtIndustries[i]) {
			unbuiltIndustries.push_back(i);
		}
	}

	int hover = -1;
	for (int i = 0; i < unbuiltIndustries.size(); i++) {
		SDL_Rect r = v2ToRect({ ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + 216 }, { 64, 64 });
		drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 255 });
		drawTexture(gameData.industryDatas[unbuiltIndustries[i]].texture, { ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + 216 }, 4.0f, LEFT, BOTTOM);
		drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 0 }, { 128, 128, 192, 64 });

		if (mouseInRect(r)) {
			hover = unbuiltIndustries[i];
		}
	}

	if (hover != -1) {
		std::string n = "Build " + gameData.industryDatas[hover].name;
		v2<int> dim = queryText(n, 1.0f);
		int xOffset = 16;
		if (state.mouseState.pos.x + dim.x + xOffset + 16 > state.res.x) {
			xOffset = -dim.x - 16;
		}

		SDL_Rect textOutline = v2ToRect(state.mouseState.pos + v2<int>{xOffset, 16}, v2<int>{dim.x, 18} + v2<int>{8, 8});
		drawRect(textOutline, { 0, 0, 0, 255 }, { 32, 32, 32, 192 });
		drawText(n, state.mouseState.pos + v2<int>{xOffset + 4, 20}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);

		if (state.mouseState.click) {
			industries.push_back(industry(hover));
			buildIndustryMenuOpen = false;
		}
	}

	v2<int> dim = queryText("Back to City Menu", 2.0f);
	SDL_Rect r = v2ToRect({ ((state.res.x * 7) / 8) - (dim.x / 2) - 8, state.res.y - dim.y - 16 }, { dim.x + 16, dim.y + 8 });
	drawRect(r, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 255 });
	drawText("Back to City Menu", { (state.res.x * 7) / 8, state.res.y - 12 - (dim.y / 2) }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	if (mouseInRect(r) && state.mouseState.click) {
		buildIndustryMenuOpen = false;
	}
}

void city::drawBuildStorageMenu() {
	drawRect(v2ToRect({ (state.res.x * 3) / 4, 0 }, { state.res.x / 4, state.res.y }), { 0, 0, 0, 255 }, { 192, 192, 192, 255 });

	drawText("Build Storage", { (state.res.x * 7) / 8, 32 }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	std::vector<bool> builtStorages = {};
	for (int i = 0; i < gameData.storageDatas.size(); i++) {
		builtStorages.push_back(false);
	}

	for (storage& i : storages) {
		builtStorages[i.type] = true;
	}

	std::vector<int> unbuiltStorages = {};
	for (int i = 0; i < builtStorages.size(); i++) {
		if (!builtStorages[i]) {
			unbuiltStorages.push_back(i);
		}
	}

	int hover = -1;
	for (int i = 0; i < unbuiltStorages.size(); i++) {
		SDL_Rect r = v2ToRect({ ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + 216 }, { 64, 64 });
		drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 255 });
		drawTexture(gameData.storageDatas[unbuiltStorages[i]].texture, { ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + 216 }, 4.0f, LEFT, BOTTOM);
		drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 0 }, { 128, 128, 192, 64 });

		if (mouseInRect(r)) {
			hover = unbuiltStorages[i];
		}
	}

	if (hover != -1) {
		std::string n = "Build " + gameData.storageDatas[hover].name;
		v2<int> dim = queryText(n, 1.0f);
		int xOffset = 16;
		if (state.mouseState.pos.x + dim.x + xOffset + 16 > state.res.x) {
			xOffset = -dim.x - 16;
		}

		SDL_Rect textOutline = v2ToRect(state.mouseState.pos + v2<int>{xOffset, 16}, v2<int>{dim.x, 18} + v2<int>{8, 8});
		drawRect(textOutline, { 0, 0, 0, 255 }, { 32, 32, 32, 192 });
		drawText(n, state.mouseState.pos + v2<int>{xOffset + 4, 20}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);

		if (state.mouseState.click) {
			storages.push_back(storage(hover));
			buildStorageMenuOpen = false;
		}
	}

	v2<int> dim = queryText("Back to City Menu", 2.0f);
	SDL_Rect r = v2ToRect({ ((state.res.x * 7) / 8) - (dim.x / 2) - 8, state.res.y - dim.y - 16 }, { dim.x + 16, dim.y + 8 });
	drawRect(r, { 0, 0, 0, 255 }, { 192, 192, 192, 255 }, { 128, 128, 192, 255 });
	drawText("Back to City Menu", { (state.res.x * 7) / 8, state.res.y - 12 - (dim.y / 2) }, 2.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	if (mouseInRect(r) && state.mouseState.click) {
		buildStorageMenuOpen = false;
	}
}

void city::draw(int i) {
	v2<float> np = project(pos);
	v2<int> sp = { int(np.x * (float)state.res.y), int(np.y * (float)state.res.y) };

	SDL_Point p = v2ToPoint(sp);
	SDL_Rect r = v2ToRect({ 0,0 }, state.res);

	if (!SDL_PointInRect(&p, &r)) {
		return;
	}

	float popWeight = std::clamp(log2f((float)population) * game.camera.zoom, 0.0f, 10.0f);
	SDL_Rect outline = v2ToRect(sp - v2<int>{(int)(popWeight * 8.0f), (int)(popWeight * 8.0f)}, { (int)(popWeight * 16.0f), (int)(popWeight * 16.0f) });

	if (popWeight >= 1.0f) {
		drawTexture(state.baseTextures[CITY], sp, popWeight, MIDDLE, CENTER);
		drawText(name, sp + v2<int>{0, (int)(8.0 * popWeight)}, popWeight / 2.0f, game.countries[owner].color, MIDDLE, BOTTOM);
	}

	if (state.mouseState.click && !game.selectingSomething() && !mouseInRect({ 0, 0, state.res.x, 64 })) {
		if (mouseInRect(outline)) {
			game.selectedCity = i;
		}
	}

	if (game.selectedCity == i) {
		drawRect(outline, { 0, 0, 0, 255 }, { 255, 255, 255, 0 });
	}
}

void city::tick() {

}