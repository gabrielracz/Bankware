#include "items.hh"
RedKey::RedKey(GLuint type, const glm::vec3 &position, Game *game) : Item(type, position, game)
{
}
bool RedKey::Event(const std::string &event)
{
    if (event == "delete_keys")
    {
        Deactivate();
    }
    if (event == "red")
    {
        return true;
    }
    return false;
}

BlueKey::BlueKey(GLuint type, const glm::vec3 &position, Game *game) : Item(type, position, game)
{
}
bool BlueKey::Event(const std::string &event)
{
    if (event == "delete_keys")
    {
        Deactivate();
    }
    if (event == "blue")
    {
        return true;
    }
    return false;
}

YellowKey::YellowKey(GLuint type, const glm::vec3 &position, Game *game) : Item(type, position, game)
{
}
bool YellowKey::Event(const std::string &event)
{
    if (event == "delete_keys")
    {
        Deactivate();
    }
    if (event == "yellow")
    {
        return true;
    }
    return false;
}
