// C.cpp : Defines the entry point for the application.
//

#include "C.h"

const int FPS = 60;
const int frameDelay = 1000 / FPS;

int main(int argc, char* argv[]) {

    Engine en = Engine();

	while (en.running())
	{
		Uint32 frameStart = SDL_GetTicks();

		en.update();
		en.render();

		Uint32 frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

    return 0;
}