//
//  Character.hpp
//  SDL2_potato3
//
//  Created by pow on 2017-09-10.
//  Copyright © 2017 potato. All rights reserved.
//

#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include <unordered_map>
#include "Animation.hpp"
#include "Attack.hpp"
#include "Loader.hpp"
#include "PhysicsBody.hpp"

class Character {
public:
        //all available animations and their indices in animations std::vector
    enum AnimationIndex {
        IDLE = 0,
        RUNNING, //1
        JUMP, //2
        DOUBLE_JUMP, //3
        FALLING, //4
        SKILLS_START //end of regular animations, start of skill animations
    };

        //character dimensions
    static const int CHAR_W, CHAR_H;
        //maximum number of jumps before touching the ground
    //static const int JUMP_CAP;
        //initial upward velocity of a jump
    static const int JUMP_VEL;
        //maximum running speed from user input
    static const int MAX_CHAR_VEL;

    static const int CHAIN_TIME_LIMIT;

        //holds the textures for animations
    std::vector<Texture> textures;
        //contains all available animations in AnimationIndex and Skills enums
    std::vector<Animation> animations;
        //contains all available skills in Skills enum
    std::vector<Attack> skills;

    Timer chainTimer;

    int jumpCap;
        //index of current skill in use
    int skillIndex;

    bool canMoveUp, canMoveDown, canMoveLeft, canMoveRight;

    Character(Timer * newTimer, SDL_Point spawnPoint, Size size);
    ~Character();
    void render(SDL_Renderer * renderer);

        //use a skill listed in the Skills enum
    void useSkill(int skill, Direction d);
        //return hitbox of a skill or nullptr if no skill is in use
    Attack getSkill();

    void setTimer(Timer * newTimer);
    SDL_Rect getBounds();
    bool hasUnitCollision();
    void setUnitCollision(bool b);
    void translate(int dx, int dy);
    SDL_Point location();
    std::vector<Attack> * getSkillSet();
        //return the direction the player is facing
    Direction getDirection();
    void setDirection(Direction d);

        //increments jumpCounter
    void jump();
    void resetJumpCounter();
    int getJumpCounter();
    void setJumpCounter(int j);

    mVector getVelocity();
    void setVelocity(int dx, int dy);
    void setXVelocity(int dx, bool forceRun = false);
    void setYVelocity(int dy);
        //frame independent movement along a vector
    void move();

    void receiveStatusEffect(StatusEffect effect);
    void removeBuffEffect(StatusEffect::BuffEffect type);
    bool hasBuffEffect(StatusEffect::BuffEffect type);
    std::unordered_map<StatusEffect::BuffEffect, StatusEffect> getBuffEffectsReceived();

    void removeDebuffEffect(StatusEffect::DebuffEffect type);
    bool hasDebuffEffect(StatusEffect::DebuffEffect type);
    std::unordered_map<StatusEffect::DebuffEffect, StatusEffect> getDebuffEffectsReceived();

    SDL_Point getPreviousChunk();
    void setPreviousChunk(SDL_Point p);
private:
        //*timer points to a constant timer, chainTimer used to determine if
        //  time limit for chaining a skill has expired
    Timer *timer;
    //SDL_Rect hitbox;
    PhysicsBody physicsbody;
        //for keeping track of the last time move() was called
    Uint32 moveStartTicks;

        //index of the current animation
    int animationIndex;
        //index of previous frame in the animation or -1 if animation is stopped
        //  used in determining return value of getSkillHitbox()
    int previousFrame;
    mVector skillVelocity;
    int hp;
        //keeps track of number of jumps made
    int jumpCounter;

    SDL_Point previousChunk;
};

#endif
