//
//  Enemy.cpp
//  SDL2_potato4
//
//  Created by pow on 2017-09-21.
//  Copyright © 2017 potato. All rights reserved.
//

#include "Enemy.hpp"

Enemy::Enemy(SDL_Renderer *renderer, Timer *_timer, SDL_Rect _hitbox, Size size) :
    Character(_timer, {_hitbox.x, _hitbox.y}, size) {

    timer = _timer;
    //physicsbody = PhysicsBody(_hitbox, _timer);

    textures = {14, Texture()};
    skills = std::vector<Attack>(10);

    Loader loader;
    loader.loadAnimations(renderer, &textures, &animations, Loader::ENEMY);

    for(unsigned int i = 0; i < animations.size(); i++) {
        animations.at(i).setTimer(timer);
    }

    loader.loadSkills(&skills, Loader::ENEMY);
}

Enemy::~Enemy() {

}

void Enemy::render(SDL_Renderer *renderer) {
    Character::render(renderer);
    //SDL_Rect bounds = Character::getBounds();
    //SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0, 0xFF);
    //SDL_RenderDrawRect(renderer, &bounds);
}

/*void Enemy::move() { physicsbody.move(); }

void Enemy::removeBuffEffect(StatusEffect::BuffEffect type) { physicsbody.removeBuffEffect(type); }

bool Enemy::hasBuffEffect(StatusEffect::BuffEffect type) { return physicsbody.hasBuffEffect(type); }

std::unordered_map<StatusEffect::BuffEffect, StatusEffect> Enemy::getBuffEffectsReceived()
{ return physicsbody.getBuffEffectsReceived(); }

void Enemy::removeDebuffEffect(StatusEffect::DebuffEffect type) { physicsbody.removeDebuffEffect(type); }

void Enemy::receiveStatusEffect(StatusEffect effect) { physicsbody.receiveStatusEffect(effect); }

bool Enemy::hasDebuffEffect(StatusEffect::DebuffEffect type) { return physicsbody.hasDebuffEffect(type); }

std::unordered_map<StatusEffect::DebuffEffect, StatusEffect> Enemy::getDebuffEffectsReceived()
{ return physicsbody.getDebuffEffectsReceived(); }

//SDL_Rect * Enemy::getBounds() { return physicsbody.getBounds(); }

mVector Enemy::getVelocity() { return physicsbody.getVelocity(); }

void Enemy::setTimer(Timer *newTimer) {
    timer = newTimer;
    physicsbody.setTimer(newTimer);
 }

void Enemy::setVelocity(int dx, int dy) { physicsbody.setVelocity(dx, dy); }

void Enemy::setXVelocity(int dx) { physicsbody.setXVelocity(dx); }

void Enemy::setYVelocity(int dy) { physicsbody.setYVelocity(dy); }*/
