#ifndef SHOTGUNNER_H
#define SHOTGUNNER_H
#include "enemy.hh"
#include <random>

class Shotgunner : public Enemy
{
private:
    int spread_amount_;
    float recoil_force_;
    float shot_cooldown_counter_;
    float shot_cooldown_;
public:
    Shotgunner(GLuint texture, const glm::vec3 &position, const glm::vec3 &origin, Game *game, float speed = 0.5);
    virtual void Update(float dt) override;
    virtual void Shoot() override;

};
#endif // SHOTGUNNER_H