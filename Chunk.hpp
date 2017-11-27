//
//  Chunk.hpp
//  SDL2_potato3
//
//  Created by pow on 2017-09-13.
//  Copyright © 2017 potato. All rights reserved.
//

#ifndef CHUNK_HPP
#define CHUNK_HPP
#include <vector>
#include <unordered_set>
#include "Tile.hpp"
#include "Character.hpp"

class Chunk {
public:
    static const int CHUNK_WIDTH, CHUNK_HEIGHT;

    Chunk(SDL_Point point);
    void render(SDL_Renderer *renderer, const SDL_Rect &camera,
                Texture *texture);
    void translate(int dx, int dy);
    void syncTiles();
    SDL_Rect getBounds();
    Tile getTile(int i, int j);
    Tile::TileType getTileType(int i, int j);
    void setLocation(SDL_Point point);
    void setTileType(int i, int j, Tile::TileType tileType);
    void setTileTypes(std::vector<std::vector<Tile::TileType>> tileTypeArray);

    //std::unordered_set<Character *> const * _charsInChunk();
    void addChar(Character * c);
    void removeChar(int i);
    void removeChar(Character * c);
    void removeAllChars();
    bool constainsChar(Character * c);
    const std::unordered_set<Character *> getCharsInChunk();

private:
    std::vector<std::vector<Tile>> tiles;
    std::unordered_set<Character *> charsInChunk;

    SDL_Rect bounds;
};

#endif
