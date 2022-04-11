#ifndef ENTITY_H
#define ENTITY_H
#include "game_object.hh"
#include "hitbox.hh"
#include "projectile.hh"
#include "weapon.hh"
#include "item.hh"
#include <string>

class Enemy;
class Effect;

class Entity : public GameObject {
protected:
	//Physics
	glm::vec3 velocity_;
	glm::vec3 acceleration_;
	float speed_;
	float mass_;
	Hitbox hitbox_;

	bool thrust_;

	//Weapons
	std::vector<Weapon*> weapons_;

	//Items
	std::vector<Item*> items_;

	//Effects
	GLuint explosion_effect_;

public:
	Entity(GLuint type, const glm::vec3& position, Game* game, float speed = 0.80, float mass = 1.0f);

	virtual void Update(float dt);
	
	void Turn(int d, float dt);
	void TurnTowards(glm::vec3& t, float dt);
	void Thrust(float dt); 
	void Break();
	void LookAtPoint(glm::vec3);
	void Shoot();
	bool CheckCollision(Hitbox& hbox);
	void Explode();
	bool CheckShield();
	void PowerUp();

	void AddWeapon(Weapon* wpn);

	void AddItem(Item* item);
	bool RaiseEvent(const std::string& event); //checks a given event against items and returns true if handled by an item

	bool IsPoweredUp();
	bool GetThrust();
	void SetEffect(GLuint eff);
	void SetScale(float s);
	glm::vec3& GetVelocity();
	inline glm::vec3& GetAcceleration() {return acceleration_;}
	float GetMass();
	void SetVelocity(const glm::vec3& v);
	void SetPosition(const glm::vec3& p);
	Hitbox& GetHitbox();

};
#endif
