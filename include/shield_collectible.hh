#ifndef SHIELD_COLLECTIBLE_H
#define SHIELD_COLLECTIBLE_H
#include "collectible.hh"
#include "game_object.hh"

class ShieldCollectible : public Collectible {

public:
	ShieldCollectible(GLuint type, const glm::vec3& position, Game* game);
	void HandleCollision(Entity* player) override;
};

#endif
