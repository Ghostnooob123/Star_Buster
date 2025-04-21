#include "Player.h"

Player::Player()
{
	this->_body.x = SCREEN_WIDTH / 2.0f - 30;
	this->_body.y = (SCREEN_HEIGHT / 2.0f) + 200.0f;
	this->_body.w = 50.0f;
	this->_body.h = 50.0f;

	this->_health = 100;
	this->_rightWep = true;
	this->_strike = nullptr;

	this->_PTexture = nullptr;
}

Player::~Player()
{

}

SDL_FRect& Player::getBody()
{
	return this->_body;
}

void Player::updateStrike()
{
	if (this->existingStrike())
	{
		moveObj(0.0f, -15.0f, this->_strike);

		if (this->_strike->getBody().y < -20.0f) {
			this->rmvStrike();
		}
	}
}

const SDL_FRect& Player::renderStrike()
{
	return this->_strike->getBody();
}

SDL_FRect& Player::getStrikeBody()
{
	return this->_strike->getBody();
}

bool Player::existingStrike()
{
	if (this->_strike)
	{
		return true;
	}
	return false;
}

void Player::rmvStrike()
{
	this->_strike = nullptr;
}

void Player::shoot()
{
	this->_strike = std::make_shared<Strike>();
	if (this->existingStrike())
	{
		this->_strike->getBody().w = 40;
		this->_strike->getBody().h = 60;
		if (this->_rightWep)
		{
			this->_strike->getBody().x = this->_body.x - 10.0f;
			this->_rightWep = false;
		}
		else
		{
			this->_strike->getBody().x = this->_body.x + 20.0f;
			this->_rightWep = true;
		}
		this->_strike->getBody().y = this->_body.y;
	}
}

void Player::setHealth(int dmg)
{
	this->_health -= dmg;
}

bool Player::playerAlive()
{
	if (this->_health > 0) {
		return true;
	}
	return false;
}

int Player::getHealth()
{
	return this->_health;
}

void Player::setTexture(SDL_Texture* _newTexture)
{
	if (_newTexture)
	{
		this->_PTexture = _newTexture;
	}
}

SDL_Texture* Player::getTexture()
{
	return this->_PTexture;
}