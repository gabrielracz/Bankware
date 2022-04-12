#ifndef COLLECTIBLES_H
#define COLLECTIBLES_H
#include "collectible.hh"
#include "game_object.hh"

class ShieldCollectible : public Collectible {

public:
	ShieldCollectible(GLuint type, const glm::vec3& position, Game* game);
	void HandleCollision(Entity* player) override;
};

class DashCollectible : public Collectible {

public:
	DashCollectible(GLuint type, const glm::vec3& position, Game* game);
	void HandleCollision(Entity* player) override;
};

class BuffCollectible : public Collectible {

public:
	BuffCollectible(GLuint type, const glm::vec3& position, Game* game);
	void HandleCollision(Entity* player) override;
};

class WeaponCollectible : public Collectible {
	WeaponCollectible(GLuint type, const glm::vec3& position, Game* game);
	void HandleCollision(Entity* player) override;
};
#endif // COLLECTIBLES_H