#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_

#include "game_object.hh"
#include "entity.hh"

class ParticleSystem : public GameObject {
public:
	ParticleSystem(GLuint texture, const glm::vec3 &position, Entity* parent, Game* game);
	void Update(float delta_time) override;
private:
	Entity* parent_;
	float max_scale_;

}; // class ParticleSystem

#endif // PARTICLE_SYSTEM_H_
