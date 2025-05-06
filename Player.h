#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include <memory>
#include <vector>

#include "Strike.h"
#include "Consts.h"
#include "templateFunc.h"

enum class StrikeType {
	SingleStrike,
	DoubleStrike,
	TripleStrike,
	None
};

class Player
{
public:
	Player();
	~Player();

	SDL_FRect& getBody();
	std::vector<std::shared_ptr<Strike>> getStrikes();
	bool existingStrike();
	void rmvStrike(std::shared_ptr<Strike> strike);
	void updateStrike();
	void shoot();
	void setHealth(int dmg);
	void heal(int heal);
	bool playerAlive();
	int getHealth();
	void turnOnShield();
	bool isShieldOn();
	void useShield(int damage);
	void setTexture(SDL_Texture* _newTexture);
	SDL_Texture* getTexture();
	void setSingleStrike();
	void setDoubleStrike();
private:
	SDL_FRect _body;

	std::vector<std::shared_ptr<Strike>> _strikes;
	SDL_Texture* _PTexture;
	int _health;
	int _shield;
	bool _isShield;
	StrikeType _strikeType;
	bool _rightWep;
};
#endif // !PLAYER_H