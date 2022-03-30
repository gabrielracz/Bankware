#include "hitbox.hh"
#include <iostream>

Hitbox::Hitbox(){
	origin_ = nullptr;
	radius_ = 0;
}

Hitbox::Hitbox(glm::vec3* origin, float radius){
	origin_ = origin;
	radius_ = radius;
}

bool Hitbox::CheckCollision(Hitbox& h){
	float distance = glm::distance(*origin_, *(h.origin_));
	return distance < (radius_ + h.GetRadius());
}

void Hitbox::SetRadius(float r){
	radius_ = r;
}

glm::vec3* Hitbox::GetOrigin(){
	return origin_;
}

float Hitbox::GetRadius(){
	return radius_;
}
