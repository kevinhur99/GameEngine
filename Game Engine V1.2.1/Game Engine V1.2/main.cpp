#include <SDL.h>
#undef main

#include "Game.hpp"

Game *game = nullptr;

int main(int argc, const char * argv[]) {

	// Establishing varibles
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;


	int frameStart;
	int frameTime;

	// Generate a class called Game
	game = new Game();

	// Have the game run init(), with the window called HurEngineV1.2, initially being centered in x and y, 
	// having the dimensions of 800 by 640 pixels, and not being full screen
	game->init("HurEngineV1.2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);

	// While the game is running
	while (game->running()) {

		//The frameStart is the the ms of when the SDL window started
		frameStart = SDL_GetTicks();

		// While running, the game will handleEvents, update, and render
		game->handleEvents();
		game->update();
		game->render();

		// This line of code will ensure that the FPS will stay the same
		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}

	}

	// When the game finished running, run clean()
	game->clean();

	return 0;
}

