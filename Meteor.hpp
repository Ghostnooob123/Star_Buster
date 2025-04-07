#ifndef METEOR_HPP
#define METEOR_HPP

#include <SDL2/SDL.h>
#include <random>

class Meteor {
public:
	Meteor();

	const float getSpeed();
	SDL_FRect& getBody();
	void takeDamage(float dmg);
	const float getHealth();

private:
	SDL_FRect _body;
	float _speed;
	float _health;
};

#endif METEOR_HPP