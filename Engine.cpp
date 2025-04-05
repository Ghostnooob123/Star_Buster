#include "Engine.h"

Engine::Engine()
{
    this->initVariables();
    this->initWindow();
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

    this->updateMovement();
    this->updateMeteor();
    this->_player.updateStrike();

    if (this->_player.playerAlive()) {
       /* this->updateMovement();
        this->updateMeteor();
        this->_player.updateStrike();*/
    }
    else
    {
       // exit();
    }

}

void Engine::render()
{
    SDL_SetRenderDrawColor(this->_render, 0, 0, 0, 255);
    SDL_RenderClear(this->_render);

    SDL_SetRenderDrawColor(this->_render, 255, 0, 255, 255);
    SDL_RenderFillRectF(this->_render, &this->_player.getBody());

    SDL_SetRenderDrawColor(this->_render, 255, 255, 0, 0);
    SDL_RenderFillRectF(this->_render, &this->_player.renderStrike());

    for (size_t i = 0; i < meteors.size(); i++)
    {
        SDL_SetRenderDrawColor(this->_render, 255, 0, 0, 0);
        if (meteors[i] != nullptr)
        {
            SDL_RenderFillRectF(this->_render, &meteors[i]->getBody());
        }
    }

    SDL_RenderPresent(this->_render);


}

void Engine::updateMeteor()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrY(0, 1);

    if (meteors.size() == 0)
    {
        for (size_t i = 0; i < meteorCap; i++)
        {

            std::shared_ptr<Meteor> meteor = std::make_shared<Meteor>();

            meteors.push_back(meteor);
        }
    }

    for (size_t i = 0; i < meteorCap - 1; i++) {
        if (checkCollisionPtr(meteors[i], meteors[i + 1])) {
            meteors[i + 1] = nullptr;
        }
    }

    for (size_t i = 0; i < meteorCap; i++)
    {
        if (meteors[i] != nullptr)
        {
            move(0.0f, meteors[i]->getSpeed(), meteors[i]);

            if (meteors[i]->getBody().y + meteors[i]->getBody().h > SCREEN_HEIGHT + 100)
            {
                meteors[i] = nullptr;
            }
        }

       
    }

    bool reset = false;
    for (size_t i = 0; i < meteorCap; i++)
    {
        if (meteors[i] == nullptr)
        {
            reset = true;
        }
        else
        {
            reset = false;
        }
    }
    if (reset)
    {
        meteors.clear();
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
                this->_player.shoot();
            }
        }
        if (this->_event.type = SDL_MOUSEBUTTONDOWN && this->_event.button.button == SDL_BUTTON_LEFT)
        {
            if (isInsideRect(this->_mouseX, this->_mouseY, this->_player.getBody()))
            {
                exit();
            }
        }                                           
    }
}

void Engine::updateMouse()
{
    SDL_GetMouseState(&this->_mouseX, &this->_mouseY);
}

void Engine::updateMovement()
{
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_W]) move(0.0f, -0.1f, this->_player.getBody()); // Up
    if (state[SDL_SCANCODE_S]) move(0.0f, 0.1f, this->_player.getBody());  // Down
    if (state[SDL_SCANCODE_A]) move(-0.1f, 0.0f, this->_player.getBody()); // Left
    if (state[SDL_SCANCODE_D]) move(0.1f, 0.0f, this->_player.getBody());  // Right

    // Forbid player extend borders of the window
    if (this->_player.getBody().x < 0) this->_player.getBody().x = 0;
    if (this->_player.getBody().x + this->_player.getBody().w > SCREEN_WIDTH) this->_player.getBody().x =
        SCREEN_WIDTH - this->_player.getBody().w;
    if (this->_player.getBody().y < 0) this->_player.getBody().y = 0;
    if (this->_player.getBody().y + this->_player.getBody().h > SCREEN_HEIGHT) this->_player.getBody().y = 
        SCREEN_HEIGHT - this->_player.getBody().h;
    for (size_t i = 0; i < meteors.size(); i++)
    {
        if (checkCollisionF(this->_player.getBody(), this->meteors[i]->getBody()))
        {
            this->_player.takeDamage(50.0f);
            meteors[i] = nullptr;
        }
    }
}

void Engine::exit()
{
    SDL_DestroyWindow(this->_window);
    SDL_DestroyRenderer(this->_render);
    SDL_Quit();
    this->_close = true;
}

void Engine::initVariables()
{
    this->_close = false;
}

void Engine::initWindow()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Unable to init SDL : % s\n", SDL_GetError());
        return;
    }

    this->_window = SDL_CreateWindow("Engine"
        , SDL_WINDOWPOS_UNDEFINED
        , SDL_WINDOWPOS_UNDEFINED
        , SCREEN_WIDTH
        , SCREEN_HEIGHT
        , SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    this->_render = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);
}
