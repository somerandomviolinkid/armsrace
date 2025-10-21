#include "game.hpp"

void city::drawMenu() {
	drawRect(v2ToRect({ (state.res.x * 3) / 4, 0 }, { state.res.x / 4, state.res.y }), { 0, 0, 0, 255 }, { 192, 192, 192, 255 });

	drawText(name, { (state.res.x * 7) / 8, 32 }, 3.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);

	drawText("Owner: ", {((state.res.x * 3) / 4) + 16, 80}, 2.0f, {0, 0, 0, 255}, LEFT, CENTER);
	drawText(game.countries[owner].name, { state.res.x - 16, 80 }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);

	drawText("Population: ", { ((state.res.x * 3) / 4) + 16, 120 }, 2.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
	drawText(formatNumber((float)population), { state.res.x - 16, 120 }, 2.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);

	drawLine({ (state.res.x * 3) / 4, 144 }, { state.res.x, 144 }, {0, 0, 0, 255});
	drawText("Industries", { (state.res.x * 7) / 8, 176 }, 3.0f, {0, 0, 0, 255}, MIDDLE, CENTER);

	int hover = -1;
	int i = 0;
	for (i = 0; i < industries.size(); i++) {
		SDL_Rect r = v2ToRect({ ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + 216 }, { 64, 64 });
		drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 255 });
		drawTexture(gameData.industryDatas[industries[i].type].texture, { ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + 216 }, 4.0f, LEFT, BOTTOM);
		drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 0}, {128, 128, 192, 64});

		if (mouseInRect(r)) {
			hover = i;
		}
	}

	//skip if owner is not player
	if (owner != 0) {
		return;
	}

	SDL_Rect r = v2ToRect({ ((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + 216 }, { 64, 64 });
	drawRect(r, { 0, 0, 0, 255 }, { 64, 64, 64, 255 });
	drawTexture(state.baseTextures[1], {((state.res.x * 3) / 4) + ((i % 5) * 80) + 16, ((i / 5) * 80) + 216}, 4.0f, LEFT, BOTTOM);
	drawRect(r, { 0, 0, 0, 255 }, { 255, 255, 255, 0 }, { 128, 128, 192, 64 });

	if (mouseInRect(r)) {
		std::string buildText = "Build Industry";
		v2<int> dim = queryText(buildText, 1.0f);
		int xOffset = 16;
		if (state.mouseState.pos.x + dim.x + 16 > state.res.x) {
			xOffset = -dim.x - 16;
		}

		SDL_Rect textOutline = v2ToRect(state.mouseState.pos + v2<int>{xOffset, 16}, v2<int>{dim.x, 18} + v2<int>{8, 8});
		drawRect(textOutline, { 0, 0, 0, 255 }, { 32, 32, 32, 192 });
		drawText(buildText, state.mouseState.pos + v2<int>{xOffset + 4, 20}, 1.0f, {255, 255, 255, 255}, LEFT, BOTTOM);

		if (state.mouseState.click) {
			buildIndustryMenuOpen = true;
			return;
		}
	}

	if (hover != -1) {
		std::string n = gameData.industryDatas[industries[hover].type].name;
		std::string w = std::format("{} / {} workers", industries[hover].workers, gameData.industryDatas[industries[hover].type].maxWorkers);
		std::string e = std::format("{:.1f}% efficiency", (float)industries[hover].workers / (float)gameData.industryDatas[industries[hover].type].maxWorkers);

		std::vector<int> widths = { queryText(n, 1.0f).x, queryText(e, 1.0f).x, queryText(w, 1.0f).x };
		int max = *std::max_element(widths.begin(), widths.end());

		int xOffset = 16;
		if (state.mouseState.pos.x + max + 16 > state.res.x) {
			xOffset = -max - 16;
		}

		SDL_Rect textOutline = v2ToRect(state.mouseState.pos + v2<int>{xOffset, 16}, v2<int>{max, 18} + v2<int>{8, 60});
		drawRect(textOutline, { 0, 0, 0, 255 }, { 32, 32, 32, 192 });
		drawText(n, state.mouseState.pos + v2<int>{xOffset + 4, 20}, 1.0f, { 255, 255, 255, 255 }, LEFT, BOTTOM);
		drawText(w, state.mouseState.pos + v2<int>{xOffset + 4, 44}, 1.0f, {255, 255, 255, 255}, LEFT, BOTTOM);
		drawText(e, state.mouseState.pos + v2<int>{xOffset + 4, 68}, 1.0f, {255, 255, 255, 255}, LEFT, BOTTOM);

		if (state.mouseState.click) {
			game.selectedIndustry = hover;
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
		if (state.mouseState.pos.x + dim.x + 16 > state.res.x) {
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

void city::draw(int i) {
	v2<float> np = project(pos);
	v2<int> sp = { int(np.x * (float)state.res.y), int(np.y * (float)state.res.y) };

	float popWeight = std::clamp(log2f((float)population) * game.camera.zoom, 0.0f, 10.0f);
	SDL_Rect outline = v2ToRect(sp - v2<int>{(int)(popWeight * 8.0f), (int)(popWeight * 8.0f)}, { (int)(popWeight * 16.0f), (int)(popWeight * 16.0f) });

	if (popWeight >= 1.0f) {
		drawTexture(state.baseTextures[0], sp, popWeight, MIDDLE, CENTER);
		drawText(name, sp + v2<int>{0, (int)(8.0 * popWeight)}, popWeight / 2.0f, { 0, 0, 0, 255 }, MIDDLE, BOTTOM);
	}

	if (state.mouseState.click && !mouseInRect(v2ToRect({ (state.res.x * 3) / 4, 0 }, { state.res.x / 4, state.res.y }))) {
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