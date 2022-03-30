#include"shield.hh"
#include<iostream>

Shield::Shield(GLuint type, const glm::vec3& position, const glm::vec3& origin, Game* game, float path_start)
	: GameObject(type, position, game){
	origin_ = origin;
	path_radius_ = glm::length(position - origin);
	path_tracker_ = path_start;
	scale_ = 0.4;
	//initial position of player;
	Update(0);
}

void Shield::Update(float dt){
	path_tracker_ = fmod(path_tracker_ + dt*2, 3.1415*2);
	float r = path_radius_;
	position_ = glm::vec3(r*cos(-path_tracker_) + origin_.x, r*sin(-path_tracker_) + origin_.y, 0);
	angle_ = fmod(path_tracker_, M_PI*2);
}

