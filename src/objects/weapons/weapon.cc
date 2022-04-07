#include "weapon.hh"
#include "game_object.hh"
#include "game.hh"
//#include <cmath>
#include <math.h>
#include <glm/fwd.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

Weapon::Weapon(GLuint weapon_type, GLuint projectile_type, const glm::vec3& position, Game* game, glm::vec3* parent_position, float* parent_angle, float cooldown)
	:GameObject(weapon_type, position, game){

	parent_position_ = parent_position;
	parent_angle_ = parent_angle;
	shot_base_cooldown_ = cooldown;
	shot_cooldown_ = shot_base_cooldown_;
	shot_timer_ = shot_cooldown_;
	projectile_type_ = projectile_type;
	shooting_angles_.push_back(angle_);
	aimable_ = false;

}

void Weapon::Update(float delta_time){
	shot_timer_ -= delta_time;
	if(shot_timer_ < 0)
		shot_timer_ = 0;
}

void Weapon::Shoot(){
	if(shot_timer_ > 0) return;
	
	for(float a : shooting_angles_){
		glm::vec3 rotated_pos = glm::rotate(position_, *parent_angle_, glm::vec3(0,0,1));   //This rotate is ok
		game_->SpawnBullet(projectile_type_, *parent_position_ + rotated_pos, *parent_angle_ + a);
	}

	shot_timer_ = shot_cooldown_;
}

void Weapon::Attach(glm::vec3 *p_position, float *p_angle){
	parent_position_ = p_position;
	parent_angle_ = p_angle;
}

void Weapon::SetAngle(float a){
	//Adjust shooting angles to be relative to the new angle
	for(int i = 0; i < shooting_angles_.size(); i++){
		shooting_angles_[i] = a + shooting_angles_[i] - angle_;
	}
	angle_ = a;
}

void Weapon::AimAt(glm::vec3 &target){
	if(!aimable_) return;
	glm::vec3 rotated_pos = glm::rotate(position_, *parent_angle_, glm::vec3(0,0,1));
	glm::vec3 world_pos = *parent_position_ + rotated_pos;
	float angle = glm::atan(target.y - world_pos.y, target.x - world_pos.x);
	SetAngle(angle - *parent_angle_ - glm::pi<float>()/2);
}

void Weapon::AddShootingAngle(float a){
	shooting_angles_.push_back(angle_ + a);
}

void Weapon::SetAimable(bool t){
	aimable_ = t;
}
