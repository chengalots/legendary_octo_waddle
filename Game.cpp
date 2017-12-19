//
//  Game.cpp
//  SDL2_potato4
//
//  Created by pow on 17-09-20.
//  Copyright © 2017 potato. All rights reserved.
//

#include "Game.hpp"

const int Game::MAX_FALL_VEL = 900;
const int Game::Y_ACCEL = 1600;

Game::Game(SDL_Renderer * _renderer, Size windowSize) {

    run = true;

    if((renderer = _renderer) == nullptr) {
        run = false;
        printf("Game must be given a non-nullptr reference to a valid SDL_Renderer\n");
    }

    camera = {0, 0, windowSize.w, windowSize.h};

    player = new Player(renderer, &constantTimer,
        {(camera.w - Character::CHAR_W) / 2, (camera.h - Character::CHAR_H) / 2});
    player->setPreviousChunk(chunkLocation(player->location()));

    enemies.push_back(new Enemy(renderer, &constantTimer, {900, 450, 60, 90}, {Character::CHAR_W, Character::CHAR_H}));

    for(unsigned int i = 0; i < enemies.size(); i++) {
        enemies.at(i)->setPreviousChunk(chunkLocation(enemies.at(i)->location()));
    }

    origin = {0,0};

        //get a reference to keyboard states
    currentKeyStates = SDL_GetKeyboardState(nullptr);
        //load texture for the tiles
    if(!tileTexture.loadFromFile(renderer, "images/tiles2.png")) run = false;

        //read the map info from file and set the tile types accordingly
    std::ifstream fileIn("maps/map1.txt");
        //check if the file openned
    if(fileIn) {
        //for storing the dimensions of the chunks std::vector
        int x = 0, y = 0;
            //attempt to read the dimensions from file, quit if it fails
        if(!(fileIn >> x)) run = false;
        if(!(fileIn >> y)) run = false;

        if(run){
                //chunks std::vector is currently empty, fill the 2D std::vector with Chunk objects
            for(int i = 0; i < x; i++) {
                chunks.push_back(std::vector<Chunk>());
                for(int j = 0; j < y; j++) {
                    chunks.at(i).push_back(Chunk (
                        {
                            i * Chunk::CHUNK_WIDTH * Tile::TILE_WIDTH + origin.x,
                            j * Chunk::CHUNK_HEIGHT * Tile::TILE_HEIGHT + origin.y
                        }
                    ));
                }
            }
                //stores the integers read from the file
            int tileType;
                //iterate through the tiles in each chunk and set their types by casting tileType as a
                //    Tile::TileType
            for(int j = 0; j < y * Chunk::CHUNK_HEIGHT; j++) {
                for(int i = 0; i < x * Chunk::CHUNK_WIDTH; i++) {
                    if(fileIn >> tileType) {
                            //the map is formatted so that a single line contains the tile type for multiple
                            //    different chunks
                        chunks.at(i / Chunk::CHUNK_WIDTH).at(
                            j /  Chunk::CHUNK_HEIGHT).setTileType(i % Chunk::CHUNK_WIDTH,
                            j % Chunk::CHUNK_HEIGHT, (Tile::TileType)tileType);
                    }
                }
            }
        }
    }
    else {
        run = false;
    }
    fileIn.close();

    translateChunks(0, -150);
    updateCollisionGrid();
        //start constant timer
    constantTimer.start();
}

Game::~Game() {
    renderer = nullptr;
    currentKeyStates = nullptr;
    delete player;
    for(Character *enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
}

void Game::tick() {

    while(SDL_PollEvent(&event)) {

        switch(event.type) {
            case SDL_QUIT: //user closes the window, exit the program
                run = false;
                break;
            case SDL_KEYDOWN: //check for key presses, for key press specific things (e.g. limiting key presses,
                              //    calculate time between key presses, etc)
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        //player jumping and double jumping
                        if(player->canMoveUp && player->getJumpCounter() < player->jumpCap) {
                            player->setYVelocity(Character::JUMP_VEL);
                            player->jump();
                        }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP: //check for movement key releases to stop char movement
                switch(event.key.keysym.sym) {
                    case SDLK_a:
                        player->setXVelocity(0);
                        break;
                    case SDLK_d:
                        player->setXVelocity(0);
                        break;
                    case SDLK_LEFT:
                        if(!enemies.empty()) enemies.at(0)->setXVelocity(0); //these shouldn't be here but like...
                        break;
                    case SDLK_RIGHT:
                        if(!enemies.empty()) enemies.at(0)->setXVelocity(0); //i need to control at least 1 enemy
                        break;
                    case SDLK_UP:
                        if(!enemies.empty()) enemies.at(0)->setYVelocity(0); // so i can test stuff like collision
                        break;
                    case SDLK_DOWN:
                        if(!enemies.empty()) enemies.at(0)->setYVelocity(0); // so yeah...
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT) {
                    Direction d;// = NONE;
                    if(currentKeyStates[SDL_SCANCODE_A] && !currentKeyStates[SDL_SCANCODE_D]) d = LEFT;
                    else if(currentKeyStates[SDL_SCANCODE_D] && !currentKeyStates[SDL_SCANCODE_A]) d = RIGHT;
                    else d = NONE;
                    player->useSkill(Player::LIGHT, d);
                }
                else if(event.button.button == SDL_BUTTON_RIGHT) {
                    Direction d;
                    if(currentKeyStates[SDL_SCANCODE_A] && !currentKeyStates[SDL_SCANCODE_D]) d = LEFT;
                    else if(currentKeyStates[SDL_SCANCODE_D] && !currentKeyStates[SDL_SCANCODE_A]) d = RIGHT;
                    else d = NONE;
                    player->useSkill(Player::HEAVY, d);
                }
                break;
            default:
                break;
        }
    }

    processKeyPresses();

    //-----------------------------
    ///    Reset jumpCounter    ///
    //-----------------------------
        //reset the player's jump counter if they're on the ground
    if(!player->canMoveDown) player->resetJumpCounter();

    //-----------------------------
    ///     Collision Grid      ///
    //-----------------------------
    updateCollisionGrid();

    //-----------------------------
    ///   Clear StatusEffects   ///
    //-----------------------------
    for(Character *enemy : enemies) {
        for(auto &effect : enemy->getDebuffEffectsReceived()) {
            if((int)constantTimer.getTicks() -  effect.second.startDurationTicks >= effect.second.duration) {
                std::cout << "a" << std::endl;
                enemy->removeDebuffEffect(effect.second.debuff);
            }
        }
        for(auto &effect : enemy->getBuffEffectsReceived()) {
            if((int)constantTimer.getTicks() -  effect.second.startDurationTicks >= effect.second.duration) {
                enemy->removeBuffEffect(effect.second.buff);
            }
        }
    }

    for(auto &effect : player->getDebuffEffectsReceived()) {
        if((int)constantTimer.getTicks() -  effect.second.startDurationTicks >= effect.second.duration) {
            player->removeDebuffEffect(effect.second.debuff);
        }
    }
    for(auto &effect : player->getBuffEffectsReceived()) {
        if((int)constantTimer.getTicks() -  effect.second.startDurationTicks >= effect.second.duration) {
            player->removeBuffEffect(effect.second.buff);
        }
    }

    //-----------------------------
    ///     Attack hitboxes     ///
    //-----------------------------
        //check if there's an attack and if it hits the target dummy
    Attack skill = player->getSkill();

    /*if(!skill.damageFrames.empty() && skill.damageFrames.at(0).damage > 0) {
        SDL_Rect bounds = enemy->getBounds();
        if(testCollision(skill.damageFrames.at(0).hitbox, bounds)) {
            for(auto &effect : skill.buffEffectsApplied) {
                if(effect.buff == StatusEffect::BLOODLUST){
                    effect.startDurationTicks = constantTimer.getTicks();
                    player->receiveStatusEffect(effect);
                    //printf("stack++\n");
                }
            }
            //printf("%d damage!\n", skill.damageFrames.at(0).damage);
            for(auto &effect : skill.debuffEffectsApplied) {
                if(effect.debuff != StatusEffect::NO_DEBUFF) {
                    effect.startDurationTicks = constantTimer.getTicks();
                    enemy->receiveStatusEffect(effect);
                }
            }
        }
    }*/

    //-----------------------------
    ///      Apply gravity      ///
    //-----------------------------
    double timeStep = loopTimer.getTicks() / 1000.0;
        //if the player is not on the ground
    if(player->canMoveDown && player->getVelocity().dy() > -MAX_FALL_VEL) {
            //apply gravity to velocity
        player->setYVelocity(player->getVelocity().dy() - (Y_ACCEL * timeStep));
            //cap falling speed
        if(player->getVelocity().dy() < -MAX_FALL_VEL) player->setYVelocity(-MAX_FALL_VEL);
            //if character walks off an edge, count it as a jump
        if(player->getJumpCounter() == 0) player->jump();
    }

        //apply gravity for target dummy
    for(Character *enemy : enemies) {
        if(canMove(DOWN, enemy, false)) {
            enemy->setYVelocity(enemy->getVelocity().dy() - (Y_ACCEL * timeStep));

            if(enemy->getVelocity().dy() < -MAX_FALL_VEL) {
                enemy->setYVelocity(-MAX_FALL_VEL);
            }
        }
        else if(enemy->getVelocity().dy() < 0) {
            enemy->setYVelocity(0);
        }
    }

    //-----------------------------
    ///    Translate chunks     ///
    //-----------------------------
        //move the background -> scrolling background so it looks like the camera is centered on the player
    if(std::abs(player->getVelocity().dx()) >= 0.05) {
        translateChunks(player->getVelocity().dx() * timeStep, 0);
    }
    if(std::abs(player->getVelocity().dy()) >= 0.05) {
        translateChunks(0, player->getVelocity().dy() * timeStep);
    }

        //move the target dummy
    for(Enemy *enemy : enemies) {
        if(std::abs(enemy->getVelocity().dx()) >= 1) {
            if(std::abs(enemy->getVelocity().dx() + player->getVelocity().dx()) < 1) {
                enemy->setXVelocity(enemy->getVelocity().dx() + player->getVelocity().dx(), true);
            }
            else enemy->setXVelocity(enemy->getVelocity().dx() + player->getVelocity().dx());
        }
        enemy->move();
    }

        //do collision check for player
    player->canMoveUp = canMove(UP, player, true);
    player->canMoveDown = canMove(DOWN, player, true);
    player->canMoveLeft = canMove(LEFT, player, true);
    player->canMoveRight = canMove(RIGHT, player, true);

        //restart the frame timer after everything has been moved
    loopTimer.start();

    for(Enemy *enemy : enemies) {
        enemy->update();
    }
    player->update();

    render();
}

void Game::processKeyPresses() {
    //-----------------------------
    ///   Process key presses   ///
    //-----------------------------
        //determine which keys are pressed and move the background accordingly, no else if statements allows
        //    simultaneous key presses
            //---------------------
            ///      A key      ///
        //if A key is pressed and D key is not pressed and the player can move to the left, set the velocity to
        //    move to the left
    if(currentKeyStates[SDL_SCANCODE_A] && !currentKeyStates[SDL_SCANCODE_D] && player->canMoveLeft) {

        player->setXVelocity(Character::MAX_CHAR_VEL);
        player->setDirection(LEFT);
    }
        //if character is trying to move LEFT but that direction is blocked, set the velocity to 0 to prevent
        //    jittering
    if(player->getVelocity().dx() > 0 && !player->canMoveLeft) {
        player->setXVelocity(0);
        if(player->canMoveDown) {
            if(player->getJumpCounter() == player->jumpCap) {
                player->setJumpCounter(player->jumpCap - 1);
            }
        }
    }
            //---------------------
            ///      D key      ///

        //same idea as above but with D key pressed and A key not pressed
    if(currentKeyStates[SDL_SCANCODE_D] && !currentKeyStates[SDL_SCANCODE_A] && player->canMoveRight) {

        player->setXVelocity(-Character::MAX_CHAR_VEL);
        player->setDirection(RIGHT);
    }
    if(player->getVelocity().dx() < 0 && !player->canMoveRight) {
        player->setXVelocity(0);
        if(player->canMoveDown) {
            if(player->getJumpCounter() == player->jumpCap) {
                player->setJumpCounter(player->jumpCap - 1);
            }
        }
    }

    if(player->getVelocity().dy() > 0 && !player->canMoveUp) {
        player->setYVelocity(0);
    }

    if(player->getVelocity().dy() < 0 && !player->canMoveDown) {
        player->setYVelocity(0);
    }
            //---------------------
            ///      1 key      ///

    if(currentKeyStates[SDL_SCANCODE_1]) {
        Direction d;
        if(currentKeyStates[SDL_SCANCODE_A] && !currentKeyStates[SDL_SCANCODE_D]) d = LEFT;
        else if(currentKeyStates[SDL_SCANCODE_D] && !currentKeyStates[SDL_SCANCODE_A]) d = RIGHT;
        else d = NONE;
        player->useSkill(Player::LIGHT, d);
    }
            //---------------------
            ///      2 key      ///

    else if(currentKeyStates[SDL_SCANCODE_2]) {
        Direction d;
        if(currentKeyStates[SDL_SCANCODE_A] && !currentKeyStates[SDL_SCANCODE_D]) d = LEFT;
        else if(currentKeyStates[SDL_SCANCODE_D] && !currentKeyStates[SDL_SCANCODE_A]) d = RIGHT;
        else d = NONE;
        player->useSkill(Player::HEAVY, d);
    }
            //---------------------
            ///      C key      ///
    if(currentKeyStates[SDL_SCANCODE_C]) {
        Direction d;
        if(currentKeyStates[SDL_SCANCODE_A] && !currentKeyStates[SDL_SCANCODE_D]) d = LEFT;
        else if(currentKeyStates[SDL_SCANCODE_D] && !currentKeyStates[SDL_SCANCODE_A]) d = RIGHT;
        else d = NONE;
        player->useSkill(Player::DODGE, d);
    }

            //---------------------
            ///TargetDummy Ctrls///
    if(currentKeyStates[SDL_SCANCODE_UP] && canMove(UP, enemies.at(0), false)) {
        enemies.at(0)->setYVelocity(Character::MAX_CHAR_VEL);
    }
    if(enemies.at(0)->getVelocity().dy() > 0 && !canMove(UP, enemies.at(0), false)) {
        enemies.at(0)->setYVelocity(0);
    }

    if(currentKeyStates[SDL_SCANCODE_DOWN] && canMove(DOWN, enemies.at(0), false)) {
        enemies.at(0)->setYVelocity(-Character::MAX_CHAR_VEL);
    }
    if(enemies.at(0)->getVelocity().dy() < 0 && !canMove(DOWN, enemies.at(0), false)) {
        enemies.at(0)->setYVelocity(0);
    }

    if(currentKeyStates[SDL_SCANCODE_LEFT] && canMove(LEFT, enemies.at(0), false)) {
        enemies.at(0)->setXVelocity(-Character::Character::MAX_CHAR_VEL);
        enemies.at(0)->setDirection(LEFT);
    }
    if(enemies.at(0)->getVelocity().dx() < 0 && !canMove(LEFT, enemies.at(0), false)) {
        enemies.at(0)->setXVelocity(0);
    }

    if(currentKeyStates[SDL_SCANCODE_RIGHT] && canMove(RIGHT, enemies.at(0), false)) {
        enemies.at(0)->setXVelocity(Character::MAX_CHAR_VEL);
        enemies.at(0)->setDirection(RIGHT);
    }
    if(enemies.at(0)->getVelocity().dx() > 0 && !canMove(RIGHT, enemies.at(0), false)) {
        enemies.at(0)->setXVelocity(0);
    }
}

void Game::render() {
    //-----------------------------
    ///        Rendering        ///
    //-----------------------------
    SDL_SetRenderDrawColor(renderer, 0x24, 0x24, 0x24, 0xFF);
    SDL_RenderClear(renderer);
            //---------------------
            ///   Background    ///
    SDL_Point startChunk = chunkLocation({0, 0});
    int xLimit = ceil((double)camera.w / (double)(Chunk::CHUNK_WIDTH * Tile::TILE_WIDTH));
    int yLimit = ceil((double)camera.h / (double)(Chunk::CHUNK_HEIGHT * Tile::TILE_HEIGHT));
        //check the chunks around the camera that might be visible
    for(unsigned int i = startChunk.x; i <= (unsigned int)(startChunk.x + xLimit) && i < chunks.size(); i++) {
        for(unsigned int j = startChunk.y; j <= (unsigned int)(startChunk.y + yLimit) && j < chunks.at(i).size(); j++) {
                //check if the chunk is actually visible
            if(testCollision(camera, chunks.at(i).at(j).getBounds())) {
                chunks.at(i).at(j).render(renderer, camera, &tileTexture);
            }
        }
    }
    for(unsigned int i = startChunk.x; i <= (unsigned int)(startChunk.x + xLimit) && i < chunks.size(); i++) {
        for(unsigned int j = startChunk.y; j <= (unsigned int)(startChunk.y + yLimit) && j < chunks.at(i).size(); j++) {
                //check if the chunk is actually visible
            if(testCollision(camera, chunks.at(i).at(j).getBounds())) {
                    //render the enemies in the visible chunks
                for(Character *character : chunks.at(i).at(j).getCharsInChunk()) {
                    if(character != player) ((Enemy *)character)->render(renderer);
                }
            }
        }
    }

    //SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0, 0xFF);
            //---------------------
            ///     Player      ///
    player->render(renderer);

        //draw everything
    SDL_RenderPresent(renderer);
}

void Game::updateCollisionGrid() {
    for(unsigned int i = 0; i < chunks.size(); i++) {
        for(unsigned int j = 0; j < chunks.at(i).size(); j++) {
            chunks.at(i).at(j).removeAllChars();
        }
    }

    SDL_Point startChunk = chunkLocation(player->location());
    for(unsigned int i = startChunk.x; i < (unsigned int)(startChunk.x + 2) && i < chunks.size(); i++) {
        for(unsigned int j = startChunk.y; j < (unsigned int)(startChunk.y + 2) && j < chunks.at(i).size(); j++) {
            if(testCollision(player->getBounds(), chunks.at(i).at(j).getBounds())) {
                chunks.at(i).at(j).addChar(player);
            }
        }
    }

    for(unsigned int i = 0; i < enemies.size(); i++) {
        SDL_Point startChunk = chunkLocation(enemies.at(i)->location());
        for(unsigned int j = startChunk.x; j < (unsigned int)(startChunk.x + 2) && j < chunks.size(); j++) {
            for(unsigned int k = startChunk.y; k < (unsigned int)(startChunk.y + 2) && k < chunks.at(i).size(); k++) {
                if(testCollision(enemies.at(i)->getBounds(), chunks.at(j).at(k).getBounds())) {
                    chunks.at(j).at(k).addChar(enemies.at(i));
                }
            }
        }
    }
}

bool Game:: canMove(Direction direction, Character * character, bool isPlayer) {
    double zipCorrection = 0.25;
    SDL_Rect hitbox = character->getBounds();
    SDL_Point startTile = absoluteTileLocation({hitbox.x, hitbox.y});
    Chunk * previousChunk = nullptr;

    for(unsigned int i = startTile.x; i < (unsigned int)(startTile.x + 2) && (i / Chunk::CHUNK_WIDTH) < chunks.size(); i++) {
        for(unsigned int j = startTile.y; j < (unsigned int)(startTile.y + 2)  &&
            (j / Chunk::CHUNK_HEIGHT) < chunks.at(i / Chunk::CHUNK_WIDTH).size(); j++) {

            Tile tile = chunks.at(i / Chunk::CHUNK_WIDTH).at(j / Chunk::CHUNK_HEIGHT).getTile(i % Chunk::CHUNK_WIDTH, j % Chunk::CHUNK_HEIGHT);

            int dx = 0, dy = 0;

            if(testCollision(hitbox, tile.getBounds())) {
                bool collisionFound = false;
                switch(direction) {
                    case LEFT:
                        if(Tile::rightWalls.count(tile.getType()) == 0) break;
                        if(tile.getBounds().x + Tile::TILE_WIDTH
                            - (Tile::TILE_WIDTH * zipCorrection) > hitbox.x) break;
                        if(tile.getBounds().y + 1 >= hitbox.y + hitbox.h ||
                            tile.getBounds(). y + tile.getBounds().w - 1 <= hitbox.y) break;

                        dx = hitbox.x - (tile.getBounds().x + Tile::TILE_WIDTH - 1);
                        collisionFound = true;
                        break;
                    case RIGHT:
                        if(Tile::leftWalls.count(tile.getType()) == 0) break;
                        if(tile.getBounds().x + (Tile::TILE_WIDTH * zipCorrection)
                            < hitbox.x + hitbox.w) break;
                        if(tile.getBounds().y + 1 >= hitbox.y + hitbox.h ||
                            tile.getBounds(). y + tile.getBounds().w - 1 <= hitbox.y) break;

                        dx = (hitbox.x + hitbox.w) - tile.getBounds().x;
                        collisionFound = true;
                        break;
                    case UP:
                        if(Tile::bottomWalls.count(tile.getType()) == 0) break;
                        if(tile.getBounds().y + Tile::TILE_HEIGHT
                            - (Tile::TILE_HEIGHT * zipCorrection) > hitbox.y) break;
                        if(tile.getBounds().x + tile.getBounds().w - 1 <= hitbox.x ||
                            tile.getBounds().x + 1 >= hitbox.x + hitbox.w) break;

                        dy = hitbox.y - (tile.getBounds().y + Tile::TILE_HEIGHT);
                        collisionFound = true;
                        break;
                    case DOWN:
                        if(Tile::topWalls.count(tile.getType()) == 0) break;
                        if(tile.getBounds().y + (Tile::TILE_HEIGHT * zipCorrection)
                            < hitbox.y + hitbox.h) break;
                        if(tile.getBounds().x + tile.getBounds().w - 1 <= hitbox.x ||
                            tile.getBounds().x + 1 >= hitbox.x + hitbox.w) break;

                        dy = (hitbox.y + hitbox.h) - tile.getBounds().y;
                        collisionFound = true;
                        break;
                    default:
                            //None? I mean you're allowed to have no direction,
                            //    but like... then why are you even using this???
                        break;
                }
                if(collisionFound) {
                    if(isPlayer) translateChunks(dx, dy);
                    else {
                        character->translate(-dx, -dy);
                    }
                    return false;
                }
            }

            if(character->hasUnitCollision() && previousChunk != &chunks.at(i / Chunk::CHUNK_WIDTH).at(j / Chunk::CHUNK_HEIGHT)) {
                for(Character * character2 : chunks.at(i / Chunk::CHUNK_WIDTH).at(j / Chunk::CHUNK_HEIGHT).getCharsInChunk()) {
                    if(character != character2 && character2->hasUnitCollision() && testCollision(hitbox, character2->getBounds())) {
                        switch(direction) {
                            case LEFT:
                                if(hitbox.x > character2->getBounds().x && std::abs(hitbox.y - character2->getBounds().y) < character2->getBounds().h - 20) {
                                    return false;
                                }
                                break;
                            case RIGHT:
                                if(hitbox.x < character2->getBounds().x && std::abs(hitbox.y - character2->getBounds().y) < character2->getBounds().h - 20) {
                                    return false;
                                }
                                break;
                            case UP:
                                if(hitbox.y > character2->getBounds().y && std::abs(hitbox.x - character2->getBounds().x) < character2->getBounds().w - 20) {
                                    return false;
                                }
                                break;
                            case DOWN:
                                if(hitbox.y < character2->getBounds().y && std::abs(hitbox.x - character2->getBounds().x) < character2->getBounds().w - 20) {
                                    return false;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }
                previousChunk = &chunks.at(i / Chunk::CHUNK_WIDTH).at(j / Chunk::CHUNK_HEIGHT);
            }
        }
    }
    return true;
}

bool Game::testCollision(SDL_Rect rect1, SDL_Rect rect2) {
    if(rect1.x > rect2.x + rect2.w) return false;
    if(rect1.x + rect1.w < rect2.x) return false;
    if(rect1.y > rect2.y + rect2.h) return false;
    if(rect1.y + rect1.h < rect2.y) return false;
    return true;
}

void Game::translateChunks(int dx, int dy) {
        //move the background
    for(unsigned int i = 0; i < chunks.size(); i++) {
        for(unsigned int j = 0; j < chunks.at(i).size(); j ++) {
            chunks.at(i).at(j).translate(dx, dy);
        }
    }
        //update the location of the map's origin
    origin.x += dx;
    origin.y += dy;

    for(Character *enemy : enemies) {
        if(std::abs(enemy->getVelocity().dx()) < 1) {
            enemy->translate(dx, 0);
        }
        enemy->translate(0, dy);
    }
}

SDL_Point Game::chunkLocation(SDL_Point point) {
    int dx = point.x - origin.x;
    int dy = point.y - origin.y;
    return {dx / (Chunk::CHUNK_WIDTH * Tile::TILE_WIDTH), dy / (Chunk::CHUNK_HEIGHT * Tile::TILE_HEIGHT)};
}

SDL_Point Game::absoluteTileLocation(SDL_Point point) {
    int dx = point.x - origin.x;
    int dy = point.y - origin.y;
    return {dx / Tile::TILE_WIDTH, dy / Tile::TILE_HEIGHT};
}
