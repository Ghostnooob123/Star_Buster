#include "Meteor.h"

Meteor::Meteor() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrX(100, 850);
	std::uniform_int_distribution<> distrY(-100, -95);

	this->_body.x = static_cast<float>(distrX(gen));
	this->_body.y = static_cast<float>(distrY(gen));
	this->_body.w = 70.0f;
	this->_body.h = 50.0f;

	this->_health = 100;

	this->_speed = 0.2f;

	this->_MTexture = nullptr;

	std::uniform_real_distribution<> distrSpeed(0.2f, 0.3f);
	this->_speed = static_cast<float>(distrSpeed(gen));
}

Meteor::~Meteor()
{

}

float Meteor::getSpeed()
{
	return this->_speed;
}

SDL_FRect& Meteor::getBody() {
	return this->_body;
}

void Meteor::setHealth(int dmg) {
	this->_health -= dmg;
}

int Meteor::getHealth() {
	return this->_health;
}

void Meteor::setTexture(SDL_Texture* _newTexture)
{
	if (_newTexture)
	{
		this->_MTexture = _newTexture;
	}
}

SDL_Texture* Meteor::getTexture()
{
	return this->_MTexture;
}

float Meteor::getAngle() const
{
	return this->_angle;
}

void Meteor::setRotate(float _newDelta)
{
	this->_delta = _newDelta;
}

void Meteor::rotate()
{
	this->_angle += this->_delta;
}
