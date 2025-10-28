#include "game.hpp"

v2<float> project(v2<float> p) {
	return ((p - game.camera.pos) * game.camera.zoom) + v2<float>{ ((float)state.res.x / (float)state.res.y) / 2.0f, 0.5f };
}

v2<float> aproject(v2<float> p) {
	return ((p - v2<float>{ ((float)state.res.x / (float)state.res.y) / 2.0f, 0.5f }) / game.camera.zoom) + game.camera.pos;
}

v2<float> project(v2<float> p, v2<float> c, v2<int> res, float z) {
	return ((p - c) * z) + v2<float>{ ((float)res.x / (float)res.y) / 2.0f, 0.5f };
}

v2<float> aproject(v2<float> p, v2<float> c, v2<int> res, float z) {
	return ((p - v2<float>{ ((float)res.x / (float)res.y) / 2.0f, 0.5f }) / z) + c;
}