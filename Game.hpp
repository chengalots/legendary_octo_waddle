//
//  Game.hpp
//  SDL2_potato4
//
//  Created by pow on 2017-09-20.
//  Copyright © 2017 potato. All rights reserved.
//

#ifndef GAME_HPP
#define GAME_HPP
#include <SDL2/SDL.h>
#include <fstream>
#include <string>
#include <vector>
//#include "Size.hpp"
//#include "Timer.hpp"
//#include "Animation.hpp"
#include "Player.hpp" //Character.hpp already includes previous 3 .hpp files
#include "Enemy.hpp"
#include "Chunk.hpp"
//#include "SkillTray.hpp"

class Game {
public:
    Game(SDL_Renderer * _renderer, Size windowSize);
    ~Game();
        //everything that would have been in the main loop in one function
    void tick();
        //run condition for the main loop, set to false if user quits or if
        //  something goes wrong
    bool run;

private:
    void processKeyPresses();
    void render();
    void updateCollisionGrid();

        //check if a hitbox can continue to move in specified direction
    bool canMove(Direction direction, Character * character, bool isPlayer);
    bool testCollisionWithPlayer(Direction direction, Character * character);
        //bounding box collision detection
    bool testCollision(SDL_Rect rect1, SDL_Rect rect2);
        //used for moving the level and everything in it (except the player)
    void translateChunks(int dx, int dy);
        //find the tile coordinates of a character
    SDL_Point chunkLocation(SDL_Point point);
    SDL_Point absoluteTileLocation(SDL_Point point);

        //maximum falling speed
    static const int MAX_FALL_VEL;
        //acceleration in the y direction, used for gravity
    static const int Y_ACCEL;

        //character that the user controls
    Player * player;
        //a target dummy, change to std::vector of enemies later
    Enemy * enemy;
    std::vector<Enemy> enemies;

        //2D std::vector of chunks, the level "background" or "terrain"
    std::vector< std::vector<Chunk> > chunks;
        //origin of the level relative to the camera
    SDL_Point origin;
        //reference texture for tiles
    Texture tileTexture;
        //used to determine what use can and cannot see
    SDL_Rect camera;

        //constantTimer is always running, loopTimer calculates time between
        //  each call of tick()
    Timer constantTimer, loopTimer;

        //pointer to renderer created in main()
    SDL_Renderer * renderer;
        //used for polling events
    SDL_Event event;
        //used for holding key states and processing key presses
    const Uint8 * currentKeyStates;

    //bool canMoveUp, canMoveDown, canMoveLeft, canMoveRight;
};

#endif
