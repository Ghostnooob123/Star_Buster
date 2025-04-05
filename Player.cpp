#include "Player.h"

Player::Player()
{
    this->_player.x = SCREEN_WIDTH / 2;
    this->_player.y = (SCREEN_HEIGHT / 2) + 200;
    this->_player.w = 40;
    this->_player.h = 40;
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
    move(0.0f, -0.15f, this->_strike);

    if (this->_strike.y < 0) {
        move(0.0f, 0.0f, this->_strike);
    }
}

const SDL_FRect& Player::renderStrike()
{
    return this->_strike;
}

void Player::shoot()
{
    if (this->_strike.y < 0)
    {
        this->_strike.w = 20;
        this->_strike.h = 40;
        this->_strike.x = this->_player.x;
        this->_strike.y = this->_player.y;
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
