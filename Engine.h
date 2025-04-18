#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <random>
#include <memory>
#include <string>

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Consts.h"
#include "templateFunc.h"

#include "Player.h"
#include "Meteor.h"
#include "Enemy.h"

class Engine
{
public:
	Engine();
	~Engine();

	bool running();

	void update();
	void render();

private:
	SDL_Window* _window; // Window
	SDL_Renderer* _renderer; // Renderer
	SDL_Event _event; // Event handler

	SDL_Surface* _surface;
	SDL_Texture* _PTexture;	 // Player texture
	SDL_Texture* _P2Texture; // Player_second texture for animation
	SDL_Texture* _MTexture;	// Meteor texture
	SDL_Texture* _BGTexture; // Backgound texture
	SDL_Texture* _STexture; // Strike texture
	SDL_Texture* _ETexture; // Enemy texture

	SDL_Texture* _SmExTexture; // Small explosion texture
	SDL_Texture* _BigExTexture;	// Big explosion texture

	TTF_Font* _fontUI; // TTF for global font UI
	SDL_Surface* _surfaceText; // Surface only for UI

	// Health UI
	SDL_Texture* _healthTexture; // UI texture for health
	SDL_Rect _healthBody; // UI body for health
	std::string _strH; // Updates health string


	bool _close;
	int _mouseX, _mouseY; // mouse x and y cords
	Uint32 _startTime; // Start game timer
	Uint32 _PAnimTime; // Player animation refresh
	Uint32 _ExAnimTime;	// Explosion delay between the frames
	Uint32 _EnemyTime; // Enemy spawn timer
	Uint32 _PlayerSTime; // Player strike interval 
	Uint32 _EnemySTime; // Enemy strike interval 

	SDL_Rect _bgBody; // Background rect 
	std::vector<std::shared_ptr<Meteor>> _meteors; // Stores meteors objects
	Player _player; // Player object
	std::shared_ptr<Enemy> _enemy; // Enemy object

	void updatePlayer(); // Updates player object
	void updateEnemy(); // Updates enemy object
	void updateMeteor(); // Updates meteor object
	
	void eventHandler(); // Handle the updates
	void updateMouse(); // Updates mouse position
	void exit(); // Exit the game and release necessary objects	from the memory

	void initVariables();
	void initWindow();
	void initTextures();
	void initFont_Text();
};
#endif // !ENGINE_H
