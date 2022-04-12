#include "satellite.hh"
#include "game.hh"

Satellite::Satellite(GLuint type, const glm::vec3 &position, Game *game, float mass) : Buoy(type, position, game, mass)
{
    int angles = ceil(log(mass_ + 3) + 1);
    //Just a transparent image
    Weapon* w = new Weapon(NONE, ENEMY_BULLET, glm::vec3(0), game);
    w->ClearWeapon();
    for(int i = 0; i < angles; ++i) {
        w->AddShootingAngle(2*i*glm::pi<float>() / angles);
    }
    AddWeapon(w);
    shot_cooldown_ = 1;
    shot_cooldown_counter_ = shot_cooldown_;
}
void Satellite::Update(float dt)
{
    if (shot_cooldown_counter_ > 0)
	{
		shot_cooldown_counter_  -= dt;
		if (shot_cooldown_counter_  < 0)
		{
			shot_cooldown_counter_ = shot_cooldown_;
            Shoot();
		}
	}
    Entity::Update(dt);
}