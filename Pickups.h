#ifndef PICKUPS_H
#define PICKUPS_H
#include <SDL2/SDL.h>
#include <random>
enum PickupsType
{
	Health,
	Shield,
	DoubleStrike,
	None
};

class Pickups
{
public:
	Pickups();
	~Pickups();
	SDL_FRect& getBody();
	SDL_Texture* getTexture() const;
	void setTexture(SDL_Texture* _newTexture);
	void setType(PickupsType _newType);
	PickupsType getType() const;
	void setSpawn(bool _spawn);
	bool isSpawn();
	void reset();
private:
	SDL_Texture* _P_UPTexture; // Texture for pickups
	SDL_FRect _body; // Body for pickups
	PickupsType _type; // Type of pickups
	bool _spawn; // Spawn pickups
};
#endif // PICKUPS_H