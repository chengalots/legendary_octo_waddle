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
        std::cerr << "SDL could not init with SDL_INIT_VIDEO. SDL error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL2_image could not init with IMG_INIT_PNG. SDL2_image error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

	if(TTF_Init() < 0) {
		std::cerr << "SDL2_ttf could not init. SDL2_ttf error: " << TTF_GetError() << std::endl;
		IMG_Quit();
		SDL_Quit();
		return EXIT_FAILURE;
	}

    SDL_Window *window = SDL_CreateWindow("Potato 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		windowSize.w, windowSize.h, SDL_WINDOW_SHOWN);

    if(window == nullptr) {
        std::cerr << "Could not create Window. SDL error: " << SDL_GetError() << std::endl;
        IMG_Quit();
		TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(renderer == nullptr) {
        std::cerr << "Could not create Renderer. SDL error: " << SDL_GetError() << std::endl;
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
