#ifndef POWERUP_H
#define POWERUP_H
#include "collectible.hh"

class Powerup : public Collectible {

public:
	Powerup(GLuint type, const glm::vec3& position, Game* game);
	void HandleCollision(Entity* player) override;
};

#endif
