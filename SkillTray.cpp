//
//  Loader.cpp
//  SDL2_potato4
//
//  Created by pow on 2017-09-25.
//  Copyright © 2017 potato. All rights reserved.
//

#include "SkillTray.hpp"

const int SkillTray::COLUMNS = 10;
const int SkillTray::ROWS = 2;

SkillTray::SkillTray(std::vector<Attack> * _playerSkills, Timer * _timer, SDL_Rect boundingBox) {

	font = TTF_OpenFont("Fonts/VeraMono.ttf", 16);
	if(font == nullptr) printf("Font failed to load. SDL2_ttf error: %s\n", TTF_GetError());

	labels = std::vector<Texture>(_playerSkills->size());
	playerSkills = _playerSkills;
	timer = _timer;
	bounds = boundingBox;
}

void SkillTray::render(SDL_Renderer * renderer) {

	SDL_RenderDrawRect(renderer, &bounds);

	std::string str;
	double cd;

	for(int i = 0; i < 1; i++) {
		for(int j = 0; j < labels.size(); j ++) {

			if(timer->getTicks() - playerSkills->at(j).coolDownStartTicks
				< playerSkills->at(j).coolDown) {

					cd = playerSkills->at(j).coolDown -
						(timer->getTicks() - playerSkills->at(j).coolDownStartTicks);
					cd /= 1000.0;

					ss.str(std::string());
					ss << std::fixed << std::setprecision(1) << cd;
					str = ss.str();
			}
			else {
				continue;
			}

			labels.at(i * COLUMNS + j).loadFromRenderedText(renderer, font, str, {255, 255, 255});
			labels.at(i * COLUMNS + j).render(renderer, j * (bounds.w / COLUMNS) + bounds.x
				, i * (bounds.h / ROWS) + bounds.y);
		}
	}
}
