#include "items.hh"
#include "game.hh"

Dash::Dash(GLuint type, const glm::vec3 &position, Game *game) : Item(type, position, game)
{
    dash_power_ = 0.6f;
    dash_power_increase_ = 0.02f;
    dash_cooldown_ = 2;
    dash_cooldown_decrease_ = 0.02f;
    time_since_last_dash_ = dash_cooldown_ + 1;
}

void Dash::Update(float dt)
{
    Item::Update(dt);
    if(time_since_last_dash_ <= dash_cooldown_) {
        time_since_last_dash_ += dt;
    }
}

bool Dash::Event(const std::string &event)
{
    // Switch statement would prob be better
    if (event == "damage")
    {
        if (game_->GetPlayer()->RaiseEvent("dash_block"))
        {
            return false;
        }
        Deactivate();
        return true;
    }
    if (event == "has_dash")
    {
        return true;
    }
    if (event == "buff_dash")
    {
        dash_power_ += dash_power_increase_;
        dash_cooldown_ -= dash_cooldown_decrease_;
        return true;
    }
    if (event == "debuff_dash")
    {
        dash_power_ -= dash_power_increase_;
        dash_cooldown_ += dash_cooldown_decrease_;
        return true;
    }
    if (event == "dash")
    {
        if (dash_cooldown_ < time_since_last_dash_)
        {
            printf("DASH\n");
            time_since_last_dash_ = 0;
            game_->PlayerThrust(dash_power_);
            return true;
        }
    }
    return false;
}