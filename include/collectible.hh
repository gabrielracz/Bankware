#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H
#include "game_object.hh"
#include "entity.hh"
#include "hitbox.hh"
#include "data.hh"

class Collectible : public GameObject{
private:
	Hitbox hitbox_;

public:
	Collectible(GLuint type, const glm::vec3& position, Game* game);
	virtual void HandleCollision(Entity* player); //Give the power up to the player
	inline Hitbox& GetHitbox() {return hitbox_;}
};

#endif
