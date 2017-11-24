//
//  Texture.hpp
//  SDL2_potato2
//
//  Created by pow on 2017-09-08.
//  Copyright © 2017 potato. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Size.hpp"

class Texture {
public:
    Texture();
    ~Texture();
    void free();
    void render(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip = nullptr,
                double angle = 0.0, SDL_Point * center = nullptr,
                SDL_RendererFlip flip = SDL_FLIP_NONE);
    bool loadFromFile(SDL_Renderer * renderer, std::string path);
    bool loadFromRenderedText(SDL_Renderer * renderer, TTF_Font * font, std::string text, SDL_Color colour);
    Size getSize();
    std::string getName();
    void setName(std::string newName);

private:
    SDL_Texture *texture;
    std::string name;
    Size size;
};

#endif
