#ifndef SHIELD_H
#define SHIELD_H
#include "game_object.hh"

class Shield : public GameObject {
private:
	float path_tracker_;
	float path_radius_;
	glm::vec3 origin_;
public:
	Shield(GLuint type, const glm::vec3& position, const glm::vec3& origin, Game* game, float path_start = 0.0f);
	void Update(float dt) override;
};

#endif
