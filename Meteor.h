#ifndef METEOR_HPP
#define METEOR_HPP

#include <SDL2/SDL.h>
#include <random>

class Meteor {
public:
	Meteor();

	float getSpeed();
	SDL_FRect& getBody();
	void takeDamage(int dmg);
	int getHealth();
	void setTexture(SDL_Texture* t);
	SDL_Texture* getTexture();

private:
	SDL_FRect _body;
	float _speed;
	int _health;
	SDL_Texture* _MTexture;
};

#endif METEOR_HPP