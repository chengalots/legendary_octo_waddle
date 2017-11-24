//
//  Texture.cpp
//  SDL2_potato2
//
//  Created by pow on 2017-09-08.
//  Copyright © 2017 potato. All rights reserved.
//

#include "Texture.hpp"

Texture::Texture() {

    texture = nullptr;
    name = "";
    size.w = 0;
    size.h = 0;
}

Texture::~Texture() {

    SDL_DestroyTexture(texture);
    texture = nullptr;
    size = {0, 0};
    //printf("Texture deinit [%s] | [%p]\n", name.c_str(), this);
}

void Texture::render(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip, double angle, SDL_Point *center,
	SDL_RendererFlip flip) {

    SDL_Rect renderToRect = {x, y, clip == nullptr ? size.w : clip->w, clip == nullptr ? size.h : clip->h};
    SDL_RenderCopyEx(renderer, texture, clip, &renderToRect, angle, center, flip);
}

bool Texture::loadFromFile(SDL_Renderer *renderer, std::string path) {

    SDL_Texture *newTexture = nullptr;
    SDL_Surface *surface = IMG_Load(path.c_str());

    if(surface == nullptr) {
        printf("Could not load surface from path %s\n. SDL2_image error: %s\n", path.c_str(), IMG_GetError());
        return false;
    }

    if((newTexture = SDL_CreateTextureFromSurface(renderer, surface)) == nullptr) {
        printf("Could not create texture from surface. SDL2_image error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return false;
    }

    if(texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
        size = {0, 0};
    }

    size.w = surface->w;
    size.h = surface->h;
    SDL_FreeSurface(surface);
    texture = newTexture;
    name = path;
    return true;
}

bool Texture::loadFromRenderedText(SDL_Renderer * renderer, TTF_Font * font,
	std::string text, SDL_Color colour) {

	SDL_Texture *newTexture = nullptr;
	SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), colour);

	if(surface == nullptr) {
		printf("Could not load surface from text. SDL2_ttf error: %s\n", TTF_GetError());
		return false;
	}

	if((newTexture = SDL_CreateTextureFromSurface(renderer, surface)) == nullptr) {
		printf("Could not create texture from surface. SDL error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return false;
	}

	if(texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
        size = {0, 0};
    }

    size.w = surface->w;
    size.h = surface->h;
    SDL_FreeSurface(surface);
    texture = newTexture;
    name = "text";
	return true;
}

Size Texture::getSize() { return size; }

std::string Texture::getName() { return name; }

void Texture::setName(std::string newName) { name = newName; }
