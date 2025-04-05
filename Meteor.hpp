#ifndef METEOR_HPP
#define METEOR_HPP

#include <SDL2/SDL.h>

class Meteor {
public:
	Meteor() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrX(100, 850);

		this->_body.x = distrX(gen);
		this->_body.y = 0.0f;
		this->_body.h = 50.0f;
		this->_body.w = 70.0f;

		std::uniform_int_distribution<> distrY(0, 2);
		if (distrY(gen) == 0)
		{
			this->_speed = 0.010f;
		}if (distrY(gen) == 1)
		{
			this->_speed = 0.012f;
		}
		if (distrY(gen) == 2)
		{
			this->_speed = 0.014f;
		}
	}

	float& getSpeed() {
		return _speed;
	}

	SDL_FRect& getBody() {
		return this->_body;
	}

private:
	SDL_FRect _body;
	float _speed;
};

#endif METEOR_HPP