//
//  main.cpp
//  SDL2_potato4
//
//  Created by pow on 2017-09-20.
//  Copyright © 2017 potato. All rights reserved.
//

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "Game.hpp"


int main(int argc, const char * argv[]) {
    Size windowSize = {1280, 720};

    //init and setup, if something fails, free existing resources and quit the program
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not init with SDL_INIT_VIDEO. SDL error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL2_image could not init with IMG_INIT_PNG. SDL2_image error: %s\n", IMG_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

	if(TTF_Init() < 0) {
		printf("SDL2_ttf could not init. SDL2_ttf error: %s\n", TTF_GetError());
		IMG_Quit();
		SDL_Quit();
		return EXIT_FAILURE;
	}

    SDL_Window *window = SDL_CreateWindow("Potato 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		windowSize.w, windowSize.h, SDL_WINDOW_SHOWN);

    if(window == nullptr) {
        printf("Could not create Window. SDL error: %s\n", SDL_GetError());
        IMG_Quit();
		TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(renderer == nullptr) {
        printf("Could not create Renderer. SDL error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
		TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }


    //      end of init and setup
    //==================================================================//
    //////////////////////////////////////////////////////////////////////
    //==================================================================//


    Game *game = new Game(renderer, windowSize);
	//std::cout << SDL_GetTicks() << std::endl;
    while(game->run) {
        game->tick();
    }


    //      end of game
    //==================================================================//
    //////////////////////////////////////////////////////////////////////
    //==================================================================//


    //free resources and stuff
    delete game;
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    IMG_Quit();
	TTF_Quit();
    SDL_Quit();
    //end program
    return EXIT_SUCCESS;
}
