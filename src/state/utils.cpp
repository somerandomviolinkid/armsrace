#include "state.hpp"

SDL_Point v2ToPoint(v2<int> point) {
	return { point.x, point.y };
}

v2<int> pointToV2(SDL_Point point) {
	return { point.x, point.y };
}

bool mouseInRect(SDL_Rect r) {
	SDL_Point mousePos = v2ToPoint(state.mouseState.pos);
	return SDL_PointInRect(&mousePos, &r);
}

void drawLine(v2<int> a, v2<int> b, SDL_Color c) {
	SDL_SetRenderDrawColor(state.renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawLine(state.renderer, a.x, a.y, b.x, b.y);
}

void drawRect(SDL_Rect r, SDL_Color border, SDL_Color fill) {
	SDL_SetRenderDrawColor(state.renderer, fill.r, fill.g, fill.b, fill.a);
	SDL_RenderFillRect(state.renderer, &r);

	SDL_SetRenderDrawColor(state.renderer, border.r, border.g, border.b, border.a);
	SDL_RenderDrawRect(state.renderer, &r);
}

void drawRect(SDL_Rect r, SDL_Color border, SDL_Color fill, SDL_Color hover) {
	if (mouseInRect(r)) {
		SDL_SetRenderDrawColor(state.renderer, hover.r, hover.g, hover.b, hover.a);
		SDL_RenderFillRect(state.renderer, &r);
	} else {
		SDL_SetRenderDrawColor(state.renderer, fill.r, fill.g, fill.b, fill.a);
		SDL_RenderFillRect(state.renderer, &r);
	}

	SDL_SetRenderDrawColor(state.renderer, border.r, border.g, border.b, border.a);
	SDL_RenderDrawRect(state.renderer, &r);
}