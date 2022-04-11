#ifndef ITEMS_H
#define ITEMS_H
#include "item.hh"

class Shield : public Item
{
private:
	float path_radius_ = 2;

public:
	Shield(GLuint type, const glm::vec3 &position, Game *game);
	virtual bool Event(const std::string &event) override;
};

class SpeedUp : public Item
{
private:
	float speed_buff_amount_;

public:
	SpeedUp(GLuint type, const glm::vec3 &position, Game *game);
	virtual void Activate() override;
	virtual void Deactivate() override;
};

class Dash : public Item
{
private:
	float dash_power_;
	float dash_cooldown_;
	float dash_power_increase_;
	float dash_cooldown_decrease_;
	float time_since_last_dash_;

public:
	Dash(GLuint type, const glm::vec3 &position, Game *game);
	virtual void Update(float dt) override;
	virtual bool Event(const std::string &event) override;
};

class DashBuff : public Item
{
public:
	DashBuff(GLuint type, const glm::vec3 &position, Game *game);
	virtual bool Event(const std::string &event) override;
	virtual void Activate() override;
	virtual void Deactivate() override;
};

class RateOfFire : public Item
{
private:
	float reduce_amount_;

public:
	RateOfFire(GLuint type, const glm::vec3 &position, Game *game);
	virtual void Activate() override;
	virtual void Deactivate() override;
};
#endif // ITEMS_H