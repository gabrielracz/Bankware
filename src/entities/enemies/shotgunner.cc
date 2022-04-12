#include "shotgunner.hh"
#include "game.hh"

Shotgunner::Shotgunner(GLuint texture, const glm::vec3 &position, const glm::vec3 &origin, Game *game, float speed) : Enemy(texture, position, origin, game, speed)
{
    spread_amount_ = 2;
    recoil_force_ = 9.0f;
    Weapon *w = new Weapon(NONE, ENEMY_BULLET, glm::vec3(0,0.4,0), game);
	w->SetScale(.5f);
    for (int i = 1; i < spread_amount_; ++i)
    {
        w->AddShootingAngle(i * glm::pi<float>() / 12);
        w->AddShootingAngle(-i * glm::pi<float>() / 12);
    }
    AddWeapon(w);
    shot_cooldown_ = 1;
    shot_cooldown_counter_ = shot_cooldown_;
    health_ = 1;
    iframe_time_ = 0.5;
}

void Shotgunner::Update(float dt)
{
    path_tracker_ = fmod(path_tracker_ + 0.02, 3.1415 * 2);
    glm::vec3 player_position = game_->GetPlayerPosition();

    float distance = glm::distance(position_, player_position);
    if (distance < 8)
    {
        state_ = HUNTING;
    }
    else if (state_ == HUNTING && distance > 20)
    {
        path_tracker_ = 0;
        origin_ = (position_ - glm::vec3(path_radius_, 0, 0));
        state_ = PATROLLING;
    }
    switch (state_)
    {
    case PATROLLING:
    {
        glm::vec3 radius = (origin_ - position_);
        float r = radius.length();
        position_ = glm::vec3(r * cos(path_tracker_) + origin_.x, r * sin(path_tracker_) + origin_.y, 0);
        angle_ = fmod(path_tracker_, glm::pi<float>() * 2);
        UpdateIFrames(dt);
        break;
    }
    case HUNTING:
    {
        // Get perpendicular vector to facing direction
        float perp_angle = angle_ + glm::pi<float>() / 2 - 0.15;
        glm::vec3 perp = glm::rotate(glm::vec3(0, 1, 0), perp_angle, glm::vec3(0, 0, 1));
        glm::vec3 player_position_delta = game_->GetPlayerPosition() - position_;
        float dp = glm::dot(perp, player_position_delta);
        Turn(dp, dt);

        float angle_to_player = glm::acos(dp / GetAngleVector().length() * player_position.length());
        if (distance < 4.5 && glm::abs(angle_to_player) < glm::pi<float>() / 4)
        {
            std::random_device rd;
            std::default_random_engine eng(rd());
            std::uniform_real_distribution<float> distr(0, 1);
            glm::vec2 force_dir = glm::normalize(glm::vec2(player_position_delta.y, -player_position_delta.x)) * 1.5f;
            if (distr(eng) > 0.8)
            {
                force_dir = glm::normalize(glm::vec2(-player_position_delta.y, player_position_delta.x)) * 1.5f;
            }
            acceleration_ = glm::vec3(force_dir.x, force_dir.y, position_.z);
            if (shot_cooldown_counter_ <= 0)
            {
                shot_cooldown_counter_ = shot_cooldown_;
                Shoot();
            }
        }
        else
        {
            if (glm::abs(angle_to_player) < glm::pi<float>() / 4)
            {
                Thrust(dt);
                if (shot_cooldown_counter_ <= 0)
                {
                    shot_cooldown_counter_ = shot_cooldown_;
                    Shoot();
                }
            }
            else
            {
                Break();
            }
        }

        Entity::Update(dt);
        break;
    }
    }
    if (shot_cooldown_counter_ > 0)
    {
        shot_cooldown_counter_ -= dt;
        if (shot_cooldown_counter_ < 0)
        {
            shot_cooldown_counter_ = 0;
        }
    }
}
void Shotgunner::Shoot()
{
    Entity::Shoot();
    velocity_ = glm::vec3(0);
    acceleration_ = GetAngleVector() * -recoil_force_;
}
