//
//  Tile.hpp
//  SDL2_potato3
//
//  Created by pow on 2017-09-12.
//  Copyright © 2017 potato. All rights reserved.
//

#ifndef TILE_HPP
#define TILE_HPP
#include <SDL2/SDL.h>
#include <unordered_set>
#include "Texture.hpp"

class Tile {
public:
    enum TileType {
        EMPTY_SPACE = 0, //0
        FLOOR, //1
        CEILING, //2
        CORNER_TOPL, //3
        LEFT_WALL, //4
        CORNER_BOTL, //5
        CORNER_TOPR, //6
        RIGHT_WALL,  //7
        CORNER_BOTR, //8
        BLOCK, //9
        KILL_BLOCK,
        ERROR_TYPE //10
    };

    static const int TILE_WIDTH, TILE_HEIGHT;

    static const std::unordered_set<TileType> topWalls, bottomWalls, leftWalls, rightWalls;

    Tile(SDL_Point point = {0, 0}, TileType _tileType = EMPTY_SPACE);
    void render(SDL_Renderer *renderer, Texture *texture);
    bool testCollision(SDL_Rect rect);
    SDL_Rect getBounds();
    TileType getType();
    void setLocation(SDL_Point point);
    void setType(TileType newType);

private:
    SDL_Rect bounds, clip;
    TileType tileType;
};

#endif
