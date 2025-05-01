#include "Pickups.h"

Pickups::Pickups()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrX(100, 850);

	this->_body.x = static_cast<float>(distrX(gen));
	this->_body.y = -100.0f;
	this->_body.w = 30.0f;
	this->_body.h = 30.0f;

	this->_P_UPTexture = nullptr;
	this->_type = PickupsType::None;
	this->_spawn = false;
}

Pickups::~Pickups()
{
}

SDL_FRect& Pickups::getBody()
{
	return this->_body;
}

SDL_Texture* Pickups::getTexture() const
{
	return this->_P_UPTexture;
}

void Pickups::setTexture(SDL_Texture* _newTexture)

{
	if (_newTexture)
	{
		this->_P_UPTexture = _newTexture;
	}
}

void Pickups::setType(PickupsType _newType)
{
	this->_type = _newType;
}

PickupsType Pickups::getType() const
{
	return this->_type;
}

void Pickups::setSpawn(bool _spawn)
{
	this->_spawn = _spawn;
}

bool Pickups::isSpawn()
{
	if (this->_spawn)
	{
		return true;
	}
	return false;
}

void Pickups::reset()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrX(100, 850);

	this->_body.x = static_cast<float>(distrX(gen));
	this->_body.y = -100.0f;
	this->_body.w = 30.0f;
	this->_body.h = 30.0f;
}
