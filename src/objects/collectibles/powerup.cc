#include "powerup.hh"
#include "game.hh"

Powerup::Powerup(GLuint type, const glm::vec3& position, Game* game)
	: Collectible(type, position, game) {
		time_ = 2.5f;
	}

void Powerup::HandleCollision(Entity *player){
	//give the player a shield
	game_->SpawnNotificiation("POWER!");
	player->PowerUp(time_);
	SetDestroyed();
}
