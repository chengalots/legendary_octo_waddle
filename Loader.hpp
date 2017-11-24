//
//  Loader.hpp
//  SDL2_potato4
//
//  Created by pow on 2017-09-25.
//  Copyright © 2017 potato. All rights reserved.
//

#ifndef LOADER_HPP
#define LOADER_HPP
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <vector>
#include "Animation.hpp"
#include "Attack.hpp"
#include "StatusEffect.hpp"

class Loader {
public:
    enum LoadType {
        CHARACTER,
		PLAYER,
        ENEMY
    };

    Loader();
    void loadAnimations(SDL_Renderer * renderer, std::vector<Texture> * textures
                        , std::vector<Animation> * animations, LoadType type);
    void loadSkills(std::vector<Attack> * skills, LoadType type);

private:
    void loadCharAnimations(SDL_Renderer * renderer,
                            std::vector<Texture> * textures,
                            std::vector<Animation> * animations);
    void loadCharSkills(std::vector<Attack> * skills);

    std::ifstream fileIn;
};

#endif
