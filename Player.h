#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include <memory>

#include "Consts.h"
#include "templateFunc.h"

class Player
{
public:
	Player();
	~Player();

	SDL_FRect& getBody();
	const SDL_FRect& renderStrike();
	SDL_FRect& getStrikeBody();
	bool existingStrike();
	void rmvStrike();
	void updateStrike();
	void shoot();
	void takeDamage(int dmg);
	bool playerAlive();
	int getHealth();
	void setTexture(SDL_Texture* _newTexture);
	SDL_Texture* getTexture();
private:
	class Strike {
	public:
		SDL_FRect& getBody() {
			return _body;
		}
	private:
		SDL_FRect _body;
	};
	SDL_FRect _body;

	std::shared_ptr<Strike> _strike;
	SDL_Texture* _PTexture;
	int _health;
	bool _rightWep;
};
#endif // !PLAYER_H