#ifndef TEMPLATE_FUNC_HPP
#define  TEMPLATE_FUNC_HPP

#include <SDL2/SDL.h>
#include <memory>

template<typename T>
bool isInsideRect(int mouseX, int mouseY, T& rect)
{
    return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
        mouseY >= rect.y && mouseY <= rect.y + rect.h);
}

template<typename T>
void move(float x, float y, T& rect)
{
    rect.x += x;
    rect.y += y;
}
template<typename T>
void move(float x, float y, std::shared_ptr<T>& rect)
{
    rect->x += x;
    rect->y += y;
}

template<typename T>
void moveObj(float x, float y, std::shared_ptr<T>& rectClass)
{
    rectClass->getBody().x += x;
    rectClass->getBody().y += y;
}

template<typename T>
bool checkCollisionF(T& rect1, T& rect2) {

    return SDL_HasIntersectionF(&rect1, &rect2);
}
template<typename T>
bool checkCollision(T& rect1, T& rect2) {

    return SDL_HasIntersection(&rect1, &rect2);
}

#endif // ! TEMPLATE_FUNC_HPP