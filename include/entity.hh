#ifndef ENTITY_H
#define ENTITY_H
#include "game_object.hh"
#include "hitbox.hh"
#include "projectile.hh"

class Enemy;
class Effect;

class Entity : public GameObject {
protected:
	glm::vec3 velocity_;
	glm::vec3 acceleration_;
	float speed_;
	float mass_;
	Hitbox hitbox_;
	float shoot_cooldown_;
	float shoot_timer_;
	bool powered_up_;

	GLuint explosion_effect_;
	GLuint projectile_type_;

public:
	Entity(GLuint type, const glm::vec3& position, Game* game, float speed = 0.80, float mass = 1.0f);
	virtual void Update(float dt);
	
	void Turn(int d, float dt);
	void Thrust(float dt); 
	void Break();
	void Shoot();
	void LookAtPoint(glm::vec2);

	void SetEffect(GLuint eff);
	void SetScale(float s);
	glm::vec3& GetVelocity();
	float GetMass();
	void SetVelocity(const glm::vec3& v);
	void SetPosition(const glm::vec3& p);
	Hitbox& GetHitbox();
	bool CheckCollision(Hitbox& hbox);
	void Explode();
	bool CheckShield();
	void PowerUp();
	bool IsPoweredUp();

};
#endif
