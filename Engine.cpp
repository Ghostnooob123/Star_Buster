#include "Engine.h"

Engine::Engine()
{
	this->initVariables();
	this->initWindow();
	this->initTextures();
	this->initFont_Text();
}

Engine::~Engine()
{
	exit();
}

bool Engine::running()
{
	if (this->_close)
	{
		return false;
	}
	return true;
}

void Engine::update()
{
	this->updateMouse();
	this->eventHandler();

	if (this->_player->playerAlive()) {
		this->updatePlayer();
		this->updateBgObjects();
		this->_player->updateStrike();
	}
	else
	{
		this->updateBgObjects();
	}
}

void Engine::render()
{
	SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
	SDL_RenderClear(this->_renderer);

	SDL_RenderCopy(this->_renderer, _BGTexture, nullptr, &this->_bgBody);
	SDL_RenderCopyF(this->_renderer, this->_player->getTexture(), nullptr, &this->_player->getBody());

	for (const auto& strike : this->_player->getStrikes()) {
		if (strike) {
			SDL_RenderCopyF(this->_renderer, this->_STexture, nullptr, &strike->getBody());
		}
	}
	for (size_t i = 0; i < this->_meteors.size(); i++)
	{
		if (this->_meteors[i] != nullptr)
		{
			SDL_FRect body = this->_meteors[i]->getBody();
			SDL_FPoint center = { body.w / 2.0f, body.h / 2.0f };
			if (this->_meteors[i]->getTexture() != this->_MTexture) {
				SDL_RenderCopyExF(this->_renderer, this->_meteors[i]->getTexture(), nullptr, &body,
					0.0f, &center, SDL_FLIP_NONE);
			}
			else
			{
				SDL_RenderCopyExF(this->_renderer, this->_meteors[i]->getTexture(), nullptr, &body,
					this->_meteors[i]->getAngle(), &center, SDL_FLIP_NONE);
			}
		}
	}
	for (size_t i = 0; i < this->_enemies.size(); i++)
	{
		if (this->_enemies[i] != nullptr)
		{
			SDL_RenderCopyF(this->_renderer, this->_enemies[i]->getTexture(), nullptr, &this->_enemies[i]->getBody());
		}
		if (this->_enemies[i] != nullptr && this->_enemies[i]->existingStrike() &&
			this->_enemies[i]->getTexture() == this->_ETexture)
		{
			SDL_RenderCopyF(this->_renderer, this->_STexture, nullptr, &this->_enemies[i]->getStrikeBody());
		}
	}
	for (size_t i = 0; i < this->_pickups.size(); i++)
	{
		if (this->_pickups[i]->isSpawn()) {
			SDL_RenderCopyF(this->_renderer, this->_pickups[i]->getTexture(), nullptr, &this->_pickups[i]->getBody());
		}
	}

	// UI
	this->_surfaceText = TTF_RenderText_Solid(this->_fontUI, this->_strH.c_str(), WHITE_TEXT);
	if (!this->_surfaceText) {
		std::cerr << "Failed to create surface: " << TTF_GetError() << '\n';
	}
	this->_healthTexture = SDL_CreateTextureFromSurface(this->_renderer, this->_surfaceText);
	if (!this->_healthTexture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_RenderCopy(this->_renderer, this->_healthTexture, NULL, &this->_healthBody);
	SDL_FreeSurface(this->_surfaceText);
	SDL_DestroyTexture(this->_healthTexture);
	if (!this->_player->playerAlive())
	{
		SDL_SetRenderDrawColor(this->_renderer, 255, 0, 0, 255);
		SDL_RenderFillRectF(this->_renderer, &this->_restartBody);
	}

	SDL_RenderPresent(this->_renderer);
}

void Engine::updateMeteor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> distrAngle(-0.05, 0.03);

	if (SDL_GetTicks() - this->_startTime >= 2000 && this->_meteors.size() == 0)
	{
		for (size_t i = 0; i < METEOR_CAP; i++)
		{
			std::shared_ptr<Meteor> meteor = std::make_shared<Meteor>();
			meteor->setTexture(this->_MTexture);
			meteor->setRotate(static_cast<float>(distrAngle(gen)));
			this->_meteors.push_back(meteor);
		}

		for (size_t i = 0; i < this->_meteors.size() - 1; i++) {
			if (checkCollisionF(this->_meteors[i]->getBody(), this->_meteors[i + 1]->getBody())) {
				this->_meteors[i + 1] = nullptr;
			}
		}

		this->_startTime = SDL_GetTicks();
	}
	for (size_t i = 0; i < this->_meteors.size(); i++)
	{
		if (this->_meteors[i] != nullptr)
		{
			moveObj(0.0f, this->_meteors[i]->getSpeed(), this->_meteors[i]);
			this->_meteors[i]->rotate();

			bool outOfBounds = this->_meteors[i]->getBody().y < -110.0f ||
				this->_meteors[i]->getBody().y + this->_meteors[i]->getBody().h > SCREEN_HEIGHT + 100 ||
				this->_meteors[i]->getBody().x < 0 ||
				this->_meteors[i]->getBody().x + this->_meteors[i]->getBody().w > SCREEN_WIDTH;

			for (auto& strike : this->_player->getStrikes()) {
				if (strike && checkCollisionF(strike->getBody(), this->_meteors[i]->getBody()))
				{
					this->_meteors[i]->setHealth(50);
					if (this->_meteors[i]->getHealth() <= 0) {

						this->_meteors[i]->setTexture(this->_SmExTexture);
						this->_ExAnimTime = SDL_GetTicks();

					}
					this->_player->rmvStrike(strike);
				}
			}
			if (outOfBounds)
			{
				this->_meteors[i] = nullptr;
			}

			if (this->_meteors[i] != nullptr && this->_meteors[i]->getTexture() == this->_MTexture &&
				checkCollisionF(this->_player->getBody(), this->_meteors[i]->getBody()))
			{
				if (this->_player->isShieldOn()) {
					this->_player->useShield(10);
				}
				else
				{
					this->_player->setHealth(10);
					this->_strH = "HP: " + std::to_string(this->_player->getHealth());

					this->_PCollTime = SDL_GetTicks();
					this->_player->setTexture(this->_PCollTexture);
				}

				this->_meteors[i]->setTexture(this->_SmExTexture);
				this->_ExAnimTime = SDL_GetTicks();
			}
			if (SDL_GetTicks() - this->_PCollTime >= 150 && this->_player->getTexture() == this->_PCollTexture) {
				this->_player->setTexture(this->_PTexture);
			}
			if (this->_meteors[i] != nullptr && SDL_GetTicks() - this->_ExAnimTime >= 150 &&
				this->_meteors[i]->getTexture() == this->_SmExTexture)
			{
				this->_meteors[i]->setTexture(this->_BigExTexture);
				this->_ExAnimTime = SDL_GetTicks();
			}
			if (this->_meteors[i] != nullptr && SDL_GetTicks() - this->_ExAnimTime >= 200 &&
				this->_meteors[i]->getTexture() == this->_BigExTexture)
			{
				this->_meteors[i] = nullptr;
			}
		}
	}
	if (SDL_GetTicks() - this->_startTime >= 3000 && this->_meteors.size() != 0)
	{
		for (size_t i = 0; i < METEOR_CAP; i++)
		{
			if (this->_meteors[i] == nullptr)
			{
				std::shared_ptr<Meteor> meteor = std::make_shared<Meteor>();
				meteor->setTexture(this->_MTexture);
				meteor->setTexture(this->_MTexture);
				meteor->setRotate(static_cast<float>(distrAngle(gen)));
				this->_meteors[i] = meteor;
				break;
			}
		}
		this->_startTime = SDL_GetTicks();
	}
}

void Engine::updatePickups()
{
	bool pickUpExist = false;
	for (size_t i = 0; i < this->_pickups.size(); i++)
	{
		if (this->_pickups[i]->isSpawn()) {
			pickUpExist = true;
			break;
		}
	}
	if (SDL_GetTicks() - this->_pickupTime >= 10000 && !pickUpExist) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrType(0, PickupsType::None);

		for (size_t i = 0; i < this->_pickups.size(); i++)
		{
			if (i == distrType(gen)) {
				this->_pickups[i]->setSpawn(true);
				switch (this->_pickups[i]->getType())
				{
				case Health:
					this->_pickups[i]->setTexture(this->_healthTexture);
					break;
				case Shield:
					this->_pickups[i]->setTexture(this->_shieldTexture);
					break;
				case DoubleStrike:
					this->_pickups[i]->setTexture(this->_DB_Texture);
					break;
				}
				break;
			}
		}
	}
	for (size_t i = 0; i < this->_pickups.size(); i++)
	{
		if (this->_pickups[i]->isSpawn()) {
			move(0.0f, 1.5f, this->_pickups[i]->getBody());
		}
		if (this->_pickups[i]->getBody().y + this->_pickups[i]->getBody().h > SCREEN_HEIGHT + 40.0f)
		{
			this->_pickups[i]->setSpawn(false);
			this->_pickups[i]->reset();
		}
	}
}

void Engine::updateBgObjects()
{
	this->updateEnemy();
	this->updateMeteor();
	this->updatePickups();
}

void Engine::eventHandler()
{
	while (SDL_PollEvent(&this->_event)) {
		if (this->_event.type == SDL_QUIT) {
			exit();
			break;
		}
		if (this->_event.type == SDL_KEYDOWN) {
			if (this->_event.key.keysym.sym == SDLK_ESCAPE) {
				exit();
				break;
			}
			if (this->_event.key.keysym.sym == SDLK_SPACE) {
				if (!this->_player->existingStrike() && SDL_GetTicks() - this->_PlayerSTime >= 500)
				{
					this->_player->shoot();
					this->_PlayerSTime = SDL_GetTicks();
				}
			}
		}
		if (this->_event.type = SDL_MOUSEBUTTONDOWN && this->_event.button.button == SDL_BUTTON_LEFT)
		{
			if (isInsideRect(this->_mouseX, this->_mouseY, this->_restartBody))
			{
				this->restart();
			}
		}
	}
}

void Engine::updateMouse()
{
	SDL_GetMouseState(&this->_mouseX, &this->_mouseY);
}

void Engine::restart()
{
	this->_player = std::make_unique<Player>();
	this->_strH = "HP: " + std::to_string(this->_player->getHealth());
	this->_player->setTexture(this->_PTexture);
	this->_player->getBody().x = SCREEN_WIDTH / 2;
	this->_player->getBody().y = SCREEN_HEIGHT - 100;
	this->_player->setSingleStrike();
	this->_meteors.clear();
	this->_enemies.clear();
	this->_pickups.clear();
	for (size_t i = 0; i < PickupsType::None; i++)
	{
		std::shared_ptr<Pickups> pickup = std::make_shared<Pickups>();
		pickup->setType(static_cast<PickupsType>(i));
		this->_pickups.push_back(pickup);
	}

	this->_startTime = SDL_GetTicks();
	this->_PAnimTime = SDL_GetTicks();
	this->_ExAnimTime = SDL_GetTicks();
	this->_EnemyTime = SDL_GetTicks();
	this->_EnemySTime = SDL_GetTicks();
	this->_PlayerSTime = SDL_GetTicks();
	this->_pickupTime = SDL_GetTicks();
}

void Engine::updatePlayer()
{
	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_W]) move(0.0f, -5.5f, this->_player->getBody()); // Up
	if (state[SDL_SCANCODE_S]) move(0.0f, 5.5f, this->_player->getBody());  // Down
	if (state[SDL_SCANCODE_A]) move(-5.5f, 0.0f, this->_player->getBody()); // Left
	if (state[SDL_SCANCODE_D]) move(5.5f, 0.0f, this->_player->getBody());  // Right

	// Forbid player extend borders of the window
	if (this->_player->getBody().x < 0) this->_player->getBody().x = 0;
	if (this->_player->getBody().x + this->_player->getBody().w > SCREEN_WIDTH) this->_player->getBody().x =
		SCREEN_WIDTH - this->_player->getBody().w;
	if (this->_player->getBody().y < 0) this->_player->getBody().y = 0;
	if (this->_player->getBody().y + this->_player->getBody().h > SCREEN_HEIGHT + 40.0f) this->_player->getBody().y =
		(SCREEN_HEIGHT + 40.0f) - this->_player->getBody().h;

	if (this->_player->isShieldOn()) {
		if (SDL_GetTicks() - this->_PAnimTime >= 100)
		{
			this->_player->setTexture(this->_P2ShieldTexture);
			this->_PAnimTime = SDL_GetTicks();
		}
		else
		{
			this->_player->setTexture(this->_PShieldTexture);
		}
	}

	if (this->_player->getTexture() != this->_PCollTexture && !this->_player->isShieldOn())
	{
		if (SDL_GetTicks() - this->_PAnimTime >= 100)
		{
			this->_player->setTexture(this->_P2Texture);
			this->_PAnimTime = SDL_GetTicks();
		}
		else
		{
			this->_player->setTexture(this->_PTexture);
		}
	}

	for (size_t i = 0; i < this->_enemies.size(); i++)
	{
		if (this->_enemies[i] != nullptr && checkCollisionF(
			this->_enemies[i]->getStrikeBody(),
			this->_player->getBody()))
		{
			if (this->_player->isShieldOn()) {
				this->_player->useShield(10);
			}
			else
			{
				this->_player->setHealth(10);
				this->_strH = "HP: " + std::to_string(this->_player->getHealth());

				this->_PCollTime = SDL_GetTicks();
				this->_player->setTexture(this->_PCollTexture);
			}
			if (this->_player->getHealth() > 0)
			{
				this->_enemies[i]->rmvStrike();
			}

			if (SDL_GetTicks() - this->_PCollTime >= 150 && this->_player->getTexture() == this->_PCollTexture) {
				this->_player->setTexture(this->_PTexture);
			}
		}
	}
	for (size_t i = 0; i < this->_pickups.size(); i++)
	{
		if (checkCollisionF(this->_pickups[i]->getBody(), this->_player->getBody()))
		{
			switch (this->_pickups[i]->getType())
			{
			case PickupsType::Health:
				this->_player->heal(20);
				break;

			case PickupsType::Shield:
				this->_player->turnOnShield();
				break;

			case PickupsType::DoubleStrike:
				this->_player->setDoubleStrike();
				break;
			}
			this->_pickups[i]->setSpawn(false);
			this->_pickups[i]->reset();
			this->_pickupTime = SDL_GetTicks();
		}
	}
}

void Engine::updateEnemy()
{
	if (this->_enemies.size() > ENEMY_CAP) {
		if (allEnemiesDestroyed()) {
			this->_enemies.clear();
			this->_EnemyTime = SDL_GetTicks();
		}
	}
	if (SDL_GetTicks() - this->_EnemyTime >= 10000 && this->_enemies.empty())
	{
		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
		enemy->setTexture(this->_ETexture);
		this->_enemies.push_back(enemy);

		this->_EnemyTime = SDL_GetTicks();
	}
	for (size_t i = 0; i < this->_enemies.size(); i++)
	{
		if (this->_enemies[i] != nullptr) {
			if (this->_enemies[i]->moveDown() && this->_enemies[i]->getTexture() == this->_ETexture)
			{
				move(0.0f, 1.5f, this->_enemies[i]->getBody());
			}
			if (this->_enemies[i]->moveRight() && this->_enemies[i]->getTexture() == this->_ETexture)
			{
				move(2.0f, 0.0f, this->_enemies[i]->getBody());
			}
			if (this->_enemies[i]->moveLeft() && this->_enemies[i]->getTexture() == this->_ETexture)
			{
				move(-2.0f, 0.0f, this->_enemies[i]->getBody());
			}
			if (this->_enemies[i]->getTexture() == this->_ETexture) {
				this->_enemies[i]->updateStrike();
			}
			if (this->_enemies[i]->getBody().y >= 40.0f && SDL_GetTicks() - this->_EnemySTime >= 800) {
				if (!this->_enemies[i]->existingStrike())
				{
					this->_enemies[i]->shoot();
					this->_EnemySTime = SDL_GetTicks();
				}
			}
			for (auto& strike : this->_player->getStrikes()) {
				if (strike && checkCollisionF(strike->getBody(), this->_enemies[i]->getBody())) {
					this->_enemies[i]->setHealth(10);
					if (this->_enemies[i]->getHealth() <= 0)
					{
						this->_enemies[i]->setTexture(this->_SmExTexture);
						this->_ExAnimTime = SDL_GetTicks();
					}
					this->_player->rmvStrike(strike);
				}
			}
			if (this->_enemies[i] != nullptr && SDL_GetTicks() - this->_ExAnimTime >= 150 &&
				this->_enemies[i]->getTexture() == this->_SmExTexture)
			{
				this->_enemies[i]->setTexture(this->_BigExTexture);
				this->_ExAnimTime = SDL_GetTicks();
				this->_EnemyTime = SDL_GetTicks();
			}
			if (this->_enemies[i] != nullptr && SDL_GetTicks() - this->_ExAnimTime >= 200 &&
				this->_enemies[i]->getTexture() == this->_BigExTexture)
			{
				this->_enemies[i] = nullptr;
			}
			if (allEnemiesDestroyed())
			{
				std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
				enemy->setTexture(this->_ETexture);
				enemy = nullptr;
				this->_enemies.push_back(enemy);
			}
		}
	}
	if (SDL_GetTicks() - this->_EnemyTime >= 10000)
	{
		if (this->_enemies.size() <= ENEMY_CAP)
		{
			if (allEnemiesDestroyed()) {
				for (size_t i = 0; i < this->_enemies.size(); i++)
				{
					if (this->_enemies[i] == nullptr)
					{
						std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
						enemy->setTexture(this->_ETexture);
						this->_enemies[i] = enemy;
					}
				}
			}
		}
		this->_EnemyTime = SDL_GetTicks();
	}
}

bool Engine::allEnemiesDestroyed() const
{
	for (const auto& enemy : this->_enemies)
	{
		if (enemy != nullptr)
			return false;
	}
	return true;
}

void Engine::exit()
{
	SDL_DestroyTexture(this->_PTexture);
	SDL_DestroyTexture(this->_MTexture);
	SDL_DestroyTexture(this->_SmExTexture);
	SDL_DestroyTexture(this->_BigExTexture);
	SDL_DestroyTexture(this->_BGTexture);
	SDL_DestroyTexture(this->_healthTexture);

	SDL_DestroyWindow(this->_window);
	SDL_DestroyRenderer(this->_renderer);

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	this->_close = true;
}

void Engine::initVariables()
{
	TTF_Init();

	this->_close = false;

	this->_player = std::make_unique<Player>();
	for (size_t i = 0; i < PickupsType::None; i++)
	{
		std::shared_ptr<Pickups> pickup = std::make_shared<Pickups>();
		pickup->setType(static_cast<PickupsType>(i));
		this->_pickups.push_back(pickup);
	}

	this->_meteors.reserve(METEOR_CAP);
	this->_enemies.reserve(ENEMY_CAP);

	this->_restartBody.x = SCREEN_WIDTH / 2.0f - 50.0f;
	this->_restartBody.y = SCREEN_HEIGHT / 2.0f;
	this->_restartBody.h = 60.0f;
	this->_restartBody.w = 140.0f;

	this->_startTime = SDL_GetTicks();
	this->_PAnimTime = SDL_GetTicks();
	this->_ExAnimTime = SDL_GetTicks();
	this->_EnemyTime = SDL_GetTicks();
	this->_EnemySTime = SDL_GetTicks();
	this->_PlayerSTime = SDL_GetTicks();
	this->_pickupTime = SDL_GetTicks();
	this->_PCollTime = SDL_GetTicks();

	this->_bgBody.x = 0;
	this->_bgBody.y = 0;
	this->_bgBody.w = SCREEN_WIDTH;
	this->_bgBody.h = SCREEN_HEIGHT;
}

void Engine::initWindow()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Unable to init SDL : % s\n", SDL_GetError());
		return;
	}

	this->_window = SDL_CreateWindow("Star Buster"
		, SDL_WINDOWPOS_UNDEFINED
		, SDL_WINDOWPOS_UNDEFINED
		, SCREEN_WIDTH
		, SCREEN_HEIGHT
		, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);
}

void Engine::initTextures()
{
	// PLAYER TEXTURES
	this->_surface = IMG_Load("textures/ship.png");
	this->_PTexture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
	if (!this->_PTexture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(this->_surface);
	this->_surface = IMG_Load("textures/shipAn2.png");
	this->_P2Texture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
	if (!this->_P2Texture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(this->_surface);
	this->_player->setTexture(this->_PTexture);

	this->_surface = IMG_Load("textures/shipCol.png");
	this->_PCollTexture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
	if (!this->_PCollTexture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(this->_surface);
	this->_surface = IMG_Load("textures/shipShieldOn.png");
	this->_PShieldTexture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
	if (!this->_PShieldTexture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(this->_surface);
	this->_surface = IMG_Load("textures/shipShieldOnAn2.png");
	this->_P2ShieldTexture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
	if (!this->_P2ShieldTexture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(this->_surface);

	// METEOR TEXTURES
	this->_surface = IMG_Load("textures/meteor.png");
	this->_MTexture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
	if (!this->_MTexture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(this->_surface);

	// EXPLOSION TEXTURES
	this->_surface = IMG_Load("textures/smallEx.png");
	this->_SmExTexture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
	if (!this->_SmExTexture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(this->_surface);
	this->_surface = IMG_Load("textures/bigEx.png");
	this->_BigExTexture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
	if (!this->_BigExTexture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(this->_surface);

	// BACKGROUND TEXTURES
	this->_surface = IMG_Load("textures/bg.png");
	this->_BGTexture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
	if (!this->_BGTexture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(this->_surface);

	// STRIKE TEXTURES
	this->_surface = IMG_Load("textures/strike.png");
	this->_STexture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
	if (!this->_STexture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(this->_surface);

	// ENEMY TEXTURES
	this->_surface = IMG_Load("textures/enemy.png");
	this->_ETexture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
	if (!this->_ETexture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(this->_surface);

	// UI TEXTURES
	this->_surface = IMG_Load("textures/restart.png");
	this->_restartTexture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
	if (!this->_restartTexture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(this->_surface);

	// PICKUPS TEXTURES
	this->_surface = IMG_Load("textures/DB_strike.png");
	this->_DB_Texture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
	if (!this->_DB_Texture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(this->_surface);
	this->_surface = IMG_Load("textures/HP_regen.png");
	this->_HPTexture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
	if (!this->_HPTexture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(this->_surface);
	this->_surface = IMG_Load("textures/shield.png");
	this->_shieldTexture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
	if (!this->_shieldTexture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(this->_surface);
}

void Engine::initFont_Text()
{
	this->_fontUI = TTF_OpenFont("font/pixel.ttf", 24);
	if (!this->_fontUI) {
		std::cerr << "Failed to load font: " << TTF_GetError() << '\n';
	}

	// HEALTH TEXT
	this->_strH = "HP: " + std::to_string(this->_player->getHealth());
	this->_surfaceText = TTF_RenderText_Solid(this->_fontUI, this->_strH.c_str(), WHITE_TEXT);
	if (!this->_surfaceText) {
		std::cerr << "Failed to create surface: " << TTF_GetError() << '\n';
	}
	this->_healthTexture = SDL_CreateTextureFromSurface(this->_renderer, this->_surfaceText);
	if (!this->_healthTexture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(this->_surfaceText);
	this->_healthBody.x = 20;
	this->_healthBody.y = 580;
	this->_healthBody.h = 50;
	this->_healthBody.w = 140;
}
