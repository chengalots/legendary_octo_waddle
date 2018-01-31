//
//  Enemy.cpp
//  SDL2_potato4
//
//  Created by pow on 2017-09-21.
//  Copyright © 2017 potato. All rights reserved.
//

#include "Enemy.hpp"

const int Enemy::FOLLOW_SPEED = Character::MAX_CHAR_VEL * 3 / 4;

Enemy::Enemy(SDL_Renderer *renderer, Timer *_timer, SDL_Rect _hitbox, Size size) :
    Character(_timer, {_hitbox.x, _hitbox.y}, size) {

    timer = _timer;

    followRange = 500;

    skills = std::vector<Attack>(10);

    Loader loader;
    loader.loadAnimations(renderer, &animations, 14, Loader::ENEMY);

    for(unsigned int i = 0; i < animations.size(); i++) {
        animations.at(i)->setTimer(timer);
    }

    loader.loadSkills(&skills, Loader::ENEMY);
}

Enemy::~Enemy() {

}

void Enemy::render(SDL_Renderer *renderer) {
    Character::render(renderer);
}

void Enemy::simpleFollowCharacter(Character* character) {
    if(character->location().x - getBounds().w * 2 > location().x
    ||location().x > character->location().x + character->getBounds().w + getBounds().w * 2) {
        if(character->location().x > location().x + getBounds().w * 2) {
            setDirection(RIGHT);
            setXVelocity(FOLLOW_SPEED);
        }
        else if(character->location().x + character->getBounds().w < location().x) {
            setDirection(LEFT);
            setXVelocity(-FOLLOW_SPEED);
        }
    }
    else {
        setXVelocity(0);
    }
}

int Enemy::getFollowRange() { return followRange; }
void Enemy::setFollowRange(int newRange) { followRange = newRange; }
