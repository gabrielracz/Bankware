#include "collectible.hh"
#include "game_object.hh"


Collectible::Collectible(GLuint type, const glm::vec3& position, Game* game)
	: GameObject(type, position, game) {
	hitbox_ = Hitbox(&position_, scale_-0.2);
}

void Collectible::HandleCollision(Entity* player){
	//Do whatever you want to the player
}
