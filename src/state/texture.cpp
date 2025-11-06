#include "state.hpp"

SDL_Rect v2ToRect(v2<int> pos, v2<int> dim) {
	return { pos.x, pos.y, dim.x, dim.y };
}

//makes rect from top left and bottom right corners
SDL_Rect v2PosToRect(v2<int> pos0, v2<int> pos1) {
	return { pos0.x, pos0.y, pos1.x - pos0.x, pos1.y - pos0.y };
}

v2<int> calculateAlign(v2<int> pos, v2<int> dim, H_ALIGN hAlign, V_ALIGN vAlign) {
	v2<int> res = { 0,0 };

	switch (hAlign) {
	case LEFT:
		res.x = pos.x;
		break;
	case MIDDLE:
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
	case CENTER:
		res.y = pos.y - (dim.y / 2);
		break;
	case TOP:
		res.y = pos.y - dim.y;
		break;
	}

	return res;
}

void loadTexture(tex_t& t, std::string path) {
	std::ifstream f(path);
	if (f.good()) {
		f.close();

		t.texture = IMG_LoadTexture(state.renderer, path.c_str());
		SDL_QueryTexture(t.texture, NULL, NULL, &t.dim.x, &t.dim.y);
	} else {
		t.texture = IMG_LoadTexture(state.renderer, "assets/textures/debug.png");
		t.dim = { 16, 16 };
	}
}

void drawTexture(tex_t texture, v2<int> pos, float scale, H_ALIGN hAlign, V_ALIGN vAlign) {
	v2<int> dim = texture.dim * scale;
	v2<int> alignedPos = calculateAlign(pos, dim, hAlign, vAlign);
	SDL_Rect r = v2ToRect(alignedPos, dim);
	SDL_RenderCopy(state.renderer, texture.texture, NULL, &r);
}