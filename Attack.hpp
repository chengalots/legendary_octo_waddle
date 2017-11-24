//
//  Attack.hpp
//  SDL2_potato3
//
//  Created by pow on 2017-09-18.
//  Copyright © 2017 potato. All rights reserved.
//

#ifndef ATTACK_HPP
#define ATTACK_HPP
#include <SDL2/SDL.h>
#include <unordered_set>
#include <map>
#include <vector>
#include "Animation.hpp"
#include "mVector.hpp"
#include "StatusEffect.hpp"

typedef struct {
    int frame;
    int damage;
    SDL_Rect hitbox;
} DamageFrame;

typedef struct {
    int attackID;

    std::map<int, DamageFrame> damageFrames;
        //which frames start the chain timer
    int chainStart, chainEnd;
        //how long before skill can be used again
    int coolDown;
        //when the cooldown started
    int coolDownStartTicks;
    //bool requiresChain;
        //determines if char is locked in animation
    bool hasAnimationLock;
    mVector skillVelocity;
    int velStartFrame, velEndFrame;
        //which skills does this attack chain into
    //std::unordered_set<int> skillChains;
    int skillChains[2];
        //
    std::vector<StatusEffect> debuffEffectsApplied;
    std::vector<StatusEffect> buffEffectsApplied;
} Attack;

#endif
