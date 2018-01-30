//
//  Enemy.hpp
//  SDL2_potato4
//
//  Created by pow on 2017-09-21.
//  Copyright © 2017 potato. All rights reserved.
//

#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <SDL2/SDL.h>
#include <unordered_map>
#include <iostream>
#include "Character.hpp"
#include "PhysicsBody.hpp"

class Enemy : public Character{
public:
    static const int FOLLOW_SPEED;

    Enemy(SDL_Renderer *renderer, Timer *timer, SDL_Rect _hitbox, Size size);
    ~Enemy();

    void render(SDL_Renderer *renderer);

    void simpleFollowCharacter(Character* character);
    int getFollowRange();
    void setFollowRange(int newRange);

private:
        //timer for frame independent movement
    Timer *timer;
    int followRange;
};

#endif
