#include "gate.hh"
#include "game.hh"

Gate::Gate(GLuint texture, const glm::vec3 &position, const glm::vec3 &origin, Game *game) : Entity(texture, position, game)
{
}

void Gate::Explode()
{
    Entity *player = game_->GetPlayer();
    if (player->RaiseEvent("red") && player->RaiseEvent("blue") && player->RaiseEvent("yellow"))
    {
        player->RaiseEvent("delete_keys");
        game_->SpawnExplosion(explosion_effect_, position_, scale_);
        game_->WinGame();
        destroyed_ = true;
    }
}