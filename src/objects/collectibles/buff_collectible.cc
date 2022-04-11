#include "collectibles.hh"
#include "items.hh"
#include <random>

BuffCollectible::BuffCollectible(GLuint type, const glm::vec3& position, Game* game)
	: Collectible(type, position, game) {}

void BuffCollectible::HandleCollision(Entity *player){
	//give the player a shield
	std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(0, 1);
	Item* i;
	if(distr(eng) > 0.5) {
		i = new SpeedUp(BUFF, glm::vec3(0.8,0,0), game_);
	} else {
		i = new RateOfFire(BUFF, glm::vec3(0.8,0,0), game_);
	}
	
	i->SetScale(0.2f);
	player->AddItem(i);
	SetDestroyed();
}
