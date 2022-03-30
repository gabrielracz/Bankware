#include "effect.hh"

Effect::Effect(GLuint texture, const glm::vec3& position, Game* game, float lifetime)
	: GameObject(texture, position, game), lifetime_(lifetime) {}

void Effect::Update(float dt){
	lifetime_ -= dt;

	if(lifetime_ <= 0)
		SetDestroyed();
}
