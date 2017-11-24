//
//  Timer.cpp
//  SDL2_potato2
//
//  Created by pow on 2017-09-07.
//  Copyright © 2017 potato. All rights reserved.
//

#include "Timer.hpp"

Timer::Timer() {
    
    startTicks = 0;
    pausedTicks = 0;
    _isRunning = false;
    isPaused = false;
}

//Timer::~Timer() { printf("Timer deinit [%p]\n", this); }

void Timer::start() {
    
    startTicks = SDL_GetTicks();
    pausedTicks = 0;
    _isRunning = true;
    isPaused = false;
}

void Timer::stop() {
    
    _isRunning = false;
    isPaused = false;
    startTicks = 0;
    pausedTicks = 0;
}

void Timer::pause() {
    
    if(!_isRunning || isPaused) return;
    
    pausedTicks = SDL_GetTicks() - startTicks;
    startTicks = 0;
    isPaused = true;
}

void Timer::unpause() {
    
    if(!_isRunning || !isPaused) return;
    
    startTicks = SDL_GetTicks() - pausedTicks;
    pausedTicks = 0;
    isPaused = false;
}

bool Timer::isRunning() { return _isRunning; }

Uint32 Timer::getTicks() {
    
    if(!_isRunning) return 0;
    if(isPaused) return pausedTicks;
    
    return SDL_GetTicks() - startTicks;
}
