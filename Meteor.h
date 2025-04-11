#ifndef METEOR_HPP
#define METEOR_HPP

#include <SDL2/SDL.h>
#include <random>

class Meteor {
public:
	Meteor();
	~Meteor();
	float getSpeed();
	SDL_FRect& getBody();
	void takeDamage(int dmg);
	int getHealth();
	void setTexture(SDL_Texture* t);
	SDL_Texture* getTexture();
	float getAngle() const;
	void setRotate(float _newDelta);
	void rotate();

private:
	SDL_FRect _body;
	float _speed;
	int _health;
	SDL_Texture* _MTexture;
	float _angle = 0.0f;
	float _delta = 0.0f;
};

#endif METEOR_HPP