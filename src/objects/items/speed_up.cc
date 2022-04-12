#include "items.hh"
#include "game.hh"

SpeedUp::SpeedUp(GLuint type, const glm::vec3 &position, Game *game) : Item(type, position, game), speed_buff_amount_(0.05f)
{
}

void SpeedUp::Activate()
{
	//printf("Speed UP\n");
	game_->SpawnNotificiation("+Speed");
	game_->GetPlayer()->SetSpeed(game_->GetPlayer()->GetSpeed() + speed_buff_amount_);
}

void SpeedUp::Deactivate()
{
	//printf("Speed DOWN\n");
	game_->SpawnNotificiation("-Speed");
	game_->GetPlayer()->SetSpeed(game_->GetPlayer()->GetSpeed() - speed_buff_amount_);
	Item::Deactivate();
}
