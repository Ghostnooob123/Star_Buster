#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include "Consts.h"
#include "templateFunc.hpp"

class Player
{
public:
	Player();
	~Player();

	SDL_FRect& getBody();
	void updateStrike();
	const SDL_FRect& renderStrike();
	void shoot();
	void takeDamage(float dmg);
	const bool playerAlive();
private:
	SDL_FRect _player;
	SDL_FRect _strike;
	float _health = 100.0f;
};
#endif // !PLAYER_H