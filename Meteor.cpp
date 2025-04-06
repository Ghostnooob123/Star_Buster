#include "Meteor.hpp"

Meteor::Meteor() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrX(100, 850);

	this->_body.x = static_cast<float>(distrX(gen));
	this->_body.y = -100.0f;
	this->_body.h = 50.0f;
	this->_body.w = 70.0f;

	this->_health = 100.0f;

	this->_speed = 0.004f;

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

const float Meteor::getSpeed()
{
	return this->_speed;
}

SDL_FRect& Meteor::getBody() {
	return this->_body;
}

void Meteor::takeDamage(float dmg) {
	this->_health -= dmg;
}

const float Meteor::getHealth() {
	return this->_health;
}
