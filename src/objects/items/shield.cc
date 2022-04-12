#include "items.hh"
#include "game.hh"
Shield::Shield(GLuint type, const glm::vec3& position, Game* game) : Item(type, position, game)
{
	
}
bool Shield::Event(const std::string& event)
{
	if(event == "block") {
		game_->SpawnNotificiation("Block");
        SetDestroyed();
        return true;
    }
    return false;
}
