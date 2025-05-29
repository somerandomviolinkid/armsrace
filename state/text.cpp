#include "state.hpp"

//helper for repeated text styles
text makeTaggedText(std::string content, v2<int> pos, textTag tag) {
	return {
		content,
		pos,
		tag.textScale,
		tag.color,
		tag.hAlign,
		tag.vAlign
	};
}

//todo make better
void drawText(text t) {
	SDL_Surface* surface = TTF_RenderUTF8_Solid_Wrapped(state.font, t.content.c_str(), t.color, 0);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(state.renderer, surface);

	drawTexture(texture, t.pos, t.textScale, t.hAlign, t.vAlign);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}