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
    Enemy(SDL_Renderer *renderer, Timer *timer, SDL_Rect _hitbox);
    ~Enemy();

    void render(SDL_Renderer *renderer);
        //frame independent movement along a vector
    /*void move();
    void removeBuffEffect(StatusEffect::BuffEffect type);
    bool hasBuffEffect(StatusEffect::BuffEffect type);
    std::unordered_map<StatusEffect::BuffEffect, StatusEffect> getBuffEffectsReceived();
    void removeDebuffEffect(StatusEffect::DebuffEffect type);
    void receiveStatusEffect(StatusEffect effect);
    bool hasDebuffEffect(StatusEffect::DebuffEffect type);
    std::unordered_map<StatusEffect::DebuffEffect, StatusEffect> getDebuffEffectsReceived();
    //SDL_Rect * getBounds();
    mVector getVelocity();
    void setTimer(Timer *newTimer);
    void setVelocity(int dx, int dy);
    void setXVelocity(int dx);
    void setYVelocity(int dy);*/

private:
        //timer for frame independent movement
    Timer *timer;
    //PhysicsBody physicsbody;
};

#endif
