#ifndef GAME_H
#define GAME_H

#include<vector>
#include<glm/glm.hpp>
#include <GL/glew.h>
#include "game_object.hh"
#include "entity.hh"
#include "enemy.hh"
#include "projectile.hh"
#include "effect.hh"
#include "collectible.hh"
#include "text_object.hh"

#define NUM_EFFECTS 10
#define NUM_PROJECTILES 10

enum EffectTypes {
	EFFECT_EXPLOSION = 0
};

enum ProjectileTypes {
	PROJECTILE_BULLET = 0,
	ENEMY_BULLET
};

class Game {
private:
	std::vector<GameObject*> backgrounds_;
	std::vector<Entity*> entities_;
	std::vector<Projectile*> projectiles_;
	std::vector<Effect*> effects_;
	std::vector<Collectible*> collectibles_;
	std::vector<TextObject*> notifications_;
	Entity* player_;

	int world_width_;
	int world_height_;
	Effect* effect_prototypes_[NUM_EFFECTS];
	Projectile* projectile_prototypes_[NUM_PROJECTILES];
	Projectile* enemy_projectile_prototypes_[NUM_PROJECTILES];

	void CheckPlayerInBorder();
public:
	Game(int world_width, int world_height);
	~Game();

	int Init();
	void InitPrototypes();
	void Update(float dt);
	bool CheckCollisions(Entity* ent, int j);
	void CheckCollectibleCollision(Collectible* col);
	bool CheckProjectileCollisions(Projectile* prj, float dt);
	//void HandleCollision(Entity* ent);
	void HandleEntityCollision(Entity* e1, Entity* e2);


	//static bool CompareEntityPositions(Entity* e1, Entity* e2);
	void SweepAndPruneEntityCollisions();
	void SweepAndPruneProjectileCollisions();
	void RemoveDestroyedObjects();

	void SpawnBullet(GLuint projectile_index, glm::vec3& position, Hitbox* instigator, float angle);
	void SpawnExplosion(GLuint effect_index, glm::vec3 position, float scale);
	void SpawnNotificiation(const std::string& text);

	void PlayerThrust(float dt);
	void PlayerTurn(int t, float dt);
	void PlayerBreak();
	void PlayerShoot();
	void PlayerLook(glm::vec3& v, float dt);
	void PlayerLook(glm::vec3& v);

	Entity* GetPlayer();
	glm::vec3& GetPlayerPosition();
	const std::vector<Entity*>& GetEntities();
	const std::vector<GameObject*>& GetBackgrounds();
	const std::vector<Projectile*>& GetProjectiles();
	const std::vector<Effect*>& GetEffects();
	const std::vector<Collectible*>& GetCollectibles();
	const std::vector<TextObject*>& GetNotifications();
};
#endif
