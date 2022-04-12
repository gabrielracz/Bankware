#include "projectile.hh"
#include "game.hh"
#include <glm/fwd.hpp>
#include <iostream>

Projectile::Projectile(GLuint texture, const glm::vec3 &position, float angle, Game* game, float speed, float lifetime, GLuint proj_type)
	: GameObject(texture, position, game) {
	angle_ = angle;
	speed_ = speed;
	lifetime_ = lifetime;
	proj_type_ = proj_type;
	SetScale(0.4);
	instigator_ = nullptr;
}

void Projectile::Update(float dt){
	lifetime_ -= dt;
	if(lifetime_ <= 0){
		SetDestroyed();
		return;
	}
	position_ += GetAngleVector() * speed_ * dt;
}

bool Projectile::RayCircleCollision(Hitbox& hitbox, float dt){
	//detect collision between this ray and the given circle hitbox
	if(&hitbox == instigator_) return false;

	glm::vec3 end = position_ + GetAngleVector() * speed_ * dt;
	glm::vec3 cir = *hitbox.GetOrigin();
	float r = hitbox.GetRadius();
	//If the hitbox is further away than this frame's travel distance there is no collision
	float distance_travelled = glm::length2(position_ - end);
	float distance_to_target = glm::length2(position_ - cir) - r;
	if(distance_to_target > distance_travelled){
		return false;
	}


	//a*t^2 + b*t + c = 0
	glm::vec3 dir = GetAngleVector();
	float a = glm::dot(dir, dir);
	float b = 2 * glm::dot(position_ - cir, dir);
	float c = glm::dot(position_ - cir, position_ - cir) - r*r;

	float discriminant = b*b - 4*a*c;

	if(discriminant < 0) //Negative discriminant means no real solution
		return false;

	//quadratic equation
	float sqrt_disc = sqrt(discriminant);
	float t1 = (-b + sqrt_disc) / 2*a;
	float t2 = (-b - sqrt_disc) / 2*a;

	if(t1 < 0 && t2 < 0)  //Collision occurs in the opposite direction to the bullet's travel
		return false;

	//if(t1 < 0 || t2 < 0)  //Only one is negative, the projectile is inside the hitbox. We don't want the player shooting themselves.
		//return false;
	//std::cout << t1 << " " << t2 << " - " << dt << std::endl;
	return true;   //Both of the collisions happen
}

void Projectile::HandleCollision(Entity *ent){
	SetDestroyed();
}
