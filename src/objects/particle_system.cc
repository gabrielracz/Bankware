#include "particle_system.hh"
#include <iostream>

ParticleSystem::ParticleSystem(GLuint texture, const glm::vec3 &position, Entity* parent, Game* game)
	: GameObject(texture, position, game){
	parent_ = parent;
	max_scale_ = 0.4;
	scale_ = 0;
}


void ParticleSystem::Update(float delta_time) {
	if(parent_->GetThrust()){
		if(scale_ < max_scale_)
			scale_ += 0.018;
		//std::cout << scale_ << std::endl;
	}else{
		if(scale_ > 0)
			scale_ -= 0.08;
		else
			scale_ = 0;
	}
	// Call the parent's update method to move the object in standard way, if desired
	GameObject::Update(delta_time);
}

