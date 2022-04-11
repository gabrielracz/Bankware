#include "shield.hh"
Shield::Shield(GLuint type, const glm::vec3& position, Game* game) : Item(type, position, game)
{
	
}
bool Shield::Event(const std::string& event)
{
	if(event == "block") {
        SetDestroyed();
        return true;
    }
    return false;
}