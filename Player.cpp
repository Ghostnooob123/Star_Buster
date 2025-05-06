#include "Player.h"

Player::Player()
{
	this->_body.x = SCREEN_WIDTH / 2.0f - 30;
	this->_body.y = (SCREEN_HEIGHT / 2.0f) + 200.0f;
	this->_body.w = 50.0f;
	this->_body.h = 50.0f;

	this->_health = 100;
	this->_rightWep = true;

	this->_PTexture = nullptr;

	this->_strikeType = StrikeType::SingleStrike;
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
	switch (this->_strikeType)
	{
	case StrikeType::SingleStrike: {
		for (size_t i = 0; i < this->_strikes.size(); i++)
		{
			if (this->_strikes[i])
			{
				moveObj(0.0f, -12.0f, this->_strikes[i]);
				if (this->_strikes[i]->getBody().y < -20.0f) {
					this->rmvStrike(this->_strikes[i]);
				}
			}
		}
		break;
	}
	case StrikeType::DoubleStrike: {
		for (size_t i = 0; i < this->_strikes.size(); i++)
		{
			if (this->_strikes[i])
			{
				moveObj(0.0f, -10.0f, this->_strikes[i]);
				if (this->_strikes[i]->getBody().y < -20.0f) {
					this->rmvStrike(this->_strikes[i]);
				}
			}
		}
		break;
	}
	}
}


std::vector<std::shared_ptr<Strike>> Player::getStrikes()
{
	return this->_strikes;
}

bool Player::existingStrike()
{
	for (size_t i = 0; i < this->_strikes.size(); i++)
	{
		if (this->_strikes[i])
		{
			return true;
		}
	}
	return false;
}

void Player::rmvStrike(std::shared_ptr<Strike> strike)
{
	auto it = std::remove_if(this->_strikes.begin(), this->_strikes.end(),
		[&](const std::shared_ptr<Strike>& s) {
			return s == strike;
		});

	if (it != this->_strikes.end())
	{
		this->_strikes.erase(it, this->_strikes.end());
	}
}

void Player::shoot()
{
	switch (this->_strikeType)
	{
	case StrikeType::SingleStrike:
	{
		std::shared_ptr<Strike> strike = std::make_shared<Strike>();

		if (!this->existingStrike())
		{
			strike->getBody().w = 40;
			strike->getBody().h = 60;
			if (this->_rightWep)
			{
				strike->getBody().x = this->_body.x - 10.0f;
				this->_rightWep = false;
			}
			else
			{
				strike->getBody().x = this->_body.x + 20.0f;
				this->_rightWep = true;
			}
			strike->getBody().y = this->_body.y;
			this->_strikes.push_back(strike);
		}
	}
	break;
	case StrikeType::DoubleStrike:
	{
		std::shared_ptr<Strike> strike_1 = std::make_shared<Strike>();
		std::shared_ptr<Strike> strike_2 = std::make_shared<Strike>();
		if (!this->existingStrike())
		{
			strike_1->getBody().w = 40;
			strike_1->getBody().h = 60;

			strike_2->getBody().w = 40;
			strike_2->getBody().h = 60;
			if (this->_rightWep)
			{
				strike_1->getBody().x = this->_body.x - 10.0f;
				strike_2->getBody().x = this->_body.x - 10.0f;
				this->_rightWep = false;
			}
			else
			{
				strike_1->getBody().x = this->_body.x + 20.0f;
				strike_2->getBody().x = this->_body.x + 20.0f;
				this->_rightWep = true;
			}
			strike_1->getBody().y = this->_body.y;
			strike_2->getBody().y = this->_body.y - 60.0f;
		}
		this->_strikes.push_back(strike_1);
		this->_strikes.push_back(strike_2);
	}
	break;
	}

}

void Player::setHealth(int dmg)
{
	this->_health -= dmg;
}

void Player::heal(int heal)
{
	if (this->_health + heal > 100)
	{
		this->_health = 100;
	}
	else
	{
		this->_health += heal;
	}
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

void Player::setSingleStrike()
{
	this->_strikeType = StrikeType::SingleStrike;
}

void Player::setDoubleStrike()
{
	this->_strikeType = StrikeType::DoubleStrike;
}
