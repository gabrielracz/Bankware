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

class PeaShooterCollectible : public Collectible {

public:
	PeaShooterCollectible(GLuint type, const glm::vec3& position, Game* game);
	void HandleCollision(Entity* player) override;
};

class DualCannonCollectible : public Collectible {
public:
	DualCannonCollectible(GLuint type, const glm::vec3& position, Game* game);
	void HandleCollision(Entity* player) override;
};

class RightCannonCollectible : public Collectible {
public:
	RightCannonCollectible(GLuint type, const glm::vec3& position, Game* game);
	void HandleCollision(Entity* player) override;
};

class LeftCannonCollectible : public Collectible {
public:
	LeftCannonCollectible(GLuint type, const glm::vec3& position, Game* game);
	void HandleCollision(Entity* player) override;
};

class RedKeyCollectible : public Collectible {
public:
	RedKeyCollectible(GLuint type, const glm::vec3& position, Game* game);
	void HandleCollision(Entity* player) override;
};

class BlueKeyCollectible : public Collectible {
public:
	BlueKeyCollectible(GLuint type, const glm::vec3& position, Game* game);
	void HandleCollision(Entity* player) override;
};

class YellowKeyCollectible : public Collectible {
public:
	YellowKeyCollectible(GLuint type, const glm::vec3& position, Game* game);
	void HandleCollision(Entity* player) override;
};
#endif // COLLECTIBLES_H