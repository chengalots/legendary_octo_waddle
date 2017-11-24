//
//  Tile.cpp
//  SDL2_potato3
//
//  Created by pow on 2017-09-12.
//  Copyright © 2017 potato. All rights reserved.
//

#include "Tile.hpp"

const int Tile::TILE_WIDTH = 100;
const int Tile::TILE_HEIGHT = 100;

const std::unordered_set<Tile::TileType> Tile::topWalls = {BLOCK, FLOOR, CORNER_TOPL, CORNER_TOPR};
const std::unordered_set<Tile::TileType> Tile::bottomWalls = {BLOCK, CEILING, CORNER_BOTL, CORNER_BOTR};
const std::unordered_set<Tile::TileType> Tile::leftWalls = {BLOCK, LEFT_WALL, CORNER_TOPL, CORNER_BOTL};
const std::unordered_set<Tile::TileType> Tile::rightWalls = {BLOCK, RIGHT_WALL, CORNER_TOPR, CORNER_BOTR};

Tile::Tile(SDL_Point point, TileType _tileType) {

    bounds = {point.x, point.y, TILE_WIDTH, TILE_HEIGHT};
    clip.w = TILE_WIDTH;
    clip.h = TILE_HEIGHT;

    setType(_tileType);
}

void Tile::render(SDL_Renderer *renderer, Texture *texture) {

    texture->render(renderer, bounds.x, bounds.y, &clip);
}

bool Tile::testCollision(SDL_Rect rect) {

    if(rect.x >= bounds.x + bounds.w) return false;
    if(rect.x + rect.w <= bounds.x) return false;
    if(rect.y >= bounds.y + bounds.h) return false;
    if(rect.y + rect.h <= bounds.y) return false;

    return true;
}

SDL_Rect Tile::getBounds() { return bounds; }

Tile::TileType Tile::getType() { return tileType; }

void Tile::setLocation(SDL_Point point) {

    bounds.x = point.x;
    bounds.y = point.y;
}

void Tile::setType(Tile::TileType newType) {

    tileType = newType;

    switch(tileType) {
        case EMPTY_SPACE:
            clip.x = 100;
            clip.y = 100;
            break;
        case FLOOR:
            clip.x = 100;
            clip.y = 0;
            break;
        case CEILING:
            clip.x = 100;
            clip.y = 200;
            break;
        case LEFT_WALL:
            clip.x = 0;
            clip.y = 100;
            break;
        case RIGHT_WALL:
            clip.x = 200;
            clip.y = 100;
            break;
        case CORNER_TOPL:
            clip.x = 0;
            clip.y = 0;
            break;
        case CORNER_TOPR:
            clip.x = 200;
            clip.y = 0;
            break;
        case CORNER_BOTL:
            clip.x = 0;
            clip.y = 200;
            break;
        case CORNER_BOTR:
            clip.x = 200;
            clip.y = 200;
            break;
        case BLOCK:
            clip.x = 300;
            clip.y = 0;
            break;
        case ERROR_TYPE:
            clip.x = 300;
            clip.y = 100;
            break;
        default:
            break;
    }
}
