#include "state.hpp"

//returns dimensions of given string and scale
v2<int> queryText(std::string text, float scale) {
	v2<int> dim = { 0, (int)((float)(state.fontAtlas['a'].dim.y + 2.0f) * scale) };
	for (char& c : text) {
		dim.x += state.fontAtlas[c].dim.x;
	}

	dim.x = (int)((float)dim.x * scale);
	return dim;
}

void drawText(std::string text, v2<int> pos, float scale, SDL_Color color, H_ALIGN hAlign, V_ALIGN vAlign) {
	v2<int> dim = queryText(text, scale);
	v2<int> offset = calculateAlign(pos, dim, hAlign, vAlign);

	for (char& c : text) {
		SDL_SetTextureColorMod(state.fontAtlas[c].texture, color.r, color.g, color.b);
		SDL_SetTextureAlphaMod(state.fontAtlas[c].texture, color.a);

		drawTexture(state.fontAtlas[c], offset, scale, LEFT, BOTTOM);
		offset.x += (int)(state.fontAtlas[c].dim.x * scale);
	}
}