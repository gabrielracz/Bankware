#ifndef ENEMY_H
#define ENEMY_H
#include "entity.hh"

enum State {
	PATROLLING,
	HUNTING
};

class Enemy : public Entity {
protected:
	unsigned int state_;	
	glm::vec3 origin_;
	float path_tracker_;
	float path_radius_;
public:
	Enemy(GLuint texture, const glm::vec3& position, const glm::vec3& origin, Game* game, float speed = 0.45);
	virtual void Update(float dt) override;
};
#endif
