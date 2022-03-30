#include "entity.hh"

#ifndef PLAYER_H
#define PLAYER_H

class Player : public Entity {
protected:

public:
	Player(GLuint texture, glm::vec3 position);

	void Update(float dt);
	void Turn(int d);
	void Thrust(); 
};
#endif
