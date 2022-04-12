#include "collectibles.hh"
#include "weapon.hh"
#include "game.hh"

WeaponCollectible::WeaponCollectible(GLuint type, const glm::vec3& position, Game* game)
	: Collectible(type, position, game) {}

void WeaponCollectible::HandleCollision(Entity *player){
	game_->SpawnNotificiation("+Weapon");
    Weapon *w = new Weapon(CANNON, PROJECTILE_BULLET, glm::vec3(0, 0.5, 0), game_, 0.8f);
    w->SetAimable(true);
    player->AddWeapon(w);
    SetDestroyed();
}
