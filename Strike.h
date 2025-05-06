#ifndef STRIKE_H
#define STRIKE_H
#include <SDL2/SDL.h>

class Strike {
public:
	SDL_FRect& getBody() {
		return _body;
	}
private:
	SDL_FRect _body;
};
#endif // !STRIKE_H