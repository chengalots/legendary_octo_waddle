//
//  Character.cpp
//  SDL2_potato3
//
//  Created by pow on 2017-09-10.
//  Copyright © 2017 potato. All rights reserved.
//

#include "Character.hpp"

    //public constants
const int Character::CHAR_W = 60;
const int Character::CHAR_H = 90;
const int Character::JUMP_VEL = 610;
const int Character::MAX_CHAR_VEL = 400;
const int Character::CHAIN_TIME_LIMIT = 100;

Character::Character(SDL_Renderer * renderer, Timer * newTimer, SDL_Point spawnPoint, Size size) {

    timer = newTimer;

    moveStartTicks = 0;

    animationIndex = 0;
    skillIndex = 0;
    hasUnitCollision = true;
    canMoveUp = canMoveDown = canMoveLeft = canMoveRight = false;
    previousFrame = -1;
    physicsbody = PhysicsBody({spawnPoint.x, spawnPoint.y, size.w, size.h}, newTimer);
    physicsbody.setDirection(RIGHT);
    hp = 100;
    jumpCap = 1;
    jumpCounter = 0;

    std::srand(SDL_GetTicks());
}

Character::~Character() {

    timer = nullptr;
}

void Character::render(SDL_Renderer *renderer) {

    if(!animations.at(animationIndex).isRunning()) {

        if(std::abs(physicsbody.getVelocity().dx()) < 1 && std::abs(physicsbody.getVelocity().dy()) < 1) {

            animationIndex = IDLE;
            animations.at(animationIndex).start();
        }
        else if(physicsbody.getVelocity().dy() < -10) {

            animationIndex = FALLING;
            animations.at(animationIndex).start();
        }
    }

    if(skillIndex >= 0 &&
        animationIndex == skillIndex + SKILLS_START && animations.at(animationIndex).isRunning()) {

        if(animations.at(animationIndex).getCurrentFrame() == skills.at(skillIndex).velStartFrame) {

            if(physicsbody.getDirection() == LEFT) {
                skillVelocity.setDx(skills.at(skillIndex).skillVelocity.dx());
            }
            else if(physicsbody.getDirection() == RIGHT) {
                skillVelocity.setDx(-skills.at(skillIndex).skillVelocity.dx());
            }

            skillVelocity.setDy(skills.at(skillIndex).skillVelocity.dy());
        }
        else if(animations.at(animationIndex).getCurrentFrame() == skills.at(skillIndex).velEndFrame) {

            skillVelocity.setDx(0);
        }
    }

    if(chainTimer.getTicks() > CHAIN_TIME_LIMIT) chainTimer.stop();

    SDL_RendererFlip flip;

    switch (physicsbody.getDirection()) {
        case LEFT:
            flip = SDL_FLIP_HORIZONTAL;
            break;
        case RIGHT:
            flip = SDL_FLIP_NONE;
            break;
        default:
            flip = SDL_FLIP_NONE;
            break;
    }

    int xOffset = 0, yOffset = 0;

    if(!animations.empty() && animationIndex >= 0 && animationIndex < animations.size()) {

        if(animations.at(animationIndex).getFrameBounds().w > physicsbody.w()) {
            xOffset = (animations.at(animationIndex).getFrameBounds().w - physicsbody.w()) / 2;
        }
        if(animations.at(animationIndex).getFrameBounds().h > physicsbody.h()) {
            yOffset = (animations.at(animationIndex).getFrameBounds().h - physicsbody.h());
        }

        animations.at(animationIndex).render(renderer, physicsbody.location().x - xOffset,
            physicsbody.location().y - yOffset,
                         flip);
    }
}

void Character::useSkill(int skill, Direction d) {

    if(skill < 0) {
        return;
    }

    if(d != NONE) physicsbody.setDirection(d);

    animations.at(animationIndex).stop();

    if(timer->getTicks() - skills.at(skill).coolDownStartTicks >= skills.at(skill).coolDown) {
        skillIndex = skill;
        animationIndex = skill + (int)SKILLS_START;
        animations.at(animationIndex).setSpeed(1.0);
        animations.at(animationIndex).start();

        chainTimer.stop();
        previousFrame = -1;
        skillVelocity.setDx(0);
        skillVelocity.setDy(0);

    }
}

Attack Character::getSkill() {

    if(animations.at(animationIndex).getCurrentFrame() == previousFrame) {
        return {};
    }

    if(skillIndex < 0) {
        return {};
    }

    if(!animations.at(skillIndex + SKILLS_START).isRunning()) {
        return {};
    }

    Attack * s = &skills.at(skillIndex);
    Attack skill = {};
    DamageFrame damageFrame = {};
    int currentFrame = animations.at(animationIndex).getCurrentFrame();

    if(s->damageFrames.count(currentFrame) != 0) {
        damageFrame = s->damageFrames.at(currentFrame);

        if(s->damageFrames.begin()->first == currentFrame) {
            s->coolDownStartTicks = timer->getTicks();
        }

        skill.damageFrames.insert({0, damageFrame});

        if(physicsbody.getDirection() == LEFT) {
            skill.damageFrames.at(0).hitbox.x = physicsbody.location().x
                - damageFrame.hitbox.w + (physicsbody.w() / 2) - damageFrame.hitbox.x;
        }
        else {
            skill.damageFrames.at(0).hitbox.x = physicsbody.location().x + (physicsbody.w() / 2)
                + damageFrame.hitbox.x;
        }

        skill.damageFrames.at(0).hitbox.y += physicsbody.location().y;

        if(physicsbody.getBuffEffectsReceived().count(StatusEffect::BLOODLUST) != 0) {
            skill.damageFrames.at(0).damage *=
            (( (double)physicsbody.getBuffEffectsReceived().at(StatusEffect::BLOODLUST).stacks
                + 1.0)/100.0 + 1.0);
        }

        skill.debuffEffectsApplied = s->debuffEffectsApplied;
        skill.buffEffectsApplied = s->buffEffectsApplied;
        skill.attackID = skillIndex;

        if(!animations.at(animationIndex).isRunning()) {
            previousFrame = -1;
        }

        previousFrame = animations.at(animationIndex).getCurrentFrame();
    }

    if(s->chainStart <= currentFrame && currentFrame <= s->chainEnd) {
        chainTimer.start();
    }

    s = nullptr;

    return skill;
}

void Character::setTimer(Timer *newTimer) {
    timer = newTimer;
    physicsbody.setTimer(newTimer);
}

SDL_Rect Character::getBounds() { return physicsbody.getBounds(); }

void Character::translate(int dx, int dy) { physicsbody.translate(dx, dy); }

SDL_Point Character::location() { return physicsbody.location(); }

std::vector<Attack> * Character::getSkillSet() { return &skills; }

Direction Character::getDirection() { return physicsbody.getDirection(); }

void Character::setDirection(Direction d) {

    if(skillIndex >= 0) {
        if(animations.at(skillIndex + SKILLS_START).isRunning()) return;
    }
    physicsbody.setDirection(d);
}

void Character::jump() { jumpCounter++; }

void Character::resetJumpCounter() { jumpCounter = 0; }

int Character::getJumpCounter() {

    if(skillIndex >= 0) {
        if(animations.at(skillIndex + SKILLS_START).isRunning()) {
            return jumpCap;
        }
    }
    return jumpCounter;
}

void Character::setJumpCounter(int j) { jumpCounter = j; }

mVector Character::getVelocity() {

    if(skillIndex >= 0) {
        if(animations.at(skillIndex + SKILLS_START).isRunning()) {
            return skillVelocity;
        }
    }
    return physicsbody.getVelocity();
}

void Character::setVelocity(int dx, int dy) {
    physicsbody.setVelocity(dx, dy);
}

void Character::setXVelocity(int dx, bool forceRun) {
    physicsbody.setXVelocity(dx);

    if(skillIndex < 0 || !animations.at(skillIndex + SKILLS_START).isRunning()) {
        if(std::abs(physicsbody.getVelocity().dx()) > 1 && std::abs(physicsbody.getVelocity().dy()) < 1 ) {
            animationIndex = RUNNING;
            if(!animations.at(animationIndex).isRunning()) animations.at(animationIndex).start();
        }
        else if(!forceRun && std::abs(physicsbody.getVelocity().dx()) < 1
            && std::abs(physicsbody.getVelocity().dy()) < 1 && animationIndex == RUNNING) {
            animations.at(animationIndex).stop();
            animationIndex = IDLE;
            animations.at(animationIndex).start();
            skillVelocity.setDx(0);
        }
    }
}

void Character::setYVelocity(int dy) {
    physicsbody.setYVelocity(dy);

    if(skillIndex <= 0 || !animations.at(skillIndex + SKILLS_START).isRunning()) {
        if(physicsbody.getVelocity().dy() < -25) {
            animations.at(animationIndex).stop();
            animationIndex = FALLING;
            animations.at(animationIndex).start();
        }
        else if(physicsbody.getVelocity().dy() >= 1) {
            animations.at(animationIndex).stop();
            animationIndex = JUMP;
            animations.at(animationIndex).start();
        }
        else if(std::abs(physicsbody.getVelocity().dy()) < 1 && animationIndex == FALLING) {
            animations.at(animationIndex).stop();
            if(std::abs(physicsbody.getVelocity().dx()) < 1) animationIndex = IDLE;
            else animationIndex = RUNNING;
            animations.at(animationIndex).start();
        }
    }
    else if(animations.at(skillIndex + SKILLS_START).isRunning()) {
        skillVelocity.setDy(dy);
    }
}

void Character::move() { physicsbody.move(); }

void Character::receiveStatusEffect(StatusEffect effect) { physicsbody.receiveStatusEffect(effect); }

void Character::removeBuffEffect(StatusEffect::BuffEffect type) { physicsbody.removeBuffEffect(type); }

bool Character::hasBuffEffect(StatusEffect::BuffEffect type) { return physicsbody.hasBuffEffect(type); }

std::unordered_map<StatusEffect::BuffEffect, StatusEffect> Character::getBuffEffectsReceived()
{ return physicsbody.getBuffEffectsReceived(); }

void Character::removeDebuffEffect(StatusEffect::DebuffEffect type) { physicsbody.removeDebuffEffect(type); }

bool Character::hasDebuffEffect(StatusEffect::DebuffEffect type) { return physicsbody.hasDebuffEffect(type); }

std::unordered_map<StatusEffect::DebuffEffect, StatusEffect> Character::getDebuffEffectsReceived()
{ return physicsbody.getDebuffEffectsReceived(); }
