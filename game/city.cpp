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
			//todo fix aliasing
			drawText({ name, sp + v2<int>{0, (int)(8.0f * popWeight)}, popWeight / 2.0f, {0, 0, 0, 255}, CENTER, BOTTOM });
		}

	} else {
		SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
		SDL_RenderDrawPoint(state.renderer, sp.x, sp.y);
	}

	if (((dist2d(sp, state.mouseState.pos) < 32.0f * popWeight) && (state.mouseState.click == true)) && game.menuOpen == -1) {
		menuOpen = true;
		if (!state.keyboardState[SDL_SCANCODE_LSHIFT]) {
			game.camera.pos = pos;
		}
	}

	if (menuOpen) {
		SDL_SetRenderDrawColor(state.renderer, 255, 0, 0, 255);
		SDL_Rect r = { sp.x - int(8.0f * popWeight), sp.y - int(8.0f * popWeight), int(16.0f * popWeight), int(16.0f * popWeight) };
		SDL_RenderDrawRect(state.renderer, &r);
	}

	SDL_Point p = { state.mouseState.pos.x, state.mouseState.pos.y };
	SDL_Rect r = { (state.res.x * 3) / 4, 0, state.res.x / 4, state.res.y };

	if (
		(dist2d(sp, state.mouseState.pos) > 32.0f * popWeight) &&
		(state.mouseState.click == true) &&
		(!SDL_PointInRect(&p, &r)) &&
		game.menuOpen == -1
	) {
		menuOpen = false;
	}
}

void city::drawMenu() {
	SDL_Rect r = {};
	int yOffset = 48;

	SDL_SetRenderDrawColor(state.renderer, 176, 176, 176, 255);
	r = { (state.res.x * 3) / 4, 0, state.res.x / 4, state.res.y };
	SDL_RenderFillRect(state.renderer, &r);

	drawText({ name, {(state.res.x * 7) / 8, yOffset}, 3.0f, {0, 0, 0, 255}, CENTER, MIDDLE });

	yOffset += 48;
	SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
	SDL_RenderDrawLine(state.renderer, (state.res.x * 3) / 4, 0, (state.res.x * 3) / 4, state.res.y);
	SDL_RenderDrawLine(state.renderer, (state.res.x * 3) / 4, yOffset, state.res.x, yOffset);

	yOffset += 24;
	drawText({"Information", {(state.res.x * 7) / 8, yOffset}, 2.0f, {0, 0, 0, 255}, CENTER, MIDDLE });

	textTag tag = { 1.0f, {0, 0, 0, 255}, LEFT, MIDDLE };
	yOffset += 32;
	drawText(makeTaggedText(std::format("Population: {}", (int)population), { ((state.res.x * 3) / 4) + 8, yOffset }, tag));
	yOffset += 16;
	drawText(makeTaggedText(std::format("Location: ({:.3f},{:.3f})", pos.x, pos.y), { ((state.res.x * 3) / 4) + 8, yOffset }, tag));

	yOffset += 16;
	SDL_RenderDrawLine(state.renderer, (state.res.x * 3) / 4, yOffset, state.res.x, yOffset);

	yOffset += 24;
	drawText({ "Industries", {(state.res.x * 7) / 8, yOffset}, 2.0f, {0, 0, 0, 255}, CENTER, MIDDLE });

	yOffset += 24;
	for (industry i : industries) {
		SDL_Point p = {state.mouseState.pos.x, state.mouseState.pos.y};
		r = { ((state.res.x * 3) / 4), yOffset, state.res.x / 4, 48 };

		SDL_Color background = {};
		if (SDL_PointInRect(&p, &r)) {
			background = {128, 128, 192, 255};
		} else {
			background = { 128, 128, 128, 255 };
		}

		drawRectOutline(r, background, { 0, 0, 0, 255 });

		yOffset += 8;
		r = { ((state.res.x * 3) / 4) + 8, yOffset, 32, 32 };
		drawRectOutline(r, { 255, 255, 255, 255 }, { 0, 0, 0, 255 });

		yOffset += 8;
		drawText(makeTaggedText(gameData.industryDatas[ i.type ].name, { ((state.res.x * 3) / 4) + 48, yOffset }, tag));
		yOffset += 8;

		drawTexture(gameData.industryDatas[ i.type ].texture, { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, MIDDLE);
		yOffset += 8;

		drawText(makeTaggedText(std::format("Workers: {} / {}", i.workers, gameData.industryDatas[ i.type ].maxWorkers), { ((state.res.x * 3) / 4) + 48, yOffset }, tag));
		yOffset += 16;
	}
}