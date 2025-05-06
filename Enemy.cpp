#include "Enemy.h"

Enemy::Enemy()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> distrWay(100.0f, 750.0f);

	this->_body.x = distrWay(gen);
	this->_body.y = -50.0f;
	this->_body.w = 60.0f;
	this->_body.h = 60.0f;

	this->_ETexture = nullptr;

	this->_moveL = false;
	this->_moveR = false;
	this->_moveDw = true;

	this->_isStrike = false;
	this->health = 110;
}

Enemy::~Enemy()
{

}

SDL_FRect& Enemy::getBody()
{
	return this->_body;
}

void Enemy::setTexture(SDL_Texture* _newTexture)
{
	if (_newTexture)
	{
		this->_ETexture = _newTexture;
	}
}

SDL_Texture* Enemy::getTexture()
{
	return this->_ETexture;
}

bool Enemy::moveLeft()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrWay(100, 200);

	if (!this->_moveR)
	{
		if (this->_body.x <= distrWay(gen))
		{
			this->_moveR = true;
			return false;
		}
		return true;
	}
	return false;
}

bool Enemy::moveRight()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrWay(600, 750);

	if (this->_moveR)
	{
		if (this->_body.x >= distrWay(gen))
		{
			this->_moveR = false;
			return false;
		}
		return true;
	}
	return false;
}

bool Enemy::moveDown()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrWay(40, 200);

	if (this->_body.y <= distrWay(gen))
	{
		return true;
	}
	return false;
}

void Enemy::updateStrike()
{
	if (this->existingStrike())
	{
		moveObj(0.0f, 11.0f, this->_strike);

		if (this->_strike->getBody().y > 920.0f) {
			this->rmvStrike();
		}
	}
}

const SDL_FRect& Enemy::renderStrike()
{
	return this->_strike->getBody();
}

SDL_FRect& Enemy::getStrikeBody()
{
	return this->_strike->getBody();
}

void Enemy::shoot()
{
	this->_strike = std::make_shared<Strike>();

	if (this->existingStrike())
	{
		this->_strike->getBody().w = 40;
		this->_strike->getBody().h = 60;
		this->_strike->getBody().x = this->_body.x;
		this->_strike->getBody().y = this->_body.y;
	}
}

bool Enemy::existingStrike()
{
	if (this->_strike)
	{
		return true;
	}
	return false;
}

void Enemy::rmvStrike()
{
	this->_strike = nullptr;
}

void Enemy::setHealth(int _dmg)
{
	this->health -= _dmg;
}

int Enemy::getHealth()
{
	return this->health;
}
