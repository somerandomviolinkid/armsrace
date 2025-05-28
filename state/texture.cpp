#include "state.hpp"

v2<int> calculateAlign(v2<int> pos, v2<int> dim, H_ALIGN hAlign, V_ALIGN vAlign) {
	v2<int> res = { 0, 0 };
	switch (hAlign) {
		case LEFT:
			res.x = pos.x;
			break;
		case CENTER:
			res.x = pos.x - (dim.x / 2);
			break;
		case RIGHT:
			res.x = pos.x - dim.x;
			break;
	}

	switch (vAlign) {
		case BOTTOM:
			res.y = pos.y;
			break;
		case MIDDLE:
			res.y = pos.y - (dim.y / 2);
			break;
		case TOP:
			res.y = pos.y - dim.y;
			break;
	}

	return res;
}

void drawTexture(SDL_Texture* texture, v2<int> pos, float scale, H_ALIGN hAlign, V_ALIGN vAlign) {
	v2<int> dim = {0, 0};
	SDL_QueryTexture(texture, NULL, NULL, &dim.x, &dim.y);
	dim = dim * scale; //todo learn assignment operators
	v2<int> alignedPos = calculateAlign(pos, dim, hAlign, vAlign);
	SDL_Rect r = {alignedPos.x, alignedPos.y, dim.x, dim.y};
	SDL_RenderCopy(state.renderer, texture, NULL, &r);
}