#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <random>
#include <memory>

#include <SDL2/SDL.h>
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
};
#endif // !ENGINE_H
