#include "collectibles.hh"
#include "weapon.hh"
#include "game.hh"

WeaponCollectible::WeaponCollectible(GLuint type, const glm::vec3& position, Game* game)
	: Collectible(type, position, game) {}

void WeaponCollectible::HandleCollision(Entity *player){
	game_->SpawnNotificiation("+Weapon");
}
