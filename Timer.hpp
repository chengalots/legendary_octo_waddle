//
//  Timer.hpp
//  SDL2_potato2
//
//  Created by pow on 2017-09-07.
//  Copyright © 2017 potato. All rights reserved.
//

#ifndef TIMER_HPP
#define TIMER_HPP
#include <SDL2/SDL.h>

class Timer {
public:
    Timer();
    //~Timer();
    void start();
    void stop();
    void pause();
    void unpause();
    bool isRunning();
    Uint32 getTicks();
    
private:
    Uint32 startTicks, pausedTicks;
    
    bool _isRunning, isPaused;
};

#endif
