#ifndef WEAPON_H
#define WEAPON_H
#include "game_object.hh"

class Weapon : public GameObject {
protected:
	float* parent_angle_;   //Need to account for parent angle plus my angle
	glm::vec3* parent_position_;
	std::vector<float> shooting_angles_;  //In case we want it to shoot multiple angles

	float shot_base_cooldown_;
	float shot_cooldown_;
	float shot_timer_;

	bool aimable_;

	GLuint projectile_type_;
public:
	Weapon(GLuint weapon_type, GLuint projectile_type, const glm::vec3& position, Game* game, glm::vec3* parent_position = nullptr, float* parent_angle = nullptr, float cooldown = 0.1f);
	virtual void Update(float delta_time) override;
	virtual void Shoot();

	void SetAngle(float angle);
	void AimAt(glm::vec3& target);
	void Attach(glm::vec3* p_position, float* p_angle);
	void AddShootingAngle(float angle);
	void ResetShootingAngles();
	void SetCooldown(float cd);
	void ReduceCooldown(float cd);
	void ResetCooldown();
	void SetAimable(bool t);
	inline void ClearWeapon() { shooting_angles_.clear(); }
};

#endif
