//
//  PhysicsBody.cpp
//  SDL2_potato3
//
//  Created by pow on 2017-10-3.
//  Copyright © 2017 potato. All rights reserved.
//

#include "PhysicsBody.hpp"

PhysicsBody::PhysicsBody() {
    timer = nullptr;
    direction = NONE;
    _hasUnitCollision = true;
}

PhysicsBody::PhysicsBody(SDL_Rect bounds, Timer * newTimer) {
    hitbox = bounds;
    timer = newTimer;
}

void PhysicsBody::setTimer(Timer * newTimer) {
    timer = newTimer;
}

void PhysicsBody::receiveStatusEffect(StatusEffect effect) {
    if(effect.buff != StatusEffect::NO_BUFF) {
        switch(effect.buff) {
            case StatusEffect::BLOODLUST:
                if(buffEffectsReceived.count(StatusEffect::BLOODLUST) != 0) {
                    if(buffEffectsReceived.at(StatusEffect::BLOODLUST).stacks
                        < StatusEffect::BLOODLUST_STACK_LIMIT) {
                        buffEffectsReceived.at(StatusEffect::BLOODLUST).stacks++;
                    }
                    buffEffectsReceived.at(StatusEffect::BLOODLUST).startDurationTicks = timer->getTicks();
                }
                else {
                    effect.stacks = 1;
                    buffEffectsReceived.insert({effect.buff, effect});
                }
                break;
            default:
                break;
        }
    }
    if(effect.debuff != StatusEffect::NO_DEBUFF) {
        if(debuffEffectsReceived.count(effect.debuff) && debuffEffectsReceived.at(effect.debuff).stacks == 0){
            debuffEffectsReceived.erase(effect.debuff);
            debuffEffectsReceived.insert({effect.debuff, effect});
        }

        switch(effect.debuff) {
            case StatusEffect::KNOCK_UP:
                velocity.setDy(effect.duration);
                removeDebuffEffect(StatusEffect::KNOCK_UP);
                break;
            case StatusEffect::KNOCK_DOWN:
                break;
            default:
                break;
        }
    }
}

void PhysicsBody::removeBuffEffect(StatusEffect::BuffEffect type) {
    if(buffEffectsReceived.count(type) == 0) return;
    buffEffectsReceived.erase(type);
}

bool PhysicsBody::hasBuffEffect(StatusEffect::BuffEffect type) {
    if(buffEffectsReceived.count(type) == 0) return false;
    return true;
}

std::unordered_map<StatusEffect::BuffEffect, StatusEffect> PhysicsBody::getBuffEffectsReceived() {
    return buffEffectsReceived;
}

void PhysicsBody::removeDebuffEffect(StatusEffect::DebuffEffect type) {
    if(debuffEffectsReceived.count(type) == 0) return;
    debuffEffectsReceived.erase(type);
}

bool PhysicsBody::hasDebuffEffect(StatusEffect::DebuffEffect type) {
    if(debuffEffectsReceived.count(type) == 0) return false;
    return true;
}

std::unordered_map<StatusEffect::DebuffEffect, StatusEffect> PhysicsBody::getDebuffEffectsReceived() {
    return debuffEffectsReceived;
}

SDL_Rect PhysicsBody::getBounds() {
    return hitbox;
}

int PhysicsBody::w() { return hitbox.w; }

int PhysicsBody::h() { return hitbox.h; }

bool PhysicsBody::hasUnitCollision() { return _hasUnitCollision; }

void PhysicsBody::setUnitCollision(bool b) { _hasUnitCollision = b; }

SDL_Point PhysicsBody::location() {
    return {hitbox.x, hitbox.y};
}

void PhysicsBody::translate(int dx, int dy) {
    hitbox.x += dx;
    hitbox.y += dy;
}

void PhysicsBody::location(int x, int y) {
    hitbox.x = x;
    hitbox.y = y;
}

mVector PhysicsBody::getVelocity() {
    return velocity;
}

Direction PhysicsBody::getDirection() {
    return direction;
}

void PhysicsBody::setVelocity(double dx, double dy) {
    velocity.setDx(dx);
    velocity.setDy(dy);
}
void PhysicsBody::setXVelocity(double dx) {
    velocity.setDx(dx);
}

void PhysicsBody::setYVelocity(double dy) {
    velocity.setDy(dy);
}

void PhysicsBody::setDirection(Direction d) {
    direction = d;
}

void PhysicsBody::move() {

    if(timer == nullptr) return;

    double timePassed = (timer->getTicks() - moveStartTicks) / 1000.0;

    if(timePassed > 1) {
        moveStartTicks = timer->getTicks();
        return;
    }

    if(velocity.dx() != 0) hitbox.x += ((double)velocity.dx() * timePassed);
    if(velocity.dy() != 0) hitbox.y -= ((double)velocity.dy() * timePassed);

    moveStartTicks = timer->getTicks();
}
