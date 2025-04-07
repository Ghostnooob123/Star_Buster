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
	const SDL_FRect& renderStrike();
	SDL_FRect& getStrikeBody();
	const bool existingStrike();
	void rmvStrike();
	void updateStrike();
	void shoot();
	void takeDamage(float dmg);
	const bool playerAlive();
private:
	class Strike {
	public:
		SDL_FRect& getBody() {
			return _body;
		}
	private:
		SDL_FRect _body;
	};
	SDL_FRect _player;
	std::shared_ptr<Strike> _strike;
	float _health;
	bool _rightWep;
};
#endif // !PLAYER_H