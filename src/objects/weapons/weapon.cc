#include "weapon.hh"
#include "game_object.hh"
#include "game.hh"
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

}

void Weapon::Update(float delta_time){
	shot_timer_ -= delta_time;
	if(shot_timer_ < 0)
		shot_timer_ = 0;
}

void Weapon::Shoot(){
	if(shot_timer_ > 0) return;

	for(float a : shooting_angles_){
		game_->SpawnBullet(projectile_type_, *parent_position_ + position_, *parent_angle_ + a);
	}

	shot_timer_ = shot_cooldown_;
}

void Weapon::Attach(glm::vec3 *p_position, float *p_angle){
	parent_position_ = p_position;
	parent_angle_ = p_angle;
}

void Weapon::AddShootingAngle(float a){
	shooting_angles_.push_back(angle_ + a);
}
