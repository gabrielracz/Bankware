#ifndef BUOY_H
#define BUOY_H
#include "entity.hh"

class Buoy : public Entity {

public:
	Buoy(GLuint type, const glm::vec3& position, Game* game, float mass = 1.0f);
};

#endif
