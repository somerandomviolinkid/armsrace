#include "game.hpp"

v2<float> project(v2<float> p) {
	return ((p - game.camera.pos) * game.camera.zoom) + v2<float>{ ((float)state.res.x / (float)state.res.y) / 2.0f, 0.5f };
}