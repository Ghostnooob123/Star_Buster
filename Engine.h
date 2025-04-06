#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <random>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL_image.h>

#include "Consts.h"
#include "templateFunc.hpp"

#include "Player.h"
#include "Meteor.hpp"

constexpr auto speed = 5.0f;

class Engine
{
public:
	Engine();
	~Engine();

	bool running();

	void update();
	void render();

private:
	SDL_Window* _window;
	SDL_Renderer* _render;
	SDL_Event _event;
	SDL_Surface* _surface;
	SDL_Texture* _PTexture;
	SDL_Texture* _MTexture;
	SDL_Texture* _SmExTexture;
	SDL_Texture* _BigExTexture;

	int _mouseX, _mouseY;

	Player _player;

	bool _close;

	std::vector<std::shared_ptr<Meteor>> meteors;
	void updateMeteor();
	
	void eventHandler();
	void updateMouse();
	void updateMovement();
	void exit();

	void initVariables();
	void initWindow();
	void initTextures();
};
#endif // !ENGINE_H
