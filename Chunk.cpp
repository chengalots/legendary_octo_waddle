//
//  Chunk.cpp
//  SDL2_potato3
//
//  Created by pow on 2017-09-13.
//  Copyright © 2017 potato. All rights reserved.
//

#include "Chunk.hpp"

const int Chunk::CHUNK_WIDTH = 10;
const int Chunk::CHUNK_HEIGHT = 8;

Chunk::Chunk(SDL_Point point) {

    tiles = std::vector<std::vector<Tile>> (CHUNK_WIDTH, std::vector<Tile> (CHUNK_HEIGHT, Tile()));
    bounds = {point.x, point.y, CHUNK_WIDTH * Tile::TILE_WIDTH, CHUNK_HEIGHT * Tile::TILE_HEIGHT};

    for(int i = 0; i < CHUNK_WIDTH; i++) {
        for(int j = 0; j < CHUNK_HEIGHT; j ++) {
            tiles.at(i).at(j).setLocation({bounds.x + Tile::TILE_WIDTH * i, bounds.y + Tile::TILE_HEIGHT * j});
        }
    }
}

void Chunk::render(SDL_Renderer *renderer, const SDL_Rect &camera, Texture *texture) {

    for(int i = 0; i < CHUNK_WIDTH; i++) {
        for(int j = 0; j < CHUNK_HEIGHT; j ++) {
            if(tiles.at(i).at(j).testCollision(camera)) {
                tiles.at(i).at(j).render(renderer, texture);
            }
        }
    }
}

void Chunk::translate(int dx, int dy) {

    bounds.x += dx;
    bounds.y += dy;
    syncTiles();
}

void Chunk::syncTiles() {

    for(int i = 0; i < CHUNK_WIDTH; i++) {
        for(int j = 0; j < CHUNK_HEIGHT; j ++) {
            tiles.at(i).at(j).setLocation({bounds.x + (i * Tile::TILE_WIDTH), bounds.y +
                (j * Tile::TILE_HEIGHT)});
        }
    }
}

SDL_Rect Chunk::getBounds() { return bounds; }

Tile Chunk::getTile(int i, int j) { return tiles.at(i).at(j); }

Tile::TileType Chunk::getTileType(int i, int j) {

    if (i < 0 || i >= CHUNK_WIDTH) { return Tile::ERROR_TYPE; }
    if (j < 0 || j >= CHUNK_HEIGHT) { return Tile::ERROR_TYPE; }
    return tiles.at(i).at(j).getType();
}

void Chunk::setLocation(SDL_Point point) {

    bounds.x = point.x;
    bounds.y = point.y;
    syncTiles();
}

void Chunk::setTileType(int i, int j, Tile::TileType tileType) {

    tiles.at(i).at(j).setType(tileType);
}

void Chunk::setTileTypes(std::vector<std::vector<Tile::TileType> > tileTypeArray) {

    for(int i = 0; i < CHUNK_WIDTH; i++) {
        for(int j = 0; j < CHUNK_HEIGHT; j++) {
            tiles.at(i).at(j).setType(tileTypeArray.at(i).at(j));
        }
    }
}

void Chunk::addChar(Character * c) { charsInChunk.insert(c); }

void Chunk::removeChar(Character * c) { charsInChunk.erase(c); }

void Chunk::removeAllChars() { charsInChunk.clear(); }

bool Chunk::constainsChar(Character * c) { return charsInChunk.count(c) != 0; }

const std::unordered_set<Character *> Chunk::getCharsInChunk() { return charsInChunk; }
