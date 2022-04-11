#include "collectibles.hh"
#include "items.hh"
#include "game.hh"

DashCollectible::DashCollectible(GLuint type, const glm::vec3& position, Game* game)
	: Collectible(type, position, game) {}

void DashCollectible::HandleCollision(Entity *player){
    if(game_->GetPlayer()->RaiseEvent("has_dash")) {
        Item* i = new DashBuff(BUFF, glm::vec3(0.8,0,0), game_);
        i->SetScale(0.2f);
        player->AddItem(i);
    } else {
        player->AddItem(new Dash(DASH, glm::vec3(1.1,0,0), game_));
    }	
	SetDestroyed();
}