#include "item.hh"

Item::Item(GLuint type, const glm::vec3& position, Game* game) : GameObject(type, position, game)
{
    scale_ = 0.4;
	path_radius_ = glm::length(position - glm::vec3(0));
}

void Item::Update(float dt)
{
    path_tracker_ = fmod(path_tracker_ + dt*2, 3.1415*2);
	float r = path_radius_;
	position_ = glm::vec3(r*cos(-path_tracker_) + origin_.x, r*sin(-path_tracker_) + origin_.y, 0);
	angle_ = fmod(path_tracker_, M_PI*2);
}