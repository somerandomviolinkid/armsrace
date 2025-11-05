#include "state.hpp"

SDL_Color int2Color(int i) {
	return { (uint8_t)((i & 0xFF000000) >> 24), (uint8_t)((i & 0x00FF0000) >> 16), (uint8_t)((i & 0x0000FF00) >> 8), (uint8_t)((i & 0x000000FF) >> 0)};
}

int color2Int(SDL_Color c) {
	return (c.r << 24) + (c.g << 16) + (c.b << 8) + (c.a << 0);
}

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

void drawPoint(v2<int> p, SDL_Color c) {
	SDL_SetRenderDrawColor(state.renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawPoint(state.renderer, p.x, p.y);
}

void drawLine(v2<int> a, v2<int> b, SDL_Color c) {
	SDL_SetRenderDrawColor(state.renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawLine(state.renderer, a.x, a.y, b.x, b.y);
}

void drawThickLine(v2<int> a, v2<int> b, SDL_Color c, int width) {
	SDL_SetRenderDrawColor(state.renderer, c.r, c.g, c.b, c.a);
	for (int i = -width / 2; i <= width / 2; i++) {
		SDL_RenderDrawLine(state.renderer, a.x - i, a.y, b.x - i, b.y);
	}
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

//returns NAN if no intersection, otherwise returns point of intersection
v2<float> lineSegIntersection(v2<float> a0, v2<float> a1, v2<float> b0, v2<float> b1) {
	float d = ((a0.x - a1.x) * (b0.y - b1.y)) - ((a0.y - a1.y) * (b0.x - b1.x));
	if (fabsf(d) < 0.001) {
		return { NAN, NAN };
	}

	float t = (((a0.x - b0.x) * (b0.y - b1.y)) - ((a0.y - b0.y) * (b0.x - b1.x))) / d;
	float u = (((a0.x - b0.x) * (a0.y - a1.y)) - ((a0.y - b0.y) * (a0.x - a1.x))) / d;
	if (isInRange(t, 0.0f, 1.0f) && isInRange(u, 0.0f, 1.0f)) {
		return { a0.x + (t * (a1.x - a0.x)), a0.y + (t * (a1.y - a0.y)) };
	} else {
		return { NAN, NAN };
	}
}

void drawPolygon(std::vector<v2<int>>& points, SDL_Color border, SDL_Color fill) {
	if (points.size() % 2 != 0) {
		//has to be even amount of v2's
		return;
	}

	int maxX = points[0].x;
	int minX = points[0].x;
	int maxY = points[0].y;
	int minY = points[0].y;

	//get maximum
	for (const v2<int>& p : points) {
		if (p.x > maxX) {
			maxX = p.x;
		}

		if (p.x < minX) {
			minX = p.x;
		}

		if (p.y > maxY) {
			maxY = p.y;
		}

		if (p.y < minY) {
			minY = p.y;
		}
	}

	//draw fill
	for (int y = minY - 1; y < maxY + 1; y++) {
		std::vector<int> xIntercepts = {};
		v2<int> a0 = { minX - 1, y };
		v2<int> a1 = { maxX + 1, y };
		for (int i = 0; i < points.size() - 1; i += 2) {
			if (!isInRange(y, points[i].y, points[i + 1].y)) {
				continue;
			}

			v2<float> intersection = lineSegIntersection(v2iTov2f(a0), v2iTov2f(a1), v2iTov2f(points[i]), v2iTov2f(points[i + 1]));
			if (isnan(intersection.x)) {
				continue;
			}

			xIntercepts.push_back((int)intersection.x);
		}

		if (xIntercepts.size() < 2) {
			continue;
		}

		std::sort(xIntercepts.begin(), xIntercepts.end());
		for (int i = 0; i < xIntercepts.size() - 1; i += 2) {
			drawLine({ xIntercepts[i], y}, {xIntercepts[i + 1], y}, fill);
		}
	}

	//draw outline
	for (int i = 0; i < points.size() - 1; i += 2) {
		drawLine(points[i], points[i + 1], border);
	}
}