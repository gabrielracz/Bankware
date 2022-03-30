#include "entity.hh"
#include "data.hh"
#include "effect.hh"
#include "game.hh"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <iostream>
#define DRAG_CONSTANT 5.0f

Entity::Entity(GLuint type, const glm::vec3& position, Game* game, float speed, float mass)
	:GameObject(type, position, game){
	acceleration_ = glm::vec3(0);
	velocity_ = glm::vec3(0);
	speed_ = speed;
	hitbox_ =  Hitbox(&position_, scale_*0.6);
	mass_ = mass;

	explosion_effect_ = EFFECT_EXPLOSION;
}

void Entity::Update(float dt){

	//AIR RESISTANCE
	//Proptional to v^2 * some constants such as fluid density and area...
	//Force applied in the opposite direction to velocity
	float v = glm::length(velocity_);
	if(glm::length(velocity_) > 0)
		acceleration_ -= (glm::normalize(velocity_) * (v*v) * DRAG_CONSTANT);

	//Velocity is accumulated while acceleration denotes our rate of change of velocity
	//at this exact time step. acceleration = dv/dt 
	velocity_ += (acceleration_ * dt);
	position_ += velocity_;
	acceleration_ = glm::vec3(0);
	
	//Update all children
	for(int i = 0; i < children_.size(); i++){
		children_[i]->Update(dt);
	}
}

void Entity::SetEffect(GLuint eff){
	explosion_effect_ = eff;
}

void Entity::AddWeapon(Weapon *wpn){
	wpn->Attach(&position_, &angle_);
	AddChild(wpn);
	weapons_.push_back(wpn);
}

void Entity::Thrust(float dt){
	acceleration_ = GetAngleVector() * speed_ * (dt*50);
	//acceleration_ = glm::rotate(glm::vec3(0, speed_, 0), angle_, glm::vec3(0,0,1))*(dt*45);
}

void Entity::Break(){
	//Reduce acceleration and velocity
	acceleration_ *= 0.98;
}

void Entity::Shoot(){
	for(Weapon* w : weapons_){
		w->Shoot();
	}
}

void Entity::Turn(int d, float dt){
	if(d > 0){
		angle_ += (4.0f  *dt);
	}else {
		angle_ -= (4.0f * dt);
	}
}

void Entity::LookAtPoint(glm::vec2 target)
{
	float angle = glm::atan(target.y, target.x);
	angle_ = angle - glm::pi<float>() / 2;
}

void Entity::SetScale(float s){
	scale_ = s;
	hitbox_.SetRadius(s/2);
}

glm::vec3& Entity::GetVelocity() {
	return velocity_;
}

float Entity::GetMass(){
	return mass_;
}

void Entity::SetVelocity(const glm::vec3 &v){
	velocity_ = v;
}

void Entity::SetPosition(const glm::vec3 &p){
	position_ = p;
}

Hitbox& Entity::GetHitbox(){
	return hitbox_;
}

bool Entity::CheckCollision(Hitbox& hbox){
	return hitbox_.CheckCollision(hbox);
}

void Entity::Explode(){
	if(!CheckShield()){
		game_->SpawnExplosion(explosion_effect_, position_, scale_);
		destroyed_ = true;
	}
}

bool Entity::CheckShield(){
	for(int i = 0; i < children_.size(); i++){
		if(children_[i]->GetType() == SHIELD){
			delete children_[i];
			children_.erase(children_.begin() + i);
			return true;
		}
	}
	return false;
}

void Entity::PowerUp(){
	draw_inverted_ = true;
	if(!weapons_.empty()){
		Weapon* w = weapons_[0];
		w->AddShootingAngle(M_PI/8);
		w->AddShootingAngle(-M_PI/8);
	}
}

