//
//  SkillTray.hpp
//  SDL2_potato4
//
//  Created by pow on 2017-09-29.
//  Copyright © 2017 potato. All rights reserved.
//
#ifndef SKILLTRAY_HPP
#define SKILLTRAY_HPP
#include <vector>
#include <sstream>
#include <iomanip>
#include "Attack.hpp"

class SkillTray {
public:
	SkillTray(std::vector<Attack> * _playerSkills, Timer * _timer, SDL_Rect boundingBox);
	void render(SDL_Renderer * renderer);

private:
	static const int COLUMNS;
	static const int ROWS;

	TTF_Font * font;
	std::vector<Attack> * playerSkills;
	std::vector<Texture> labels;
	SDL_Rect bounds;
	Timer * timer;
	std::stringstream ss;
};

#endif
