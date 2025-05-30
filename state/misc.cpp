#include "state.hpp"

void drawRectOutline(SDL_Rect r, SDL_Color fill, SDL_Color outline) {
	SDL_SetRenderDrawColor(state.renderer, fill.r, fill.g, fill.b, fill.a);
	SDL_RenderFillRect(state.renderer, &r);

	SDL_SetRenderDrawColor(state.renderer, outline.r, outline.g, outline.b, outline.a);
	SDL_RenderDrawRect(state.renderer, &r);
}