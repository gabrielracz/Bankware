#ifndef SATELLITE_H
#define SATELLITE_H
#include "buoy.hh"

class Satellite : public Buoy
{
private:
    float shot_cooldown_counter_;
    float shot_cooldown_;
public:
    Satellite(GLuint type, const glm::vec3 &position, Game *game, float mass = 1.0f);
    virtual void Update(float dt) override;
};

#endif // SATELLITE_H