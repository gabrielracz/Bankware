#include "items.hh"
#include "game.hh"

RateOfFire::RateOfFire(GLuint type, const glm::vec3 &position, Game *game) : Item(type, position, game), reduce_amount_(0.02f)
{
}

void RateOfFire::Activate()
{
	printf("Rate of fire UP\n");
	game_->GetPlayer()->ReduceWeaponCooldown(reduce_amount_);
}

void RateOfFire::Deactivate()
{
	printf("Rate of fire DOWN\n");
	game_->GetPlayer()->ReduceWeaponCooldown(-reduce_amount_);
	Item::Deactivate();
}