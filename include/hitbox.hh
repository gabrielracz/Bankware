#ifndef HITBOX_H
#define HITBOX_H
#include<glm/glm.hpp>

class Hitbox {
private:
	glm::vec3* origin_;
	float radius_;
public:
	Hitbox();
	Hitbox(glm::vec3* origin, float radius);
	bool CheckCollision(Hitbox& h);
	void SetRadius(float r);
	glm::vec3* GetOrigin();
	float GetRadius();
};

#endif
