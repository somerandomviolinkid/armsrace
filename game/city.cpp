#include "game.hpp"

void city::tick() {
	population *= 1.000002f;

	for (industry& i : industries) {
		i.tick();
	}
}

void city::draw() {
	v2<float> np = project(pos);
	v2<int> sp = { int(np.x * (float)state.res.y), int(np.y * (float)state.res.y) };

	float popWeight = std::clamp(log10f(population) * game.camera.zoom, 0.0f, 5.0f);
	if (popWeight >= 1.0f) {
		drawTexture(state.baseTextures[ 1 ], sp, popWeight, CENTER, MIDDLE);

		if (popWeight >= 2.0f) {
			popWeight /= 2.0f;
			drawText({ name, sp + v2<int>{0, (int)(16.0f * popWeight)}, popWeight, {0, 0, 0, 255}, CENTER, BOTTOM });
		}

	} else {
		SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
		SDL_RenderDrawPoint(state.renderer, sp.x, sp.y);
	}

	if (menuOpen || ((dist2d(sp, state.mouseState.pos) < 32.0f * popWeight) && (state.mouseState.click == true))) {
		menuOpen = true;
		drawMenu();
	}

	SDL_Point p = { state.mouseState.pos.x, state.mouseState.pos.y };
	SDL_Rect r = { (state.res.x * 3) / 4, 0, state.res.x / 4, state.res.y };

	if (
		(dist2d(sp, state.mouseState.pos) > 32.0f * popWeight) &&
		(state.mouseState.click == true) &&
		(!SDL_PointInRect(&p, &r))
		) {
		menuOpen = false;
	}
}

void city::drawMenu() {
	SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
	SDL_Rect r = { (state.res.x * 3) / 4, 0, state.res.x / 4, state.res.y };
	SDL_RenderFillRect(state.renderer, &r);

	drawText({ name, {(state.res.x * 7) / 8, 48}, 3.0f, {255, 255, 255, 255}, CENTER, MIDDLE });

	SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(state.renderer, (state.res.x * 3) / 4, 0, (state.res.x * 3) / 4, state.res.y);
	SDL_RenderDrawLine(state.renderer, (state.res.x * 3) / 4, 96, state.res.x, 96);

	drawText({ "Population: " + std::to_string((int)population), {((state.res.x * 3) / 4) + 8, 112}, 1.0f, {255, 255, 255, 255}, LEFT, MIDDLE });
	drawText({ "Location: (" + std::to_string(pos.x) + "," + std::to_string(pos.y) + ")", {((state.res.x * 3) / 4) + 8, 136}, 1.0f, {255, 255, 255, 255}, LEFT, MIDDLE });

	SDL_RenderDrawLine(state.renderer, (state.res.x * 3) / 4, 160, state.res.x, 160);

	int yOffset = 0;
	for (industry i : industries) {
		drawTexture(gameData.industryDatas[ i.type ].texture, { ((state.res.x * 3) / 4) + 8, 184 }, 1.0f, LEFT, MIDDLE);
		int w, h;
		SDL_QueryTexture(gameData.industryDatas[ i.type ].texture, NULL, NULL, &w, &h);
		printf("%d %d\n", w, h);
		drawText({ gameData.industryDatas[ i.type ].name, { ((state.res.x * 3) / 4) + 32, 184}, 1.0f, {255, 255, 255, 255}, LEFT, MIDDLE });
		yOffset += 24;
	}
}