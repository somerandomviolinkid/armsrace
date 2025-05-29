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

		SDL_SetRenderDrawColor(state.renderer, 255, 0, 0, 255);
		SDL_Rect r = { sp.x - int(16.0f * popWeight), sp.y - int(16.0f * popWeight), int(32.0f * popWeight), int(32.0f * popWeight) };
		SDL_RenderDrawRect(state.renderer, &r);
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
	SDL_Rect r = {};

	SDL_SetRenderDrawColor(state.renderer, 208, 208, 208, 255);
	r = { (state.res.x * 3) / 4, 0, state.res.x / 4, state.res.y };
	SDL_RenderFillRect(state.renderer, &r);

	drawText({ name, {(state.res.x * 7) / 8, 48}, 3.0f, {0, 0, 0, 255}, CENTER, MIDDLE });

	SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
	SDL_RenderDrawLine(state.renderer, (state.res.x * 3) / 4, 0, (state.res.x * 3) / 4, state.res.y);
	SDL_RenderDrawLine(state.renderer, (state.res.x * 3) / 4, 96, state.res.x, 96);

	textTag tag = { 1.0f, {0, 0, 0, 255}, LEFT, MIDDLE };
	drawText(makeTaggedText(std::format("Population: {}", (int)population), { ((state.res.x * 3) / 4) + 8, 112 }, tag));
	drawText(makeTaggedText(std::format("Location: ({:.3f},{:.3f})", pos.x, pos.y), { ((state.res.x * 3) / 4) + 8, 136 }, tag));

	SDL_RenderDrawLine(state.renderer, (state.res.x * 3) / 4, 152, state.res.x, 152);

	int yOffset = 176;
	for (industry i : industries) {
		r = { ((state.res.x * 3) / 4) + 8, yOffset - 16, 32, 32 };

		SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(state.renderer, &r);

		SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(state.renderer, &r);

		drawTexture(gameData.industryDatas[ i.type ].texture, { ((state.res.x * 3) / 4) + 8, yOffset }, 2.0f, LEFT, MIDDLE);
		tag.textScale = 2.0f;
		drawText(makeTaggedText(gameData.industryDatas[ i.type ].name, { ((state.res.x * 3) / 4) + 48, yOffset }, tag));
		yOffset += 32;

		tag.textScale = 1.0f;
		drawText(makeTaggedText("Workers:", { ((state.res.x * 3) / 4) + 8, yOffset }, tag));
		drawText(makeTaggedText(std::format("{} / {}", i.workers, gameData.industryDatas[ i.type ].maxWorkers), { ((state.res.x * 7) / 8) + 8, yOffset }, tag));
		yOffset += 24;

		for (std::pair<const uint32_t, float>& pair : i.inventory) {
			r = { ((state.res.x * 3) / 4) + 8, yOffset - 8, 16, 16 };
			SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(state.renderer, &r);

			SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(state.renderer, &r);

			drawTexture(gameData.resourceDatas[ pair.first ].texture, { ((state.res.x * 3) / 4) + 8, yOffset }, 1.0f, LEFT, MIDDLE);

			drawText(makeTaggedText(std::format("{}:", gameData.resourceDatas[ pair.first ].name), { ((state.res.x * 3) / 4) + 32, yOffset }, tag));
			drawText(makeTaggedText(std::format("{:.2f} / {:.2f}", pair.second, gameData.industryDatas[ i.type ].storage[ pair.first ]), { ((state.res.x * 7) / 8) + 8, yOffset }, tag));
			yOffset += 24;
		}

		yOffset -= 8;
		SDL_RenderDrawLine(state.renderer, (state.res.x * 3) / 4, yOffset, state.res.x, yOffset);
		yOffset+=24;
	}
}