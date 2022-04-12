#include "collectibles.hh"
#include "weapon.hh"
#include "game.hh"

WeaponCollectible::WeaponCollectible(GLuint type, const glm::vec3& position, Game* game)
	: Collectible(type, position, game) {}

void WeaponCollectible::HandleCollision(Entity *player){
    if(player->HasWeapon("pea")) {
        player->GetWeapon("pea")->ReduceCooldown(0.05f);
        SetDestroyed();
        game_->SpawnNotificiation("+Weapon Upgrade");
        return;
    }
    game_->SpawnNotificiation("+Weapon");
    Weapon *w = new Weapon(CANNON, PROJECTILE_BULLET, glm::vec3(0, 0.5, 0), game_, 0.8f, "pea");
    w->SetAimable(true);
    player->AddWeapon(w);
    SetDestroyed();
}
