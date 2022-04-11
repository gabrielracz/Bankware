#include "collectibles.hh"
#include "shield.hh"

ShieldCollectible::ShieldCollectible(GLuint type, const glm::vec3& position, Game* game)
	: Collectible(type, position, game) {}

void ShieldCollectible::HandleCollision(Entity *player){
	//give the player a shield
	player->AddItem(new Shield(SHIELD, glm::vec3(1.5,0,0), game_));
	SetDestroyed();
}
