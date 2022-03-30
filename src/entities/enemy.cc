#include "enemy.hh"
#include "game.hh"
#include <iostream>

Enemy::Enemy(GLuint type, const glm::vec3& position, const glm::vec3& origin, Game* game, float speed)
: Entity(type, position, game, speed){
	origin_ = origin;
	state_ = PATROLLING;
	path_tracker_ = 0;
	path_radius_ = (position_ - origin_).length();
	
}

void Enemy::Update(float dt){
	path_tracker_ = fmod(path_tracker_ + 0.02, 3.1415*2);
	glm::vec3 player_position = game_->GetPlayerPosition();

	float distance = glm::distance(position_, player_position);
	if(distance < 6.5){
		state_ = HUNTING;
	}else if(state_ == HUNTING && distance > 20){
		path_tracker_ = 0;
		origin_ = (position_ - glm::vec3(path_radius_, 0, 0));
		state_ = PATROLLING;
	}
	switch(state_){
		case PATROLLING: {
			glm::vec3 radius = (origin_ - position_);
			float r = radius.length();
			position_ = glm::vec3(r*cos(path_tracker_) + origin_.x, r*sin(path_tracker_) + origin_.y, 0);
			angle_ = fmod(path_tracker_, M_PI*2);
			break;
		}
		case HUNTING: {
			//Get perpendicular vector to facing direction
			float perp_angle = angle_ + M_PI/2 - 0.15;
			glm::vec3 perp = glm::rotate(glm::vec3(0,1,0), perp_angle, glm::vec3(0,0,1));
			glm::vec3 player_position = game_->GetPlayerPosition() - position_;
			float dp = glm::dot(perp, player_position);
			Turn(dp, dt);

			float angle_to_player = glm::acos(dp / GetAngleVector().length() * player_position.length());
			if(glm::abs(angle_to_player) < M_PI/4){
				Thrust(dt);
			}else{
				Break();
			}

			Entity::Update(dt);
			break;
		}
	}
}
