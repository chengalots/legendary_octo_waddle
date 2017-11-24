//
//  PhysicsBody.hpp
//  SDL2_potato3
//
//  Created by pow on 2017-10-3.
//  Copyright © 2017 potato. All rights reserved.
//

#ifndef PHYSICSBODY_HPP
#define PHYSICSBODY_HPP
#include <SDL2/SDL.h>
#include <unordered_map>
#include <iostream>
#include "mVector.hpp"
#include "StatusEffect.hpp"
#include "Timer.hpp"

class PhysicsBody {
public:
	PhysicsBody();
	PhysicsBody(SDL_Rect bounds, Timer * timer = nullptr);
	void setTimer(Timer * newTimer);

	void receiveStatusEffect(StatusEffect effect);

	void removeBuffEffect(StatusEffect::BuffEffect type);
	bool hasBuffEffect(StatusEffect::BuffEffect type);
	std::unordered_map<StatusEffect::BuffEffect, StatusEffect> getBuffEffectsReceived();

	void removeDebuffEffect(StatusEffect::DebuffEffect type);
	bool hasDebuffEffect(StatusEffect::DebuffEffect type);
	std::unordered_map<StatusEffect::DebuffEffect, StatusEffect> getDebuffEffectsReceived();

	SDL_Rect * getBounds();
	int w();
	int h();
	SDL_Point location();
	void location(int x, int y);
	void translate(int dx, int dy);

	mVector getVelocity();
	Direction getDirection();
	void setVelocity(double dx, double dy);
	void setXVelocity(double dx);
	void setYVelocity(double dy);
	void setDirection(Direction d);
	void move();

private:
	std::unordered_map<StatusEffect::DebuffEffect, StatusEffect> debuffEffectsReceived;
	std::unordered_map<StatusEffect::BuffEffect, StatusEffect> buffEffectsReceived;

	SDL_Rect hitbox;

	mVector velocity;
	Direction direction;

	Timer *timer;
	unsigned int moveStartTicks;
};

#endif
