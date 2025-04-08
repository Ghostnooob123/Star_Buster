#include "Meteor.h"

Meteor::Meteor() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrX(100, 850);

	this->_body.x = static_cast<float>(distrX(gen));
	this->_body.y = -100.0f;
	this->_body.h = 50.0f;
	this->_body.w = 70.0f;

	this->_health = 100;

	this->_speed = 0.2f;

	this->_MTexture = nullptr;

	std::uniform_int_distribution<> distrY(0, 1);
	if (distrY(gen) == 0)
	{
		this->_speed = 0.2f;
	}
	if (distrY(gen) == 1)
	{
		this->_speed = 0.3f;
	}
}

float Meteor::getSpeed()
{
	return this->_speed;
}

SDL_FRect& Meteor::getBody() {
	return this->_body;
}

void Meteor::takeDamage(int dmg) {
	this->_health -= dmg;
}

int Meteor::getHealth() {
	return this->_health;
}

void Meteor::setTexture(SDL_Texture* _newTexture)
{
	this->_MTexture = _newTexture;
}

SDL_Texture* Meteor::getTexture()
{
	return this->_MTexture;
}
