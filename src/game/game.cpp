#include "game.hpp"
void Game::updateCamera() {
	if (state.mouseState.down) {
		v2<float> mouseMove = { (float)state.mouseState.motion.x / (float)state.res.y, (float)state.mouseState.motion.y / (float)state.res.y };
		game.camera.pos = game.camera.pos - (mouseMove / camera.zoom);
	}

	if (state.keyboardState[SDL_SCANCODE_W]) {
		game.camera.pos.y -= camera.zoom;
	}

	if (state.keyboardState[SDL_SCANCODE_A]) {
		game.camera.pos.x -= camera.zoom;
	}

	if (state.keyboardState[SDL_SCANCODE_S]) {
		game.camera.pos.y += camera.zoom;
	}

	if (state.keyboardState[SDL_SCANCODE_D]) {
		game.camera.pos.x += camera.zoom;
	}

	camera.zoom = std::clamp(camera.zoom * powf(1.05f, (float)state.mouseState.scroll), 0.01f, 2.0f);
}

void Game::tick() {
	std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
	for (city& c : cities) {
		c.tick();
		for (industry& i : c.industries) {
			i.tick();
		}
	}

	for (mine& i : mines) {
		i.tick();
	}

	ticks++;
	tickStart = std::chrono::high_resolution_clock::now();
	tickTime = std::chrono::duration_cast<std::chrono::microseconds>(game.tickStart - t0).count();
}

void Game::draw() {
	switch (mode) {
	case NORMAL:
	{
		if (state.keyboardState[SDL_SCANCODE_ESCAPE]) {
			mode = PAUSE_MENU;
			game.running = false;
			break;
		}

		if (state.keyboardState[SDL_SCANCODE_C]) {
			game.camera.pos = game.cities[0].pos;
			game.camera.zoom = 0.2f;
		}

		if (state.keyboardState[SDL_SCANCODE_SPACE] && speedChangeTimer >= 6) {
			running = !running;
			speedChangeTimer = 0;
		}

		if (state.keyboardState[SDL_SCANCODE_MINUS] && speedChangeTimer >= 6 && selectedSpeed > 0) {
			selectedSpeed--;
			speedChangeTimer = 0;
		}

		if (state.keyboardState[SDL_SCANCODE_EQUALS] && speedChangeTimer >= 6 && selectedSpeed < 5) {
			selectedSpeed++;
			speedChangeTimer = 0;
		}

		speedChangeTimer++;

		if (frames % 100 == 0) {
			frameTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - frameStart).count();
			frameStart = std::chrono::high_resolution_clock::now();
			frames = 0;
		}

		SDL_SetRenderDrawColor(state.renderer, 0, 192, 0, 255);
		SDL_RenderClear(state.renderer);

		occludeRects.push_back({ 0, 0, state.res.x, 64 });
		occludeRects.push_back({ 16, state.res.y - (state.res.y / 4) - 16, state.res.x / 4, state.res.y / 4 });

		if (selectedCity != -1 || selectedResource != -1 || selectedMine != -1) {
			occludeRects.push_back(v2ToRect({ (state.res.x * 3) / 4, 0 }, { state.res.x / 4, state.res.y }));
		}

		bool mouseOccluded = false;
		for (SDL_Rect& r : occludeRects) {
			if (mouseInRect(r)) {
				mouseOccluded = true;
				break;
			}
		}

		if (!mouseOccluded) {
			updateCamera();
			if (state.mouseState.click) {
				if (selectedCity != -1) {
					cities[selectedCity].buildIndustryMenuOpen = false;
					cities[selectedCity].buildStorageMenuOpen = false;
				}

				selectedCity = -1;

				selectedCountry = -1;

				selectedIndustry = -1;
				industryImportResourceSelected = -1;
				industryExportResourceSelected = -1;

				selectedStorage = -1;

				selectedResource = -1;

				selectedMine = -1;
				mineExportResourceSelected = -1;
				mineAllocateWorkersMenuScroll = 0;
			}
		}

		
		//draw borders - todo speed up
		
		memset(mapPixelArray, 0, (size_t)(4 * state.res.x * state.res.y));
		std::vector<borderData> posArray = {};
		for (city& c : cities) {
			v2<float> cnp = project(c.pos);
			v2<int> csp = { int(cnp.x * (float)state.res.y), int(cnp.y * (float)state.res.y) };

			SDL_Rect screenRect = { 0, 0, state.res.x, state.res.y };
			SDL_Point p = v2ToPoint(csp);

			if (SDL_PointInRect(&p, &screenRect)) {
				posArray.push_back({ csp, colorMul(countries[c.owner].color, 0.75f)});
			}
		}

		memset(mapPixelBufferArray, 0, (size_t)(4 * state.res.x * state.res.y));
		for (int i = 0; i < std::min(int(5.0f * camera.zoom * float(state.res.y)), state.res.x) + 1; i++) {
			int initialSize = (int)posArray.size();
			for (int b = 0; b < initialSize; b++) {
				mapPixelArray[(posArray[b].pos.y * state.res.x) + posArray[b].pos.x] = color2Int(posArray[b].color) - 192;

				//propagate east
				if (posArray[b].pos.x - 1 >= 0 
					&& (mapPixelArray[(posArray[b].pos.y * state.res.x) + posArray[b].pos.x - 1] == 0)
					&& mapPixelBufferArray[posArray[b].pos.y * state.res.x + posArray[b].pos.x - 1] == 0
					) {
					posArray.push_back({ posArray[b].pos - v2<int>{1, 0}, posArray[b].color});
					mapPixelBufferArray[posArray[b].pos.y * state.res.x + posArray[b].pos.x - 1] = (uint32_t)color2Int(posArray[b].color);
				}

				//propagate west
				if (posArray[b].pos.x + 1 < state.res.x 
					&& (mapPixelArray[(posArray[b].pos.y * state.res.x) + posArray[b].pos.x + 1] == 0)
					&& mapPixelBufferArray[posArray[b].pos.y * state.res.x + posArray[b].pos.x + 1] == 0
					) {
					posArray.push_back({ posArray[b].pos + v2<int>{1, 0}, posArray[b].color});
					mapPixelBufferArray[posArray[b].pos.y * state.res.x + posArray[b].pos.x + 1] = (uint32_t)color2Int(posArray[b].color);
				}

				//propagate north
				if (posArray[b].pos.y - 1 >= 0
					&& (mapPixelArray[((posArray[b].pos.y - 1) * state.res.x) + posArray[b].pos.x] == 0)
					&& mapPixelBufferArray[((posArray[b].pos.y - 1) * state.res.x) + posArray[b].pos.x] == 0
					) {
					posArray.push_back({ posArray[b].pos - v2<int>{0, 1}, posArray[b].color});
					mapPixelBufferArray[((posArray[b].pos.y - 1) * state.res.x) + posArray[b].pos.x] = (uint32_t)color2Int(posArray[b].color);
				}

				//propagate south
				if (posArray[b].pos.y + 1 < state.res.y 
					&& (mapPixelArray[((posArray[b].pos.y + 1) * state.res.x) + posArray[b].pos.x] == 0)
					&& mapPixelBufferArray[((posArray[b].pos.y + 1) * state.res.x) + posArray[b].pos.x] == 0
					) {
					posArray.push_back({ posArray[b].pos + v2<int>{0, 1}, posArray[b].color});
					mapPixelBufferArray[((posArray[b].pos.y + 1) * state.res.x) + posArray[b].pos.x] = (uint32_t)color2Int(posArray[b].color);
				}
			}

			posArray.erase(posArray.begin(), posArray.begin() + initialSize);
		}

		void* px;
		int pitch;
		SDL_LockTexture(mapTexture, NULL, &px, &pitch);
		for (int y = 0; y < state.res.y; y++) {
			memcpy(&((uint8_t*)px)[y * pitch], &mapPixelArray[y * state.res.x], (size_t)(state.res.x * 4));
		}

		SDL_UnlockTexture(mapTexture);
		SDL_RenderCopy(state.renderer, mapTexture, NULL, NULL);
		
		
		
		for (int i = 0; i < cities.size(); i++) {
			cities[i].draw(i);

			v2<float> cnp = project(cities[i].pos);
			v2<int> csp = { int(cnp.x * (float)state.res.y), int(cnp.y * (float)state.res.y) };

			SDL_Point cp = v2ToPoint(csp);
			SDL_Rect screenRect = v2ToRect({ 0, 0 }, state.res);

			for (industry& ind : cities[i].industries) {
				for (const std::pair<int, float> p : gameData.industryDatas[ind.type].outputs) {
					for (exportData& e : ind.exportDatas[p.first]) {
						v2<float> inp = project(cities[e.targetCity].pos);
						v2<int> isp = { int(inp.x * (float)state.res.y), int(inp.y * (float)state.res.y) };

						SDL_Point ep = v2ToPoint(isp);

						if (!SDL_PointInRect(&cp, &screenRect) && !SDL_PointInRect(&ep, &screenRect)) {
							continue;
						}

						if (!SDL_PointInRect(&cp, &screenRect) || !SDL_PointInRect(&ep, &screenRect)) {
							v2<float> intersections[4] = {
								lineSegIntersection(v2iTov2f(csp), v2iTov2f(isp), v2iTov2f({0, 0 }), v2iTov2f({state.res.x, 0})),
								lineSegIntersection(v2iTov2f(csp), v2iTov2f(isp), v2iTov2f({0, 0 }), v2iTov2f({0, state.res.y})),
								lineSegIntersection(v2iTov2f(csp), v2iTov2f(isp), v2iTov2f({state.res.x, 0 }), v2iTov2f({state.res.x, state.res.y})),
								lineSegIntersection(v2iTov2f(csp), v2iTov2f(isp), v2iTov2f({0, state.res.y }), v2iTov2f({state.res.x, state.res.y}))
							};

							v2<int> p0 = SDL_PointInRect(&cp, &screenRect) ? csp : isp;
							v2<int> p1 = { 0, 0 };
							for (int j = 0; j < 4; j++) {
								if (!isnan(intersections[j].x)) {
									p1 = v2fTov2i(intersections[j]);
									break;
								}
							}

							drawLine(p0, p1, { 0, 0, 0, 255 });
							continue;
						}

						drawLine(csp, isp, { 0, 0, 0, 255 });
					}
				}
			}

			for (storage& s : cities[i].storages) {
				for (const std::pair<int, float> p : s.inventory) {
					for (exportData& e : s.exportDatas[p.first]) {
						v2<float> snp = project(cities[e.targetCity].pos);
						v2<int> ssp = { int(snp.x * (float)state.res.y), int(snp.y * (float)state.res.y) };

						SDL_Point ep = v2ToPoint(ssp);

						if (!SDL_PointInRect(&cp, &screenRect) && !SDL_PointInRect(&ep, &screenRect)) {
							continue;
						}

						if (!SDL_PointInRect(&cp, &screenRect) || !SDL_PointInRect(&ep, &screenRect)) {
							v2<float> intersections[4] = {
								lineSegIntersection(v2iTov2f(csp), v2iTov2f(ssp), v2iTov2f({0, 0 }), v2iTov2f({state.res.x, 0})),
								lineSegIntersection(v2iTov2f(csp), v2iTov2f(ssp), v2iTov2f({0, 0 }), v2iTov2f({0, state.res.y})),
								lineSegIntersection(v2iTov2f(csp), v2iTov2f(ssp), v2iTov2f({state.res.x, 0 }), v2iTov2f({state.res.x, state.res.y})),
								lineSegIntersection(v2iTov2f(csp), v2iTov2f(ssp), v2iTov2f({0, state.res.y }), v2iTov2f({state.res.x, state.res.y}))
							};

							v2<int> p0 = SDL_PointInRect(&cp, &screenRect) ? csp : ssp;
							v2<int> p1 = { 0, 0 };
							for (int j = 0; j < 4; j++) {
								if (!isnan(intersections[j].x)) {
									p1 = v2fTov2i(intersections[j]);
									break;
								}
							}

							drawLine(p0, p1, { 0, 0, 0, 255 });
							continue;
						}

						drawLine(csp, ssp, { 0, 0, 0, 255 });
					}
				}
			}
		}

		for (int i = 0; i < mines.size(); i++) {
			mines[i].draw(i);
			v2<float> mnp = project(mines[i].pos);
			v2<int> msp = { int(mnp.x * (float)state.res.y), int(mnp.y * (float)state.res.y) };

			SDL_Point mp = v2ToPoint(msp);
			SDL_Rect mr = v2ToRect({ 0, 0 }, state.res);

			SDL_Rect screenRect = v2ToRect({ 0, 0 }, state.res);

			for (const std::pair<int, float> p : mines[i].inventory) {
				for (exportData& e : mines[i].exportDatas[p.first]) {
					v2<float> cnp = project(cities[e.targetCity].pos);
					v2<int> csp = { int(cnp.x * (float)state.res.y), int(cnp.y * (float)state.res.y) };

					SDL_Point ep = v2ToPoint(csp);
					SDL_Rect er = v2ToRect({ 0, 0 }, state.res);

					if (!SDL_PointInRect(&mp, &screenRect) && !SDL_PointInRect(&ep, &screenRect)) {
						continue;
					}

					if (!SDL_PointInRect(&mp, &screenRect) || !SDL_PointInRect(&ep, &screenRect)) {
						v2<float> intersections[4] = {
							lineSegIntersection(v2iTov2f(csp), v2iTov2f(msp), v2iTov2f({0, 0 }), v2iTov2f({state.res.x, 0})),
							lineSegIntersection(v2iTov2f(csp), v2iTov2f(msp), v2iTov2f({0, 0 }), v2iTov2f({0, state.res.y})),
							lineSegIntersection(v2iTov2f(csp), v2iTov2f(msp), v2iTov2f({state.res.x, 0 }), v2iTov2f({state.res.x, state.res.y})),
							lineSegIntersection(v2iTov2f(csp), v2iTov2f(msp), v2iTov2f({0, state.res.y }), v2iTov2f({state.res.x, state.res.y}))
						};

						v2<int> p0 = SDL_PointInRect(&ep, &screenRect) ? csp : msp;
						v2<int> p1 = { 0, 0 };
						for (int j = 0; j < 4; j++) {
							if (!isnan(intersections[j].x)) {
								p1 = v2fTov2i(intersections[j]);
								break;
							}
						}

						drawLine(p0, p1, { 0, 0, 0, 255 });
						continue;
					}

					drawLine(csp, msp, { 0, 0, 0, 255 });
				}
			}
		}

		for (int i = 0; i < naturalResources.size(); i++) {
			naturalResources[i].draw(i);
		}

		if (camera.zoom < 0.025) {
			for (int i = 0; i < countries.size(); i++) {
				v2<float> western = countries[i].getWesternMost(i);
				v2<float> wnp = project(western);
				v2<int> wsp = { int(wnp.x * (float)state.res.y), int(wnp.y * (float)state.res.y) };

				v2<float> eastern = countries[i].getEasternMost(i);
				v2<float> enp = project(eastern);
				v2<int> esp = { int(enp.x * (float)state.res.y), int(enp.y * (float)state.res.y) };

				v2<float> center = countries[i].getCenter(i);
				v2<float> cnp = project(center);
				v2<int> csp = { int(cnp.x * (float)state.res.y), int(cnp.y * (float)state.res.y) };

				float scale = fabsf((float)(esp.x - wsp.x) / (16.0f * game.countries[i].name.length()));
				drawText(countries[i].name, csp, scale, countries[i].color, MIDDLE, CENTER);
			}
		}

		drawTopMenu();
		drawMinimap();

		//what the fuck, fix
		if (selectedCity != -1) {
			if (cities[selectedCity].buildIndustryMenuOpen) {
				cities[selectedCity].drawBuildIndustryMenu();
			} else if (cities[selectedCity].buildStorageMenuOpen) {
				cities[selectedCity].drawBuildStorageMenu();
			} else if (selectedIndustry != -1) {
				if (industryImportResourceSelected != -1) {
					cities[selectedCity].industries[selectedIndustry].drawImportMenu(industryImportResourceSelected);
				} else if (industryExportResourceSelected != -1) {
					cities[selectedCity].industries[selectedIndustry].drawExportMenu(industryExportResourceSelected);
				} else {
					cities[selectedCity].industries[selectedIndustry].drawMenu();
				}
			} else if (selectedStorage != -1) {
				if (storageExportResourceSelected != -1) {
					cities[selectedCity].storages[selectedStorage].drawExportMenu(storageExportResourceSelected);
				} else {
					cities[selectedCity].storages[selectedStorage].drawMenu();
				}
			} else {
				cities[selectedCity].drawMenu();
			}
		} else if (selectedResource != -1) {
			naturalResources[selectedResource].drawMenu();
		} else if (selectedMine != -1) {
			if (mineExportResourceSelected != -1) {
				mines[selectedMine].drawExportMenu(mineExportResourceSelected);
			} else {
				mines[selectedMine].drawMenu();
			}
		} else if (selectedCountry != -1) {
			countries[selectedCountry].drawMenu(selectedCountry);
		}

		drawText(std::format("Camera Position: ({:.3f},{:.3f})", camera.pos.x, camera.pos.y), { state.res.x / 2, state.res.y - 44 }, 1.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);
		drawText(std::format("Camera Zoom: {:.3f}x", camera.zoom), { state.res.x / 2, state.res.y - 28 }, 1.0f, { 0, 0, 0, 255 }, MIDDLE, CENTER);
		drawText(std::format("Tick time: {} us", tickTime), { state.res.x / 2 - 16, state.res.y - 12 }, 1.0f, { 0, 0, 0, 255 }, RIGHT, CENTER);
		drawText(std::format("Frame time: {} us", frameTime / 100), { state.res.x / 2 + 16, state.res.y - 12 }, 1.0f, { 0, 0, 0, 255 }, LEFT, CENTER);
		occludeRects.clear();

		break;
	}
	case PAUSE_MENU:
		drawPauseMenu();
		pauseMenuTick();
		break;
	case SAVE_GAME:
		break;
	}

	game.frames++;
}

void Game::drawTopMenu() {
	SDL_Rect r = { 0, 0, state.res.x, 64 };

	std::string suffix = ticks % 24 >= 12 ? "PM" : "AM";
	int normalizedTime = ticks % 24 >= 12 ? ticks % 24 - 12 : ticks % 24;
	if (normalizedTime == 0) {
		normalizedTime = 12;
	}

	std::string d = std::format("Day {}, {:02}:00 ", ticks / 24, normalizedTime) + suffix;
	int dw = queryText(d, 3.0f).x;
	
	int xOffset = 8;
	drawRect(r, { 0, 0, 0, 255 }, { 192, 192, 192, 255 });
	drawText(d, { xOffset, 32 }, 3.0f, { 0, 0, 0, 255 }, LEFT, CENTER);

	xOffset += dw + 32;
	drawTexture(state.baseTextures[POPULATION], { xOffset, 32 }, 3.0f, LEFT, CENTER);

	xOffset += 56;
	std::string p = formatNumber((float)countryPopulation(0));
	int pw = queryText(p, 3.0f).x;
	drawText(p, { xOffset, 32 }, 3.0f, { 0, 0, 0, 255 }, LEFT, CENTER);

	xOffset += pw + 32;
	drawTexture(state.baseTextures[MONEY], { xOffset, 32 }, 3.0f, LEFT, CENTER);

	xOffset += 56;
	std::string m = formatNumber(game.countries[0].money);
	drawText(m, { xOffset, 32 }, 3.0f, { 0, 0, 0, 255 }, LEFT, CENTER);

	if (!running) {
		drawTexture(state.baseTextures[PAUSED], { state.res.x - (56 * 7), 8 }, 3.0f, LEFT, BOTTOM);
	}

	int speedHover = -1;
	for (int i = 0; i < 6; i++) {
		SDL_Rect rect = v2ToRect({ state.res.x - (56 * 6) + (56 * i), 8 }, { 48, 48 });
		SDL_Color fill = { 128, 128, 128, 255 };
		if (selectedSpeed == i) {
			fill = { 64, 64, 192, 255 };
		}

		drawRect(rect, { 0, 0, 0, 255 }, fill, { 128, 128, 192, 128 });
		if (mouseInRect(rect)) {
			speedHover = i;
		}
	}

	if (speedHover != -1 && state.mouseState.click) {
		selectedSpeed = speedHover;
	}
}

void Game::drawMinimap() {
	SDL_Rect outline = { 16, state.res.y - (state.res.y / 4) - 16, state.res.x / 4, state.res.y / 4 };
	drawRect(outline, { 0, 0, 0, 255 }, { 0, 255, 0, 255 });

	bool cameraChanged = false;
	for (city& c : cities) {
		v2<float> np = project(c.pos, v2<float>{ 0.0f, 0.0f }, state.res / 4, 0.01f);
		v2<int> sp = { 16 + int(np.x * (float)state.res.y / 4.0f), ((state.res.y * 3) / 4) + int(np.y * (float)state.res.y / 4.0f) - 16 };
		SDL_Point p = v2ToPoint(sp);

		if (!SDL_PointInRect(&p, &outline)) {
			continue;
		}

		if (c.capital) {
			SDL_Rect r = v2ToRect(sp - v2<int>{ 4, 4 }, { 8, 8 });
			drawRect(r, { 0, 0, 0, 255 }, game.countries[c.owner].color);

			if (mouseInRect(r) && state.mouseState.click) {
				camera.pos = c.pos;
				camera.zoom = 0.2f;
				cameraChanged = true;
			}
		} else if (c.population > 1000000) {
			SDL_Rect r = v2ToRect(sp - v2<int>{ 2, 2 }, { 4, 4 });
			drawRect(r, { 0, 0, 0, 255 }, game.countries[c.owner].color);
		} else {
			drawPoint(sp, { 0, 0, 0, 255 });
		}
	}

	for (naturalResource& r : naturalResources) {
		v2<float> np = project(r.pos, v2<float>{ 0.0f, 0.0f }, state.res / 4, 0.01f);
		v2<int> sp = { 16 + int(np.x * (float)state.res.y / 4.0f), ((state.res.y * 3) / 4) + int(np.y * (float)state.res.y / 4.0f) - 16 };
		SDL_Point p = v2ToPoint(sp);

		if (!SDL_PointInRect(&p, &outline)) {
			continue;
		}

		drawPoint(sp, { 0, 0, 255, 255 });
	}

	for (mine& m : mines) {
		v2<float> np = project(m.pos, v2<float>{ 0.0f, 0.0f }, state.res / 4, 0.01f);
		v2<int> sp = { 16 + int(np.x * (float)state.res.y / 4.0f), ((state.res.y * 3) / 4) + int(np.y * (float)state.res.y / 4.0f) - 16 };
		SDL_Point p = v2ToPoint(sp);

		if (!SDL_PointInRect(&p, &outline)) {
			continue;
		}

		drawPoint(sp, { 255, 0, 255, 255 });
	}

	v2<float> cp = project(camera.pos, { 0.0f, 0.0f }, state.res / 4, 0.01f);
	v2<int> csp = { 16 + int(cp.x * (float)state.res.y / 4.0f),  ((state.res.y * 3) / 4) + int(cp.y * (float)state.res.y / 4.0f) - 16 };

	SDL_Point p = v2ToPoint(csp);
	if (!SDL_PointInRect(&p, &outline)) {
		return;
	}

	drawLine(csp - v2<int>{4, 0}, csp + v2<int>{4, 0}, { 32, 32, 32, 192 });
	drawLine(csp - v2<int>{0, 4}, csp + v2<int>{0, 4}, { 32, 32, 32, 192 });

	if (mouseInRect(outline) && state.mouseState.click && !cameraChanged) {
		camera.pos = aproject({ (((float)state.mouseState.pos.x - 16.0f) * 4.0f) / (float)state.res.y, (((float)state.mouseState.pos.y + 16.0f) / ((float)state.res.y / 4.0f)) - 3.0f}, { 0.0f, 0.0f }, state.res / 4, 0.01f);
	}

	SDL_Rect viewport = v2ToRect(
		csp - ((v2fTov2i({ (float)state.res.x / (float)state.res.y, 1.0f }) * (int)(1.0f / camera.zoom))),
		v2fTov2i({ (float)state.res.x / (float)state.res.y, 1.0f }) * (int)(2.0f / camera.zoom)
	);

	drawRect(viewport, { 64, 64, 64, 128 }, { 128, 128, 128, 64 });
}

void Game::generateAlerts() {

}

bool Game::selectingSomething() {
	return selectedCity != -1 || selectedIndustry != -1 || selectedResource != -1 || selectedMine != -1 || selectedCountry != -1;
}

Game game;

void resetGameSettings() {
	game.camera.pos = game.cities[0].pos;
	game.camera.zoom = 0.2f;

	game.selectedCountry = -1;

	game.selectedCity = -1;
	game.selectedIndustry = -1;
	game.selectedResource = -1;
	game.selectedStorage = -1;
	game.selectedMine = -1;

	game.cityIndustryMenuOpen = false;
	game.cityStorageMenuOpen = false;

	game.storageInventoryMenuOpen = false;
	game.storageExportsMenuOpen = false;
	game.storageExportResourceSelected = -1;

	game.industryInventoryMenuOpen = false;
	game.industryImportsMenuOpen = false;
	game.industryExportsMenuOpen = false;
	game.industryImportResourceSelected = -1;
	game.industryExportResourceSelected = -1;

	game.mineInventoryMenuOpen = false;
	game.mineAllocateWorkersMenuOpen = false;
	game.mineAllocateWorkersMenuScroll = 0;
	game.mineExportsMenuOpen = false;
	game.mineExportResourceSelected = -1;

	game.occludeRects = {};
	game.mapTexture = SDL_CreateTexture(state.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, state.res.x, state.res.y);
	SDL_SetTextureBlendMode(game.mapTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(game.mapTexture, 240);

	memset(game.mapPixelArray, 0, (size_t)(4 * state.res.x * state.res.y));

	game.tickStart = std::chrono::high_resolution_clock::now();
	game.frameStart = std::chrono::high_resolution_clock::now();
}

void newGame() {
	clearGame();
	std::chrono::high_resolution_clock::time_point loadStart = std::chrono::high_resolution_clock::now();
	game.mode = NORMAL;

	game.playingCountry = 0;

	int cityCounter = 0;
	for (int i = 0; i < 5; i++) {
		game.countries.push_back(country("Country " + std::to_string(i), 1000000.0f, { (uint8_t)randf(game.gen, 0.0f, 255.0f), (uint8_t)randf(game.gen, 0.0f, 255.0f), (uint8_t)randf(game.gen, 0.0f, 255.0f), 255 }));

		v2<float> capitolPos = { randf(game.gen, -95.0f, 95.0f), randf(game.gen, -50.0f, 50.0f) };
		game.cities.push_back(city("Capitol", capitolPos, i, true));
		game.cities[cityCounter].population = 3000000;
		cityCounter++;

		for (int j = 0; j < 4; j++) {
			v2<float> pos = makeVector(capitolPos, randf(game.gen, 0.0f, 6.28f), randf(game.gen, 3.0f, 4.0f));
			game.cities.push_back(city("City " + std::to_string(j), pos, i, false));
			game.cities[cityCounter].population = (int)randf(game.gen, 1000000.0f, 3000000.0f);
			cityCounter++;
		}

		for (int j = 0; j < 10; j++) {
			v2<float> pos = makeVector(capitolPos, randf(game.gen, 0.0f, 6.28f), randf(game.gen, 4.0f, 6.0f));
			game.cities.push_back(city("City " + std::to_string(j), pos, i, false));
			game.cities[cityCounter].population = (int)randf(game.gen, 250000.0f, 1000000.0f);
			cityCounter++;
		}

		for (int j = 0; j < 20; j++) {
			v2<float> pos = makeVector(capitolPos, randf(game.gen, 0.0f, 6.28f), randf(game.gen, 6.0f, 8.0f));
			game.cities.push_back(city("City " + std::to_string(j), pos, i, false));
			game.cities[cityCounter].population = (int)randf(game.gen, 100000.0f, 250000.0f);
			cityCounter++;
		}

		for (int j = 0; j < 45; j++) {
			v2<float> pos = makeVector(capitolPos, randf(game.gen, 0.0f, 6.28f), randf(game.gen, 8.0f, 10.0f));
			game.cities.push_back(city("City " + std::to_string(j), pos, i, false));
			game.cities[cityCounter].population = (int)randf(game.gen, 25000.0f, 100000.0f);
			cityCounter++;
		}
	}

	for (int i = 0; i < gameData.rawResources.size(); i++) {
		for (int j = 0; j < 5; j++) {
			v2<float> startPos = { randf(game.gen, -95.0f, 95.0f), (randf(game.gen, -50.0f, 50.0f)) };
			int count = (int)randf(game.gen, 1.0f, 6.0f);
			for (int k = 0; k < count; k++) {
				v2<float> pos = makeVector(startPos, randf(game.gen, 0.0f, 6.28f), randf(game.gen, 1.0f, 2.0f));
				game.naturalResources.push_back(naturalResource(gameData.rawResources[i], pos));
			}
		}
	}

	game.countries[0].color = { 0, 0, 0, 255 };

	game.ticks = 0;
	game.frames = 0;

	game.selectedSpeed = 1;
	game.running = false;
	game.speedChangeTimer = 0;

	resetGameSettings();

	printf("Created new game in %lld microseconds.\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - loadStart).count());
}

void clearGame() {
	game.countries.clear();
	game.cities.clear();
	game.mines.clear();
	game.naturalResources.clear();
	SDL_DestroyTexture(game.mapTexture);
}