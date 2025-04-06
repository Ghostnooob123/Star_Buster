#include "Player.h"

Player::Player()
{
    this->_player.x = SCREEN_WIDTH / 2;
    this->_player.y = (SCREEN_HEIGHT / 2) + 200;
    this->_player.w = 50;
    this->_player.h = 50;

    this->_health = 100.0f;
    this->_rightWep = true;
    this->_strike = nullptr;
}

Player::~Player()
{

}

SDL_FRect& Player::getBody()
{
	return this->_player;
}

void Player::updateStrike()
{
    if (this->_strike != nullptr)
    {
        move(0.0f, -0.15f, this->_strike);

        if (this->_strike->y < 0) {
            this->_strike = nullptr;
        }
    }
}

const std::shared_ptr< SDL_FRect>& Player::renderStrike()
{
    return this->_strike;
}

std::shared_ptr< SDL_FRect>& Player::getStrikeBody()
{
    return this->_strike;
}

void Player::shoot()
{
    this->_strike = std::make_shared<SDL_FRect>();
    if (this->_strike != nullptr)
    {
        this->_strike->w = 10;
        this->_strike->h = 40;
        if (this->_rightWep)
        {
            this->_strike->x = this->_player.x;
            this->_rightWep = false;
        }
        else
        {
            this->_strike->x = this->_player.x + 40.0f;
            this->_rightWep = true;
        }
        this->_strike->y = this->_player.y;
    }
}

void Player::takeDamage(float dmg)
{
    this->_health -= dmg;
}

const bool Player::playerAlive()
{
    if (this->_health != 0.0f) {
        return true;
    }
    return false;
}
