#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>

#include <memory>
#include <random>

#include "Strike.h"
#include "templateFunc.h"

class Enemy {
public:
	Enemy();
	~Enemy();
	SDL_FRect& getBody();
	void setTexture(SDL_Texture* _newTexture);
	SDL_Texture* getTexture();
	bool moveLeft();
	bool moveRight();
	bool moveDown();
	void updateStrike();
	const SDL_FRect& renderStrike();
	SDL_FRect& getStrikeBody();
	void shoot();
	bool existingStrike();
	void rmvStrike();
	void setHealth(int _dmg);
	int getHealth();

private:
	std::shared_ptr<Strike> _strike;
	SDL_FRect _body;
	SDL_Texture* _ETexture;
	int health;
	bool _isStrike;
	bool _moveL;
	bool _moveR;
	bool _moveDw;
};

#endif // !ENEMY_H