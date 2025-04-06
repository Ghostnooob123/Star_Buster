#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include <memory>

#include "Consts.h"
#include "templateFunc.hpp"

class Player
{
public:
	Player();
	~Player();

	SDL_FRect& getBody();
	const std::shared_ptr< SDL_FRect>& renderStrike();
	std::shared_ptr< SDL_FRect>& getStrikeBody();
	void updateStrike();
	void shoot();
	void takeDamage(float dmg);
	const bool playerAlive();
private:
	SDL_FRect _player;
	std::shared_ptr<SDL_FRect> _strike = nullptr;
	float _health = 100.0f;
	bool _rightWep = true;
};
#endif // !PLAYER_H