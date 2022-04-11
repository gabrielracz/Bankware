#include "powerup.hh"

Powerup::Powerup(GLuint type, const glm::vec3& position, Game* game)
	: Collectible(type, position, game) {}

void Powerup::HandleCollision(Entity *player){
	//give the player a shield
	player->PowerUp();
	SetDestroyed();
}
