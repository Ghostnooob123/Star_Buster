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

    if (this->_player.playerAlive()) {
        this->updatePlayer();
        this->updateEnemy();
        this->updateMeteor();
        this->_player.updateStrike();
        if (this->_enemy)
        {
            this->_enemy->updateStrike();
        }
    }
    else
    {
        exit();
    }

}

void Engine::render()
{
    SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->_renderer);

    SDL_RenderCopy(this->_renderer, _BGTexture, nullptr, &this->_bgBody);

    SDL_RenderCopyF(this->_renderer, this->_player.getTexture(), nullptr, &this->_player.getBody());

    if (this->_player.existingStrike())
    {
        SDL_RenderCopyF(this->_renderer, this->_STexture, nullptr, &this->_player.getStrikeBody());
    }

    for (size_t i = 0; i < this->_meteors.size(); i++)
    {
        if (this->_meteors[i] != nullptr)
        {
            SDL_RenderCopyF(this->_renderer, this->_meteors[i]->getTexture(), nullptr, &this->_meteors[i]->getBody());
        }
    }

    if (this->_enemy != nullptr)
    {
        SDL_RenderCopyF(this->_renderer, this->_enemy->getTexture(), nullptr, &this->_enemy->getBody());
    }
    if (this->_enemy != nullptr && this->_enemy->existingStrike())
    {
        SDL_RenderCopyF(this->_renderer, this->_STexture, nullptr, &this->_enemy->getStrikeBody());
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
    SDL_DestroyTexture(_healthTexture);

    SDL_RenderPresent(this->_renderer);
}

void Engine::updateMeteor()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrY(0, 1);

    if (this->_meteors.size() == 0)
    {
        for (size_t i = 0; i < METEOR_CAP; i++)
        {
            std::shared_ptr<Meteor> meteor = std::make_shared<Meteor>();
            meteor->setTexture(this->_MTexture);
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

            bool outOfBounds = this->_meteors[i]->getBody().y < -110.0f ||
                this->_meteors[i]->getBody().y + this->_meteors[i]->getBody().h > SCREEN_HEIGHT + 100 ||
                this->_meteors[i]->getBody().x < 0 ||
                this->_meteors[i]->getBody().x + this->_meteors[i]->getBody().w > SCREEN_WIDTH;

            bool hit = this->_player.existingStrike() &&
                checkCollisionF(this->_player.getStrikeBody(), this->_meteors[i]->getBody());

            if (hit)
            {
                this->_meteors[i]->takeDamage(50);
                if (this->_meteors[i]->getHealth() == 0) {

                    this->_meteors[i]->setTexture(this->_SmExTexture);
                    this->_ExAnimTime = SDL_GetTicks();
                }
                if (this->_meteors[i]->getHealth() > 0)
                {
                    this->_player.rmvStrike();
                }
            }
            else if (outOfBounds)
            {
                this->_meteors[i] = nullptr;
            }

            if (this->_meteors[i] != nullptr && this->_meteors[i]->getTexture() == this->_MTexture &&
                checkCollisionF(this->_player.getBody(), this->_meteors[i]->getBody()))
            {
                this->_player.takeDamage(10);
                this->_strH = "HP: " + std::to_string(this->_player.getHealth());

                this->_meteors[i]->setTexture(this->_SmExTexture);
                this->_ExAnimTime = SDL_GetTicks();
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
                this->_meteors[i] = meteor;
                break;
            }
        }

        this->_startTime = SDL_GetTicks();
    }
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
                if (!this->_player.existingStrike() && SDL_GetTicks() - this->_PlayerSTime >= 500)
                {
                    this->_player.shoot();
                    this->_PlayerSTime = SDL_GetTicks();
                }
            }
        }
        //if (this->_event.type = SDL_MOUSEBUTTONDOWN && this->_event.button.button == SDL_BUTTON_LEFT)
        //{
        //    if (isInsideRect(this->_mouseX, this->_mouseY, this->_player.getBody()))
        //    {
        //        exit();
        //    }
        //}
    }
}

void Engine::updateMouse()
{
    SDL_GetMouseState(&this->_mouseX, &this->_mouseY);
}

void Engine::updatePlayer()
{
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_W]) move(0.0f, -5.5f, this->_player.getBody()); // Up
    if (state[SDL_SCANCODE_S]) move(0.0f, 5.5f, this->_player.getBody());  // Down
    if (state[SDL_SCANCODE_A]) move(-5.5f, 0.0f, this->_player.getBody()); // Left
    if (state[SDL_SCANCODE_D]) move(5.5f, 0.0f, this->_player.getBody());  // Right

    // Forbid player extend borders of the window
    if (this->_player.getBody().x < 0) this->_player.getBody().x = 0;
    if (this->_player.getBody().x + this->_player.getBody().w > SCREEN_WIDTH) this->_player.getBody().x =
        SCREEN_WIDTH - this->_player.getBody().w;
    if (this->_player.getBody().y < 0) this->_player.getBody().y = 0;
    if (this->_player.getBody().y + this->_player.getBody().h > SCREEN_HEIGHT + 40.0f) this->_player.getBody().y = 
        (SCREEN_HEIGHT + 40.0f ) - this->_player.getBody().h;

    if (SDL_GetTicks() - this->_PAnimTime >= 100)
    {
        this->_player.setTexture(this->_P2Texture);     
        this->_PAnimTime = SDL_GetTicks();
    }
    else
    {
        this->_player.setTexture(this->_PTexture);
    }

    if (this->_enemy != nullptr && checkCollisionF(this->_enemy->getStrikeBody(), this->_player.getBody()))
    {
        this->_player.takeDamage(10);
        this->_strH = "HP: " + std::to_string(this->_player.getHealth());
        if (this->_player.getHealth() == 0)
        {
            
        }
        if (this->_player.getHealth() > 0)
        {
            this->_enemy->rmvStrike();
        }
    }
}

void Engine::updateEnemy()
{
    if (SDL_GetTicks() - this->_EnemyTime >= 5000)
    {
        if (this->_enemy == nullptr)
        {
            this->_enemy = std::make_shared<Enemy>();
            this->_enemy->setTexture(this->_ETexture);
            this->_EnemyTime = SDL_GetTicks();
        }
        else
        {
            if (this->_enemy->moveDown())
            {
                move(0.0f, 1.5f, this->_enemy->getBody());
            }
            if (this->_enemy->moveRight())
            {
                move(2.5f, 0.0f, this->_enemy->getBody());
            }
            if (this->_enemy->moveLeft())
            {
                move(-2.5f, 0.0f, this->_enemy->getBody());
            }
        }
    }

    if (this->_enemy != nullptr)
    {
        if (this->_enemy->getBody().y >= 40.0f && SDL_GetTicks() - this->_EnemySTime >= 800) {
            if (!this->_enemy->existingStrike())
            {
                this->_enemy->shoot();
                _EnemySTime = SDL_GetTicks();
            }
        }

        if (checkCollisionF(this->_player.getStrikeBody(), this->_enemy->getBody()))
        {
            this->_enemy->takeDamage(20);
            if (this->_enemy->getHealth() == 0)
            {
                this->_enemy->setTexture(this->_SmExTexture);
                this->_ExAnimTime = SDL_GetTicks();
            }
            if (this->_enemy->getHealth() > 0)
            {
                this->_player.rmvStrike();
            }
        }
    }

    if (this->_enemy != nullptr && SDL_GetTicks() - this->_ExAnimTime >= 150 &&
        this->_enemy->getTexture() == this->_SmExTexture)
    {
        this->_enemy->setTexture(this->_BigExTexture);
        this->_ExAnimTime = SDL_GetTicks();
    }
    if (this->_enemy != nullptr && SDL_GetTicks() - this->_ExAnimTime >= 200 &&
        this->_enemy->getTexture() == this->_BigExTexture)
    {
        this->_enemy = nullptr;
    }
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

    this->_startTime = SDL_GetTicks();
    this->_PAnimTime = SDL_GetTicks();
    this->_ExAnimTime = SDL_GetTicks();
    this->_EnemyTime = SDL_GetTicks();
    this->_EnemySTime = SDL_GetTicks();
    this->_PlayerSTime = SDL_GetTicks();

    this->_bgBody.x = 0;
    this->_bgBody.y = 0;
    this->_bgBody.w = SCREEN_WIDTH;
    this->_bgBody.h = SCREEN_HEIGHT;

    this->_enemy = std::make_shared<Enemy>();
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
    this->_player.setTexture(this->_PTexture);

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
    this->_enemy->setTexture(this->_ETexture);
}

void Engine::initFont_Text()
{
    this->_fontUI = TTF_OpenFont("font/pixel.ttf", 24);
    if (!this->_fontUI) {
        std::cerr << "Failed to load font: " << TTF_GetError() << '\n';
    }

    // HEALTH TEXT
    this->_strH = "HP: " + std::to_string(this->_player.getHealth());
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
