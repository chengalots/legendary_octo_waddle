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
const int Player::NUM_OF_TEXTURES = 14;

Player::Player(SDL_Renderer * renderer, Timer * newTimer, SDL_Point spawnPoint) :
    Character(newTimer, spawnPoint, {width, height}) {
    textures.clear();
    animations.clear();
    skills.clear();

    textures = {NUM_OF_TEXTURES, Texture()};
    skills = std::vector<Attack>(SKILLSET_SIZE);

    Loader loader;
    loader.loadAnimations(renderer, &textures, &animations, Loader::PLAYER);

    for(unsigned int i = 0; i < animations.size(); i++) {
        animations.at(i).setTimer(newTimer);
    }

    loader.loadSkills(&skills, Loader::PLAYER);

    jumpCap = 2;

    nextSkill = END;
}

void Player::update() {
    Character::update();
    if(nextSkill != (int)END && chainTimer.isRunning()) {
        Character::useSkill(nextSkill, Character::getDirection());
        nextSkill = (int)END;
    }
    else if(!animations.at(skillIndex + Character::SKILLS_START).isRunning()) nextSkill = (int)END;
}

void Player::setXVelocity(int dx) {
    Character::setXVelocity(dx);
}

void Player::move() {}

void Player::useSkill(AttackType t, Direction d) {
    if(t == DODGE && !animations.at(ROLL + SKILLS_START).isRunning()) {
        if(animations.at(skillIndex + SKILLS_START).isRunning()
            && skills.at(skillIndex).hasAnimationLock
            && !chainTimer.isRunning()) return;
        nextSkill = (int)END;
        Character::useSkill(ROLL, d);
        return;
    }
    if(chainTimer.isRunning()) {
        if(t == LIGHT) {
            if(skills.at(skillIndex).skillChains[0] >= 0) {
                nextSkill = (int)END;
                Character::useSkill(skills.at(skillIndex).skillChains[0], d);
            }
        }
        else if(t == HEAVY) {
            if(skills.at(skillIndex).skillChains[0] >= 0) {
                nextSkill = (int)END;
                Character::useSkill(skills.at(skillIndex).skillChains[1], d);
            }
        }
    }
    else if(!animations.at(skillIndex + SKILLS_START).isRunning()) {
        if(t == LIGHT) {
            nextSkill = (int)END;
            Character::useSkill(OHS, d);
        }
        else if(t == HEAVY) {
            nextSkill = (int)END;
            Character::useSkill(HEARTTHRUST, d);
        }
    }
    else {
        if(t == LIGHT) {
            if(skills.at(skillIndex).skillChains[0] >= 0)
                nextSkill = skills.at(skillIndex).skillChains[0];
        }
        else if(t == HEAVY) {
            if(skills.at(skillIndex).skillChains[0] >= 0)
                nextSkill = skills.at(skillIndex).skillChains[1];
        }
    }
}
