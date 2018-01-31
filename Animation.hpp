//
//  Animation.hpp
//  SDL2_potato2
//
//  Created by pow on 2017-09-08.
//  Copyright © 2017 potato. All rights reserved.
//

#ifndef ANIMATION_HPP
#define ANIMATION_HPP
#include <SDL2/SDL.h>
#include <string>
#include "Texture.hpp"
#include "Timer.hpp"
//#include "Size.hpp"

class Animation {
public:
    Animation(std::string path, SDL_Renderer * renderer, SDL_Rect sheetBounds, SDL_Rect frameRect,
              int _totalFrames, double _fps = 0, bool _loop = true);
    ~Animation();
    void render(SDL_Renderer *renderer, int x, int y,
                SDL_RendererFlip flip = SDL_FLIP_NONE);
    bool loadFromFile(SDL_Renderer *renderer, std::string path, double _fps);
    int getCurrentFrame();
    SDL_Rect getFrameBounds();
    Size getFrameSize();
    bool doesLoop();
    bool isPaused();
    bool isRunning();
    void setFrameBounds(SDL_Rect newBounds);
    void setSpeed(double newSpeed);
    void setTimer(Timer *newTimer);
    void start();
    void stop();
    void pause();
    void unpause();

private:
    static const std::string NO_FILE_PATH;

    Texture *spriteSheet;
    SDL_Rect spriteSheetBounds;
    Timer *timer;
    SDL_Rect frameBounds;

    Uint32 startTicks, pausedTicks;

    double fps;
    double speed;
    int currentFrame, totalFrames;
    bool _isPaused, _isRunning;
    bool loop;
};

#endif
