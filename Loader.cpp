//
//  Loader.cpp
//  SDL2_potato4
//
//  Created by pow on 2017-09-25.
//  Copyright © 2017 potato. All rights reserved.
//

#include "Loader.hpp"

Loader::Loader() { }

void Loader::loadAnimations(SDL_Renderer * renderer, std::vector<Animation *> * animations, int numOfAnimations, LoadType type) {

    switch (type) {
        case PLAYER:
            loadCharAnimations(renderer, animations, numOfAnimations);
            break;
        case ENEMY:
            loadCharAnimations(renderer, animations, numOfAnimations);
            break;
        default:
            break;
    }
}

void Loader::loadSkills(std::vector<Attack> * skills, LoadType type) {
    switch (type) {
        case PLAYER:
            loadCharSkills(skills);
            break;
        case ENEMY:
            loadCharSkills(skills);
            break;
        default:
            break;
    }
}

void Loader::loadCharAnimations(SDL_Renderer * renderer, std::vector<Animation *> * animations, int numOfAnimations) {

    if(fileIn.is_open()) fileIn.close();

    fileIn.open("Char_Textures_List.txt");

    if(!fileIn) {
        std::cout << "Could not open Char_Textures_List.txt" << std::endl;
        return;
    }

    animations->clear();

    std::string input;
    SDL_Rect sheetBounds, frameRect;
    int totalFrames, fps;
    bool loop = false;

    for(int i = 0; i < numOfAnimations; i++) {

        std::getline(fileIn, input);
        std::string path = input;

        std::getline(fileIn, input);

        do {

            sscanf(input.c_str(), "%d,%d,%d,%d", &sheetBounds.x, &sheetBounds.y,
                &sheetBounds.w, &sheetBounds.h);

            std::getline(fileIn, input);
            sscanf(input.c_str(), "%d,%d,%d,%d", &frameRect.x, &frameRect.y, &frameRect.w, &frameRect.h);

            std::getline(fileIn, input);
            sscanf(input.c_str(), "%d,%d", &totalFrames, &fps);

            std::getline(fileIn, input);
            if(input == "true") loop = true;
            else if(input == "false") loop = false;

            animations->push_back(new Animation(path, renderer, sheetBounds, frameRect, totalFrames, fps, loop));

            std::getline(fileIn, input);

        } while(!input.empty());
    }

    fileIn.close();
}

void Loader::loadCharSkills(std::vector<Attack> * skills) {

    if(fileIn.is_open()) fileIn.close();

    fileIn.open("Char_Skills_List.txt");

    if(!fileIn) {
        std::cout << "Could not open Char_Skills_List.txt" << std::endl;
        return;
    }

    /*
    // ORDER:
    //        attackID
    //         [x]hitbox
    //        [x]damage
    //        [x]damage frames
    //        debuff effects
    //        debuff effect duration
    //        buff effects
    //        buff effect duration
    //        chain frames interval
    //        skill chains
    //        cooldown -> in ms
    //        skill velocity
    //        skill velocity frame interval
    //        requires chain -> bool
    //        has animation lock -> bool
    */

    std::stringstream ss;
    std::string input, token;
    Attack * s;
    int num;

    for(unsigned int i = 0; i < skills->size(); i++) {
        s = &skills->at(i);

        std::getline(fileIn, input);
        s->attackID = std::atoi(input.c_str());

        std::getline(fileIn, input);

        if(!input.empty()) {
            DamageFrame df = {};
            ss = std::stringstream(input);
            while(std::getline(ss, token, ';')) {
                sscanf(token.c_str(), "%d,%d, {%d,%d,%d,%d}", &(df.frame), &(df.damage),
                        &(df.hitbox.x), &(df.hitbox.y), &(df.hitbox.w),
                        &(df.hitbox.h));
                s->damageFrames.insert({df.frame, df});
            }
        }


        std::getline(fileIn, input);
            //debuff effects
        if(!input.empty()) {

            ss = std::stringstream(input);
            while(ss >> num) {

                s->debuffEffectsApplied.push_back({(StatusEffect::DebuffEffect)num,
                    StatusEffect::NO_BUFF, 0, 0, 0});
                if(ss.peek() == ',') ss.ignore();
            }
        }

        std::getline(fileIn, input);
            //debuff effect duration
        if(!input.empty()) {

            ss = std::stringstream(input);
            for(auto &effect : s->debuffEffectsApplied) {
                if(!(ss >> num)) continue;
                if(num < 0) num = 0;
                effect.duration = num;
                if(ss.peek() == ',') ss.ignore();
            }
        }

        std::getline(fileIn, input);
            //buff effects
        if(!input.empty()) {
            ss = std::stringstream(input);
            while(ss >> num) {
                s->buffEffectsApplied.push_back({StatusEffect::NO_DEBUFF,
                    (StatusEffect::BuffEffect)num, 0, 0, 0});
                if(ss.peek() == ',') ss.ignore();
            }
        }

        std::getline(fileIn, input);
            //buff effect duration
        if(!input.empty()) {

            ss = std::stringstream(input);
            for(auto &effect : s->buffEffectsApplied) {
                if(!(ss >> num)) continue;
                if(num < 0) num = 0;
                effect.duration = num;
                if(ss.peek() == ',') ss.ignore();
            }
        }

        std::getline(fileIn, input);
            //chain frames interval
        sscanf(input.c_str(), "%d,%d", &s->chainStart, &s->chainEnd);

        if(s->chainStart < 0 || s->chainEnd < s->chainStart) {

            s->chainEnd = -1;
        }

        std::getline(fileIn, input);
            //skill chains
        sscanf(input.c_str(), "%d,%d", &s->skillChains[0], &s->skillChains[1]);
        /*if(!input.empty()){

            ss = std::stringstream(input);
            while(ss >> num) {

                s->skillChains.insert(num);
                if(ss.peek() == ',') ss.ignore();
            }
        }*/
            //cooldown
        std::getline(fileIn, input);
        s->coolDown = std::atoi(input.c_str());

        s->coolDownStartTicks = -10000000;
            //skill velocity
        std::getline(fileIn, input);
        int dx, dy;
        sscanf(input.c_str(), "%d,%d", &dx, &dy);
        s->skillVelocity.setDx(dx);
        s->skillVelocity.setDy(dy);
            //skill velocity frame interval
        std::getline(fileIn, input);
        int vsf, vef;
        sscanf(input.c_str(), "%d,%d", &vsf, &vef);
        s->velStartFrame = vsf;
        s->velEndFrame = vef;

        std::getline(fileIn, input);
            //requires chain
        //if(input == "true") s->requiresChain = true;
        //else if(input == "false") s->requiresChain = false;

        std::getline(fileIn, input);
            //has animation lock
        if(input == "true") s->hasAnimationLock = true;
        else if(input == "false") s->hasAnimationLock = false;

        std::getline(fileIn, input);
    }

    s = nullptr;

    fileIn.close();
}
