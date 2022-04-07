#include"blade.hh"
#include<iostream>

Blade::Blade(GLuint type, const glm::vec3& position, Game* game)
	: GameObject(type, position, game){
	path_tracker_ = 0;
	scale_ = 1.5;
}

void Blade::Update(float dt){
	path_tracker_ = fmod(path_tracker_ + dt*10, 3.1415*2);
	angle_ = fmod(path_tracker_, glm::pi<float>()*2);
}

