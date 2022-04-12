#include "collectibles.hh"
#include "items.hh"
#include "game.hh"

RedKeyCollectible::RedKeyCollectible(GLuint type, const glm::vec3& position, Game* game)
	: Collectible(type, position, game) {}

void RedKeyCollectible::HandleCollision(Entity *player){
	Item* key = new RedKey(REDKEY, glm::vec3(2.5,0,0), game_);
    key->SetScale(0.5f);
	game_->SpawnNotificiation("Found Red Key!");
	player->AddItem(key);
	SetDestroyed();
}

BlueKeyCollectible::BlueKeyCollectible(GLuint type, const glm::vec3& position, Game* game)
	: Collectible(type, position, game) {}

void BlueKeyCollectible::HandleCollision(Entity *player){
	Item* key = new BlueKey(BLUEKEY, glm::vec3(2.5,0,0), game_);
    key->SetScale(0.5f);
	game_->SpawnNotificiation("Found Blue Key!");
	player->AddItem(key);
	SetDestroyed();
}

YellowKeyCollectible::YellowKeyCollectible(GLuint type, const glm::vec3& position, Game* game)
	: Collectible(type, position, game) {}

void YellowKeyCollectible::HandleCollision(Entity *player){
	Item* key = new YellowKey(YELLOWKEY, glm::vec3(2.5,0,0), game_);
    key->SetScale(0.5f);
	game_->SpawnNotificiation("Found Yellow Key!");
	player->AddItem(key);
	SetDestroyed();
}