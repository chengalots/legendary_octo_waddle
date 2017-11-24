//
//  StatusEffect.hpp
//  SDL2_potato3
//
//  Created by pow on 2017-10-1.
//  Copyright © 2017 potato. All rights reserved.
//

#ifndef STATUSEFFECT_HPP
#define STATUSEFFECT_HPP

typedef struct {
	enum DebuffEffect {
		KNOCK_UP = 0,
		KNOCK_DOWN, //1
		STAGGER, //2
		AIRBORNE, //3
		STUNNED, //4
		ROOTED, //5
		NO_DEBUFF //
	};
	enum BuffEffect {
		IFRAME = 0,
		UNWAVERING, //1
		CD_CHANCE_RESET, //2
		CD_RESET_SKILL, //3
		BLOODLUST, //4
		NO_BUFF //
	};
	enum stackLimits {
		BLOODLUST_STACK_LIMIT = 16,
	};

	DebuffEffect debuff;
	BuffEffect buff;
	int duration;
	int startDurationTicks;
	int stacks;
} StatusEffect;

#endif
