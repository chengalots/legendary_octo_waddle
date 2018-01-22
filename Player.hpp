//
//  Player.hpp
//  SDL2_potato3
//
//  Created by pow on 2017-10-15.
//  Copyright © 2017 potato. All rights reserved.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Character.hpp"

class Player : public Character {
public:
        //all available skills
    enum Skills {
        ROLL = 0,
        HEARTTHRUST, //1
        OHS, //2
        REND, //3
        EVISC, //4
        MEASURED_SLICE, //5
        STAGGERING_SLASH, //6
        AAAA, //7
        SPIN_MOVE, //8
        ROLL_ATTACK_H, //9
        END //absolute end of animations std::vector
    };

    enum AttackType {
        LIGHT,
        HEAVY,
        DODGE
    };

    static const int width, height;
    static const int SKILLSET_SIZE;
    static const int NUM_OF_TEXTURES;

    Player(SDL_Renderer * renderer, Timer * newTimer, SDL_Point spawnPoint);
    void update();

    void setXVelocity(int dx);
    void move();
    void useSkill(AttackType t, Direction d);

private:
    //int nextSkill;
};

#endif
