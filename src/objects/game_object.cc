#include "game_object.hh"
#include "game.hh"
#include <cmath>
#include <iostream>

GameObject::GameObject(GLuint type, const glm::vec3 &position, Game* game){
	game_ = game;
	position_ = position;
	type_ = type;
	scale_ = 1.0f;
	angle_ = 0.0f;
	destroyed_ = false;
	draw_inverted_ = false;
}

GameObject::~GameObject(){
	for(auto child : children_){
		delete child;
	}
}

void GameObject::Update(float dt){
	//Do nothing
}

glm::vec3 GameObject::GetAngleVector(){
	double a = angle_ + M_PI_2;
	return glm::vec3(cos(a), sin(a), 0);
}

void GameObject::AddChild(GameObject* obj){
	children_.push_back(obj);
}


void GameObject::SetDestroyed(){
	destroyed_ = true;
	for(auto child : children_){
		child->SetDestroyed();
	}
}

std::vector<GameObject*>& GameObject::GetChildren(){
	return children_;
}

