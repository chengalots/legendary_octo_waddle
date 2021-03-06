//
//  Player.cpp
//  SDL2_potato3
//
//  Created by pow on 2017-10-15.
//  Copyright © 2017 potato. All rights reserved.
//

#include "Player.hpp"

const int Player::width = 60, Player::height = 90;
const int Player::SKILLSET_SIZE = 10;
const int Player::NUM_OF_ANIMATIONS = 14;

Player::Player(SDL_Renderer * renderer, Timer * newTimer, SDL_Point spawnPoint) :
    Character(newTimer, spawnPoint, {width, height}) {
    animations.clear();
    skills.clear();

    skills = std::vector<Attack>(SKILLSET_SIZE);

    Loader loader;
    loader.loadAnimations(renderer, &animations, NUM_OF_ANIMATIONS, Loader::PLAYER);

    for(unsigned int i = 0; i < animations.size(); i++) {
        animations.at(i)->setTimer(newTimer);
    }

    loader.loadSkills(&skills, Loader::PLAYER);

    jumpCap = 2;
}

void Player::update() {
    Character::update();
}

void Player::setXVelocity(int dx) {
    Character::setXVelocity(dx);
}

void Player::move() {}

void Player::useSkill(AttackType t, Direction d) {
    if(t == DODGE && !animations.at(ROLL + SKILLS_START)->isRunning()) {
        if(animations.at(skillIndex + SKILLS_START)->isRunning()
            && skills.at(skillIndex).hasAnimationLock
            && !chainTimer.isRunning()) return;
        Character::useSkill(ROLL, d);
        return;
    }
    if(chainTimer.isRunning()) {
        if(t == LIGHT) {
            if(skills.at(skillIndex).skillChains[0] >= 0) {
                Character::useSkill(skills.at(skillIndex).skillChains[0], d);
            }
        }
        else if(t == HEAVY) {
            if(skills.at(skillIndex).skillChains[0] >= 0) {
                Character::useSkill(skills.at(skillIndex).skillChains[1], d);
            }
        }
    }
    else if(!animations.at(skillIndex + SKILLS_START)->isRunning()) {
        if(t == LIGHT) {
            Character::useSkill(OHS, d);
        }
        else if(t == HEAVY) {
            Character::useSkill(HEARTTHRUST, d);
        }
    }
}
