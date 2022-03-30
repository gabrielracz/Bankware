#ifndef BLADE_H
#define BLADE_H
#include "entity.hh"
#include "game_object.hh"

class Blade : public GameObject {
private:
	float path_tracker_;
	float path_radius_;
public:
	Blade(GLuint type, const glm::vec3& position, Game* game);
	void Update(float dt) override;
};

#endif
