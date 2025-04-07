#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <random>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Consts.h"
#include "templateFunc.hpp"

#include "Player.h"
#include "Meteor.hpp"

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
	SDL_Renderer* _renderer;
	SDL_Event _event;

	SDL_Surface* _surface;
	SDL_Texture* _PTexture;
	SDL_Texture* _MTexture;
	SDL_Texture* _SmExTexture;
	SDL_Texture* _BigExTexture;
	SDL_Texture* _BGTexture;

	TTF_Font* _fontUI;
	SDL_Surface* _surfaceText;

	// Points UI
	SDL_Texture* _pointsTexture;
	SDL_Rect _pointsBody;


	int _mouseX, _mouseY;
	Uint32 _startTime = SDL_GetTicks();
	Uint32 _animationTime = SDL_GetTicks();

	Player _player;

	bool _close;
	int _spawnM;


	SDL_Rect _bgBody;
	std::vector<std::shared_ptr<Meteor>> meteors;
	void updateMeteor();
	
	void eventHandler();
	void updateMouse();
	void updateMovement();
	void exit();

	void initVariables();
	void initWindow();
	void initTextures();
	void initFont_Text();
};
#endif // !ENGINE_H
