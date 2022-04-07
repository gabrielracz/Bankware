#include "buoy.hh"
//#include <cmath>
#include <math.h>


Buoy::Buoy(GLuint type, const glm::vec3& position, Game* game, float mass)
	: Entity(type, position, game, 0, mass) {
	//Adjust scale to mass
	scale_ = log(mass_+1) + 1;
	hitbox_.SetRadius(scale_/4);
}
