#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "game_object.hh"
#include "hitbox.hh"
class Entity;

class Projectile : public GameObject {
protected:
	float speed_;
	float lifetime_;
	GLuint proj_type_;
public:
	Projectile(GLuint texture, const glm::vec3 &position, float angle, Game* game, float speed = 30, float lifetime = 0.9f, GLuint proj_type = 0);
	
	void Update(float dt) override;
	bool RayCircleCollision(Hitbox& hitbox, float dt);
	void HandleCollision(Entity* ent);
	inline float GetSpeed() {return speed_;};
	inline GLuint GetProjectileType() const { return proj_type_; }
	
};
//testetr
#endif

