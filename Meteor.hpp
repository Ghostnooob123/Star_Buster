#ifndef METEOR_HPP
#define METEOR_HPP

#include <SDL2/SDL.h>
#include <random>

class Meteor {
public:
	Meteor() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrX(100, 850);

		this->_body.x = static_cast<float>(distrX(gen));
		this->_body.y = 0.0f;
		this->_body.h = 50.0f;
		this->_body.w = 70.0f;

		this->_health = 100.0f;

		std::uniform_int_distribution<> distrY(0, 2);
		if (distrY(gen) == 0)
		{
			this->_speed = 0.004f;
		}if (distrY(gen) == 1)
		{
			this->_speed = 0.005f;
		}
		if (distrY(gen) == 2)
		{
			this->_speed = 0.007f;
		}
	}

	float& getSpeed() {
		return _speed;
	}

	SDL_FRect& getBody() {
		return this->_body;
	}

	void takeDamage(float dmg) {
		this->_health -= dmg;
	}

	constexpr float getHealth() {
		return this->_health;
	}

private:
	SDL_FRect _body;
	float _speed;
	float _health;
};

#endif METEOR_HPP