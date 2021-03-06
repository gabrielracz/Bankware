#include "items.hh"
#include "game.hh"

DashBuff::DashBuff(GLuint type, const glm::vec3 &position, Game *game) : Item(type, position, game)
{
	
}
bool DashBuff::Event(const std::string& event)
{
    if(event == "damage" || event == "dash_block") {
        Deactivate();
        return true;
    }
    return false;
}

void DashBuff::Activate()
{
	//printf("Dash UP\n");
	game_->SpawnNotificiation("+Dash Power");
    game_->GetPlayer()->RaiseEvent("buff_dash");
}

void DashBuff::Deactivate()
{
	//printf("Dash DOWN\n");
	game_->SpawnNotificiation("-Dash Power");
    game_->GetPlayer()->RaiseEvent("debuff_dash");
	Item::Deactivate();
}
