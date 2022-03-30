#ifndef EFFECT_H
#define EFFECT_H
#include "game_object.hh"

class Effect : public GameObject {
protected:
	float lifetime_;
public:
	Effect(GLuint texture, const glm::vec3& position, Game* game, float lifetime = 0.7f);

	void Update(float dt) override;
};

#endif
