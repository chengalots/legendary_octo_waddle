//
//  Animation.cpp
//  SDL2_potato2
//
//  Created by pow on 2017-09-08.
//  Copyright © 2017 potato. All rights reserved.
//

#include "Animation.hpp"

const std::string Animation::NO_FILE_PATH = "-No file path provided-";

Animation::Animation(Texture *texture, SDL_Rect sheetBounds, SDL_Rect frameRect,
    int _totalFrames, double _fps, bool _loop) {

    spriteSheet = texture;
    timer = nullptr;
    frameBounds = {sheetBounds.x, sheetBounds.y, frameRect.w, frameRect.h};
    startTicks = pausedTicks = 0;
    _isRunning = _isPaused = false;
    name = NO_FILE_PATH;
    totalFrames = _totalFrames;
    currentFrame = 0;
    fps = _fps;
    speed = 1.0;
    loop = _loop;
    spriteSheetBounds = {sheetBounds.x, sheetBounds.y, sheetBounds.w, sheetBounds.h};
}

Animation::~Animation() {

    timer = nullptr;

    if(name == NO_FILE_PATH) {
        spriteSheet = nullptr;
        //printf("Animation [%p] spritesheet set to nullptr\n", this);
    }
    else {
        delete spriteSheet;
        //printf("Animation [%p] spritesheet deleted", this);
    }
    //printf("Animation deinit [%s] | [%p]\n", name.c_str(), this);
}

void Animation::render(SDL_Renderer *renderer, int x, int y, SDL_RendererFlip flip) {

    if(!loop && currentFrame == totalFrames - 1) {
        SDL_Point c = {frameBounds.w / 2, frameBounds.h / 2};
        spriteSheet->render(renderer, x, y, &frameBounds, 0, &c, flip);
        if(_isRunning) stop();
        return;
    }

    if(fps > 0 && !_isPaused && (double)(timer->getTicks() - startTicks) >= 1000.0/(fps * speed)) {
        if(name == NO_FILE_PATH) {
            frameBounds.x += frameBounds.w;
            currentFrame++;

            if(loop && currentFrame >= totalFrames) {
                frameBounds.x = spriteSheetBounds.x;
                frameBounds.y = spriteSheetBounds.y;
                currentFrame = 0;
            }
            else if(frameBounds.x >= spriteSheetBounds.w + spriteSheetBounds.x) {
                frameBounds.x = spriteSheetBounds.x;
                frameBounds.y += frameBounds.h;
            }
        }

        startTicks = timer->getTicks();
        //pausedTicks = 0;
    }
        //flipping requires a centre, 0 is a rotation angle
    SDL_Point c = {frameBounds.w / 2, frameBounds.h / 2};
    spriteSheet->render(renderer, x, y, &frameBounds, 0, &c, flip);
}

bool Animation::loadFromFile(SDL_Renderer *renderer, std::string path, double _fps) {

    fps = _fps;
    name = path;
    return spriteSheet->loadFromFile(renderer, path);
}

int Animation::getCurrentFrame() { return currentFrame; }

Size Animation::getFrameSize() { return {frameBounds.w, frameBounds.h}; }

SDL_Rect Animation::getFrameBounds() { return frameBounds; }

std::string Animation::getName() { return name; }

bool Animation::doesLoop() { return loop; }

bool Animation::isPaused() { return _isPaused; }

bool Animation::isRunning() { return _isRunning; }

void Animation::setFrameBounds(SDL_Rect newBounds) { frameBounds = {newBounds.x,
    newBounds.y, newBounds.w, newBounds.h}; }

void Animation::setName(std::string newName) { name = newName; }

void Animation::setSpeed(double newSpeed) { speed = newSpeed; }

void Animation::setTimer(Timer *newTimer) { timer = newTimer; }

void Animation::start() {

    if(timer == nullptr) return;

    startTicks = timer->getTicks();
    pausedTicks = 0;
    _isRunning = true;
    _isPaused = false;
    currentFrame = 0;
    frameBounds.x = spriteSheetBounds.x;
    frameBounds.y = spriteSheetBounds.y;
}

void Animation::stop() {

    if(timer == nullptr) return;

    _isRunning = false;
    _isPaused = false;
}

void Animation::pause() {

    if(timer == nullptr) return;
    if(!_isRunning || _isPaused) return;

    _isPaused = true;
    pausedTicks = timer->getTicks() - startTicks;
    startTicks = 0;
}

void Animation::unpause() {

    if(timer == nullptr) return;
    if(!_isRunning || !_isPaused) return;

    _isPaused = false;
    startTicks = timer->getTicks() - pausedTicks;
    pausedTicks = 0;
}
