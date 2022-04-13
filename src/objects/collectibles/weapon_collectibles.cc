#include "collectibles.hh"
#include "weapon.hh"
#include "game.hh"

PeaShooterCollectible::PeaShooterCollectible(GLuint type, const glm::vec3 &position, Game *game)
    : Collectible(type, position, game) {}

void PeaShooterCollectible::HandleCollision(Entity *player)
{
    if (player->HasWeapon("pea"))
    {
        player->GetWeapon("pea")->ReduceCooldown(0.05f);
        game_->SpawnNotificiation("+Pea Shooter Upgrade");
        SetDestroyed();
        return;
    }
    game_->SpawnNotificiation("+Pea Shooter");
    Weapon *w = new Weapon(BULLET, PROJECTILE_BULLET, glm::vec3(0, 0.7, 0), game_, 0.5f, "pea");
    w->SetAimable(true);
	w->SetScale(0.6);
    player->AddWeapon(w);
    SetDestroyed();
}

DualCannonCollectible::DualCannonCollectible(GLuint type, const glm::vec3 &position, Game *game)
    : Collectible(type, position, game) {scale_ = 2;}

void DualCannonCollectible::HandleCollision(Entity *player)
{
    bool hasrc = player->HasWeapon("right_cannon");
    bool haslc = player->HasWeapon("left_cannon");
    if (hasrc && haslc)
    {
        player->GetWeapon("right_cannon")->ReduceCooldown(0.03f);
        player->GetWeapon("left_cannon")->ReduceCooldown(0.03f);
        game_->SpawnNotificiation("+Dual Cannon Upgrade");
        SetDestroyed();
        return;
    }
    else if (hasrc && !haslc)
    {
        Weapon *left_cannon = new Weapon(CANNON, PROJECTILE_BULLET, glm::vec3(-1, 0.5, 0), game_, 0.1, "left_cannon");
        left_cannon->SetAimable(true);
        left_cannon->SetScale(2);
        player->AddWeapon(left_cannon);
        game_->SpawnNotificiation("+Left Cannon");
        SetDestroyed();
        return;
    }
    else if (!hasrc && haslc)
    {
        Weapon *right_cannon = new Weapon(CANNON, PROJECTILE_BULLET, glm::vec3(1, 0.5, 0), game_, 0.1, "right_cannon");
        right_cannon->SetScale(2);
        right_cannon->SetAimable(true);
        player->AddWeapon(right_cannon);
        game_->SpawnNotificiation("+Right Cannon");
        SetDestroyed();
        return;
    }
    game_->SpawnNotificiation("+Dual Cannons");
    Weapon *right_cannon = new Weapon(CANNON, PROJECTILE_BULLET, glm::vec3(1, 0.5, 0), game_, 0.1, "right_cannon");
    right_cannon->SetScale(2);
    right_cannon->SetAimable(true);
    player->AddWeapon(right_cannon);
    Weapon *left_cannon = new Weapon(CANNON, PROJECTILE_BULLET, glm::vec3(-1, 0.5, 0), game_, 0.1, "left_cannon");
    left_cannon->SetAimable(true);
    left_cannon->SetScale(2);
    player->AddWeapon(left_cannon);
    SetDestroyed();
}

LeftCannonCollectible::LeftCannonCollectible(GLuint type, const glm::vec3 &position, Game *game)
    : Collectible(type, position, game) {scale_ = 2;}

void LeftCannonCollectible::HandleCollision(Entity *player)
{
    if (player->HasWeapon("left_cannon"))
    {
        player->GetWeapon("left_cannon")->ReduceCooldown(0.05f);
        game_->SpawnNotificiation("+Left Cannon Upgrade");
        SetDestroyed();
        return;
    }
    game_->SpawnNotificiation("+Left Cannon");
    Weapon *left_cannon = new Weapon(CANNON, PROJECTILE_BULLET, glm::vec3(-1, 0.5, 0), game_, 0.15, "left_cannon");
    left_cannon->SetAimable(true);
    //left_cannon->SetScale(2);
    player->AddWeapon(left_cannon);
    SetDestroyed();
}

RightCannonCollectible::RightCannonCollectible(GLuint type, const glm::vec3 &position, Game *game)
    : Collectible(type, position, game) {scale_ = 2;}

void RightCannonCollectible::HandleCollision(Entity *player)
{
    if (player->HasWeapon("right_cannon"))
    {
        player->GetWeapon("right_cannon")->ReduceCooldown(0.05f);
        game_->SpawnNotificiation("+Right Cannon Upgrade");
        SetDestroyed();
        return;
    }
    Weapon *right_cannon = new Weapon(CANNON, PROJECTILE_BULLET, glm::vec3(1, 0.5, 0), game_, 0.15, "right_cannon");
    //right_cannon->SetScale(2);
    right_cannon->SetAimable(true);
    player->AddWeapon(right_cannon);
    SetDestroyed();
}
