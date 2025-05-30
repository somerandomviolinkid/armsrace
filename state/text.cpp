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

void drawText(text t) {
	v2<int> dim = {(int)t.content.length() * 8, 18};
	dim = dim * t.textScale;
	v2<int> offset = calculateAlign(t.pos, dim, t.hAlign, t.vAlign);

	for (char& c : t.content) {
		SDL_SetTextureColorMod(state.glyphAtlas[ c ], t.color.r, t.color.g, t.color.b);
		SDL_SetTextureAlphaMod(state.glyphAtlas[ c ], t.color.a);

		drawTexture(state.glyphAtlas[ c ], offset, t.textScale, LEFT, BOTTOM);
		offset.x += (int)(8.0f * t.textScale);
	}
}