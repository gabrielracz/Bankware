#include "shield_collectible.hh"
#include "shield.hh"

ShieldCollectible::ShieldCollectible(GLuint type, const glm::vec3& position, Game* game)
	: Collectible(type, position, game) {}

void ShieldCollectible::HandleCollision(Entity *player){
	//give the player a shield
	player->AddChild(new Shield(SHIELD, glm::vec3(2,0,0), glm::vec3(0, 0, 0), game_));
	SetDestroyed();
}
