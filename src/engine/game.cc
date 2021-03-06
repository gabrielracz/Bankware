#include "game.hh"
#include "data.hh"
#include "effect.hh"
#include "collectible.hh"
#include "buoy.hh"
#include "entity.hh"
#include "particle_system.hh"
#include "projectile.hh"
#include "game_object.hh"
#include "blade.hh"
#include "powerup.hh"
#include "item.hh"
#include "items.hh"
#include "collectibles.hh"
#include "text_object.hh"
#include "weapon.hh"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <utility>
#include <vector>
#include <array>
#include "satellite.hh"
#include "shotgunner.hh"
#include "gate.hh"

Game::Game(int width, int height)
{
	world_width_ = width;
	world_height_ = height;
}

Game::~Game()
{
	for (auto ent : entities_)
	{
		delete ent;
	}
	for (auto bkg : backgrounds_)
	{
		delete bkg;
	}
	for (auto prj : projectiles_)
	{
		delete prj;
	}
	for (auto eff : effects_)
	{
		delete eff;
	}
}

int Game::InitArcade()
{
	game_over_ = false;
	InitPrototypes();
	// Tile the world
	float tile_size = 2;
	for (int y = 0; y < world_height_; y += tile_size)
	{
		for (int x = 0; x < world_width_; x += tile_size)
		{
			GameObject* bkg;
			if(x > world_width_*0.80 && y < world_height_*0.20){   //Bottom right corner
				//std::cout << x << world_width_/6 << std::endl;
				bkg = new GameObject(GRID, glm::vec3(x - world_width_ / 2, y - world_height_ / 2, 0), this);
			}else{
				int r = rand() % 4;
				GLuint bgs[] = {BACKGROUND1, BACKGROUND2, BACKGROUND3, BACKGROUND4};
				bkg = new GameObject(bgs[r], glm::vec3(x - world_width_ / 2, y - world_height_ / 2, 0), this);
			}
			bkg->SetScale(tile_size);
			bkg->SetInverted(true);
			backgrounds_.push_back(bkg);
		}
	}

	// Initialze player
	player_ = new Entity(SHIP, glm::vec3(0, 0, 0), this, 0.45);
	ParticleSystem *thrust = new ParticleSystem(FIRE, glm::vec3(0, -0.75, 0), player_, this);
	player_->AddChild(thrust);

	player_->AddItem(new Shield(SHIELD, glm::vec3(1.5, 0, 0), this));
	player_->AddItem(new Shield(SHIELD, glm::vec3(1.5, 0, 0), this));
	entities_.push_back(player_);

	// Initialize random enemies
	for (int i = 0; i < 250; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		entities_.emplace_back(new Enemy(ENEMY, glm::vec3(randx, randy, 0), glm::vec3(randx - rand() % 3, randy, 0), this));
	}

	for (int i = 0; i < 80; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		Shotgunner* s = new Shotgunner(GUNNER, glm::vec3(randx, randy, 0), glm::vec3(randx - rand() % 3, randy, 0), this);
		s->SetScale(2.2);
		entities_.push_back(s);
	}

	// Initialize Buoys
	int num_buoys = 50;
	float mass_values[] = {3, 8, 20};
	for (int i = 0; i < num_buoys; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		float randmass = mass_values[rand() % 3];
		Satellite* s = new Satellite(BUOY, glm::vec3(randx, randy, 0), this, randmass);
		entities_.push_back(s);
	}

	// Initialize Powerups
	for (int i = 0; i < 20; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		collectibles_.push_back(new Powerup(POWERUP, glm::vec3(randx, randy, 0), this));
	}

	for (int i = 0; i < 15; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		collectibles_.push_back(new DashCollectible(DASH, glm::vec3(randx, randy, 0), this));
	}

	for (int i = 0; i < 30; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		collectibles_.push_back(new BuffCollectible(BUFF, glm::vec3(randx, randy, 0), this));
	}

	for (int i = 0; i < 10; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		collectibles_.push_back(new PeaShooterCollectible(WEAPON_C, glm::vec3(randx, randy, 0), this));
	}

	for (int i = 0; i < 5; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		collectibles_.push_back(new LeftCannonCollectible(CANNON, glm::vec3(randx, randy, 0), this));
	}

	for (int i = 0; i < 5; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		collectibles_.push_back(new RightCannonCollectible(CANNON, glm::vec3(randx, randy, 0), this));
	}

	for (int i = 0; i < 1; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		collectibles_.push_back(new DualCannonCollectible(CANNON, glm::vec3(randx, randy, 0), this));
	}

	int num_shields = 25;
	for (int i = 0; i < num_shields; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		collectibles_.push_back(new ShieldCollectible(SHIELD, glm::vec3(randx, randy, 0), this));
	}

	return 0;
}

void Game::InitPrototypes(){
	//Init effects
	effect_prototypes_[EFFECT_EXPLOSION] = new Effect(PARTICLE_EXPLOSION, glm::vec3(0), this);
	effect_prototypes_[EFFECT_EXPLOSION]->SetScale(0.3f);

	// Init Projectiles
	projectile_prototypes_[PROJECTILE_BULLET] = new Projectile(BULLET, glm::vec3(0), 0, this);
	Projectile *p = new Projectile(E_BULLET, glm::vec3(0), 0, this, 10, 0.9, ENEMY_BULLET);
	p->SetScale(0.5f);
	enemy_projectile_prototypes_[ENEMY_BULLET] = p;
}

// Init the initial game state
int Game::Init()
{
	game_over_ = false;
	InitPrototypes();
	// Tile the world
	float tile_size = 2;
	int tiles_wide = world_width_ / tile_size;
	int tiles_high = world_height_ / tile_size;
	//MAP IS TILED FROM BOTTOM LEFT TO TOP RIGHT
	for (int y = -world_height_/2; y < world_height_/2; y += tile_size)
	{
		for (int x = -world_width_/2; x < world_width_/2; x += tile_size)
		{
			GameObject* bkg;
			if(x > 60 && y < -60){   //Bottom right corner
				bkg = new GameObject(GRID, glm::vec3(x, y, 0), this);
			}else if(x < -70 && y < -70){
				bkg = new GameObject(GRID, glm::vec3(x, y, 0), this);
			}else if(x < -60 && y > 40){
				bkg = new GameObject(GRID, glm::vec3(x, y, 0), this);
			}else{
				int r = rand() % 4;
				GLuint bgs[] = {BACKGROUND1, BACKGROUND2, BACKGROUND3, BACKGROUND4};
				bkg = new GameObject(bgs[r], glm::vec3(x, y, 0), this);
			}
			bkg->SetScale(tile_size);
			backgrounds_.push_back(bkg);
		}
	}
	std::random_device rd;
	std::default_random_engine eng(rd());
	
	std::uniform_real_distribution<float> rad(1, 3);
	std::uniform_real_distribution<float> z1_y(-100, -50);
	std::uniform_real_distribution<float> z1_x(50, 100);
	//Get value with distr(eng)
	
	//============ ZONE 1 ===========================
	for(int i = 0; i < 25; i++){
		float randy = z1_y(eng);
		float randx = z1_x(eng);
		entities_.emplace_back(new Enemy(ENEMY, glm::vec3(randx, randy , 0), glm::vec3(randx - rad(eng), randy, 0), this));
	}
		
	collectibles_.push_back(new PeaShooterCollectible(WEAPON_C, glm::vec3(80, -80, 0), this));

	collectibles_.push_back(new BuffCollectible(BUFF, glm::vec3(90, -92, 0), this));
	collectibles_.push_back(new BuffCollectible(BUFF, glm::vec3(90, -88, 0), this));
	collectibles_.push_back(new BuffCollectible(BUFF, glm::vec3(92, -90, 0), this));
	collectibles_.push_back(new BuffCollectible(BUFF, glm::vec3(88, -90, 0), this));
	collectibles_.push_back(new RedKeyCollectible(REDKEY, glm::vec3(90, -90, 0), this));
	
	//============ ZONE 2 =========================
	
	std::uniform_real_distribution<float> z2_y(-100, -50);
	std::uniform_real_distribution<float> z2_x(-100, -50);

	for(int i = 0; i < 30; i++){
		float randy = z2_y(eng);
		float randx = z2_x(eng);
		entities_.emplace_back(new Enemy(ENEMY, glm::vec3(randx, randy , 0), glm::vec3(randx - rad(eng), randy, 0), this));
	}

	entities_.push_back(new Satellite(BUOY, glm::vec3(-100, -72, 0), this, 3));
	entities_.push_back(new Satellite(BUOY, glm::vec3(-83, -72, 0), this, 8));
	entities_.push_back(new Satellite(BUOY, glm::vec3(-72, -72, 0), this, 3));
	entities_.push_back(new Satellite(BUOY, glm::vec3(-72, -84, 0), this, 8));
	entities_.push_back(new Satellite(BUOY, glm::vec3(-72, -100, 0), this, 3));
	entities_.push_back(new Satellite(BUOY, glm::vec3(-90, -90, 0), this, 2));
	entities_.push_back(new Satellite(BUOY, glm::vec3(-90, -76, 0), this, 2));
	entities_.push_back(new Satellite(BUOY, glm::vec3(-80, -80, 0), this, 2));
	collectibles_.push_back(new BlueKeyCollectible(BLUEKEY, glm::vec3(-98, -98, 0), this));
	collectibles_.push_back(new LeftCannonCollectible(CANNON, glm::vec3(-62, -87, 0), this));
	collectibles_.push_back(new LeftCannonCollectible(CANNON, glm::vec3(-84, -60, 0), this));
	collectibles_.push_back(new Powerup(POWERUP, glm::vec3(-99, -98, 0), this));

	//========== ZONE 3 ============================

	std::uniform_real_distribution<float> z3_y(50, 100);
	std::uniform_real_distribution<float> z3_x(-100, -50);
	
	for(int i = 0; i < 30; i++){
		float randy = z3_y(eng);
		float randx = z3_x(eng);
		entities_.emplace_back(new Enemy(ENEMY, glm::vec3(randx, randy , 0), glm::vec3(randx - rad(eng), randy, 0), this));
	}

	float mass_values[] = {3, 8, 20};
	for (int i = 0; i < 20; i++)
	{
		float randy = z3_y(eng);
		float randx = z3_x(eng);
		float randmass = mass_values[rand() % 3];
		Satellite* s = new Satellite(BUOY, glm::vec3(randx, randy, 0), this, randmass);
		entities_.push_back(s);
	}

	for (int i = 0; i < 12; i++)
	{
		float randy = z3_y(eng);
		float randx = z3_x(eng);
		Shotgunner* s = new Shotgunner(GUNNER, glm::vec3(randx, randy, 0), glm::vec3(randx - rand() % 3, randy, 0), this);
		s->SetScale(2.2);
		entities_.push_back(s);
	}

	collectibles_.push_back(new RightCannonCollectible(CANNON, glm::vec3(-67, 46, 0), this));
	collectibles_.push_back(new RightCannonCollectible(CANNON, glm::vec3(-94, 60, 0), this));
	collectibles_.push_back(new Powerup(POWERUP, glm::vec3(-91, 50, 0), this));
	collectibles_.push_back(new Powerup(POWERUP, glm::vec3(-84, 50, 0), this));
	collectibles_.push_back(new Powerup(POWERUP, glm::vec3(-80, 50, 0), this));
	collectibles_.push_back(new Powerup(POWERUP, glm::vec3(-60, 50, 0), this));
	collectibles_.push_back(new Powerup(POWERUP, glm::vec3(-35, 70, 0), this));
	collectibles_.push_back(new Powerup(POWERUP, glm::vec3(-35, 90, 0), this));
	collectibles_.push_back(new YellowKeyCollectible(YELLOWKEY, glm::vec3(-98, 98, 0), this));

	//FINAL SHOWDOWN

	std::uniform_real_distribution<float> z4_y(50, 100);
	std::uniform_real_distribution<float> z4_x(50, 100);
	
	for(int i = 0; i < 40; i++){
		float randy = z4_y(eng);
		float randx = z4_x(eng);
		entities_.emplace_back(new Enemy(ENEMY, glm::vec3(randx, randy , 0), glm::vec3(randx - rad(eng), randy, 0), this));
	}

	for (int i = 0; i < 30; i++)
	{
		float randy = z4_y(eng);
		float randx = z4_x(eng);
		float randmass = mass_values[rand() % 3];
		Satellite* s = new Satellite(BUOY, glm::vec3(randx, randy, 0), this, randmass);
		entities_.push_back(s);
	}

	for (int i = 0; i < 22; i++)
	{
		float randy = z4_y(eng);
		float randx = z4_x(eng);
		Shotgunner* s = new Shotgunner(GUNNER, glm::vec3(randx, randy, 0), glm::vec3(randx - rand() % 3, randy, 0), this);
		s->SetScale(2.2);
		entities_.push_back(s);
	}

	for (int i = 0; i < 15; i++)
	{
		float randy = z4_y(eng);
		float randx = z4_x(eng);
		collectibles_.push_back(new Powerup(POWERUP, glm::vec3(randx, randy, 0), this));
	}

	
	

	entities_.push_back(new Satellite(BUOY, glm::vec3(94, 98, 0), this, 12));
	entities_.push_back(new Satellite(BUOY, glm::vec3(94, 95, 0), this, 12));
	entities_.push_back(new Satellite(BUOY, glm::vec3(94, 92, 0), this, 12));
	entities_.push_back(new Satellite(BUOY, glm::vec3(94, 89, 0), this, 12));
	entities_.push_back(new Satellite(BUOY, glm::vec3(96, 89, 0), this, 12));
	entities_.push_back(new Satellite(BUOY, glm::vec3(98, 89, 0), this, 12));
	Gate* ToHeaven = new Gate(GATE_CLOSED, glm::vec3(97, 97, 0), this);
	ToHeaven->SetScale(4);
	entities_.push_back(ToHeaven);



	// Initialze player
	player_ = new Entity(SHIP, glm::vec3(0, 0, 0), this, 0.45);
	ParticleSystem *thrust = new ParticleSystem(FIRE, glm::vec3(0, -0.75, 0), player_, this);
	player_->AddChild(thrust);

	//Weapon *right_cannon = new Weapon(CANNON, PROJECTILE_BULLET, glm::vec3(1, 0.5, 0), this, 0.1);
	//right_cannon->SetScale(2);
	//right_cannon->SetAimable(true);
	//player_->AddWeapon(right_cannon);
	//Weapon *left_cannon = new Weapon(CANNON, PROJECTILE_BULLET, glm::vec3(-1, 0.5, 0), this, 0.1);
	//left_cannon->SetAimable(true);
	//left_cannon->SetScale(2);
	//player_->AddWeapon(left_cannon);
	// Weapon* booty_cannon = new Weapon(BULLET, PROJECTILE_BULLET, glm::vec3(0.0, -1, 0), this);
	// booty_cannon->SetAngle(glm::pi<float>());
	// player_->AddWeapon(booty_cannon);

	player_->AddItem(new Shield(SHIELD, glm::vec3(1.5, 0, 0), this));
	player_->AddItem(new Shield(SHIELD, glm::vec3(1.5, 0, 0), this));
	player_->AddItem(new Shield(SHIELD, glm::vec3(1.5, 0, 0), this));
	entities_.push_back(player_);

	// Initialize random enemies
	for (int i = 0; i < 200; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		entities_.emplace_back(new Enemy(ENEMY, glm::vec3(randx, randy, 0), glm::vec3(randx - rand() % 3, randy, 0), this));
	}

	for (int i = 0; i < 50; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		Shotgunner* s = new Shotgunner(GUNNER, glm::vec3(randx, randy, 0), glm::vec3(randx - rand() % 3, randy, 0), this);
		s->SetScale(2.2);
		//entities_.push_back(s);
	}

	// Initialize Buoys
	int num_buoys = 40;
	//float mass_values[] = {3, 8, 20};
	for (int i = 0; i < num_buoys; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		float randmass = mass_values[rand() % 3];
		Satellite* s = new Satellite(BUOY, glm::vec3(randx, randy, 0), this, randmass);
		//entities_.push_back(s);
	}

	// Initialize Powerups
	int num_powerups = 20;
	for (int i = 0; i < num_powerups; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		collectibles_.push_back(new Powerup(POWERUP, glm::vec3(randx, randy, 0), this));
	}

	for (int i = 0; i < num_powerups; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		collectibles_.push_back(new DashCollectible(DASH, glm::vec3(randx, randy, 0), this));
	}

	for (int i = 0; i < num_powerups; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		collectibles_.push_back(new BuffCollectible(BUFF, glm::vec3(randx, randy, 0), this));
	}

	for (int i = 0; i < 10; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		collectibles_.push_back(new PeaShooterCollectible(WEAPON_C, glm::vec3(randx, randy, 0), this));
	}

	// Initialize collectible shields
	int num_shields = 30;
	for (int i = 0; i < num_shields; i++)
	{
		float randx = rand() % world_width_ - (float)world_width_ / 2;
		float randy = rand() % world_height_ - (float)world_height_ / 2;
		collectibles_.push_back(new ShieldCollectible(SHIELD, glm::vec3(randx, randy, 0), this));
	}

	return 0;
}

void Game::Update(float dt)
{
	// Called once per frame
	for (auto it = projectiles_.begin(); it != projectiles_.end();)
	{
		Projectile *prj = *it;
		if (prj->IsDestroyed())
		{
			it = projectiles_.erase(it);
			delete prj;
		}
		else
		{
			CheckProjectileCollisions(prj, dt);
			prj->Update(dt);
			it++;
		}
	}

	SweepAndPruneEntityCollisions();
	for (auto it = entities_.begin(); it != entities_.end();)
	{
		Entity *ent = *it;
		if (ent->IsDestroyed())
		{
			it = entities_.erase(it);
			if (ent != player_)
				delete ent;
			if(ent == player_) {
				GameOver();
				return;
			}
		}
		else
		{
			ent->Update(dt);
			it++;
		}
	}

	for (auto it = collectibles_.begin(); it != collectibles_.end();)
	{
		Collectible *col = *it;
		if (col->IsDestroyed())
		{
			it = collectibles_.erase(it);
			delete col;
		}
		else
		{
			CheckCollectibleCollision(col);
			it++;
		}
	}

	for (auto it = effects_.begin(); it != effects_.end();)
	{
		Effect *eff = *it;
		if (eff->IsDestroyed())
		{
			it = effects_.erase(it);
			delete eff;
		}
		else
		{
			eff->Update(dt);
			it++;
		}
	}

	for (auto it = notifications_.begin(); it != notifications_.end();)
	{
		TextObject *n = *it;
		if (n->IsDestroyed())
		{
			it = notifications_.erase(it);
			delete n;
		}
		else
		{
			n->Update(dt);
			it++;
		}
	}

	CheckPlayerInBorder();
}

void Game::CheckPlayerInBorder()
{
	if (player_->GetPosition().x > world_width_ / 2)
	{
		glm::vec3 force = glm::normalize(player_->GetPosition() - glm::vec3(player_->GetPosition().x + 10, player_->GetPosition().y, player_->GetPosition().z));
		float diff = player_->GetPosition().x - world_width_ / 2;
		player_->SetVelocity(force * glm::clamp(glm::abs(diff), 0.2f, 2.0f));
	}
	if (player_->GetPosition().x < -world_width_ / 2)
	{
		glm::vec3 force = glm::normalize(player_->GetPosition() - glm::vec3(player_->GetPosition().x - 10, player_->GetPosition().y, player_->GetPosition().z));
		float diff = player_->GetPosition().x + world_width_ / 2;
		player_->SetVelocity(force * glm::clamp(glm::abs(diff), 0.2f, 2.0f));
	}
	if (player_->GetPosition().y > world_height_ / 2)
	{
		glm::vec3 force = glm::normalize(player_->GetPosition() - glm::vec3(player_->GetPosition().x, player_->GetPosition().y + 10, player_->GetPosition().z));
		float diff = player_->GetPosition().y - world_height_ / 2;
		player_->SetVelocity(force * glm::clamp(glm::abs(diff), 0.2f, 2.0f));
	}
	if (player_->GetPosition().y < -world_height_ / 2)
	{
		glm::vec3 force = glm::normalize(player_->GetPosition() - glm::vec3(player_->GetPosition().x, player_->GetPosition().y - 10, player_->GetPosition().z));
		float diff = player_->GetPosition().y + world_height_ / 2;
		player_->SetVelocity(force * glm::clamp(glm::abs(diff), 0.2f, 2.0f));
	}
}

//=============COLLISIONS=====================

// Collectibles only check against player
void Game::CheckCollectibleCollision(Collectible *col)
{
	if (player_->CheckCollision(col->GetHitbox()))
	{
		col->HandleCollision(player_);
	}
}

// Projectiles only check against entities
bool Game::CheckProjectileCollisions(Projectile *prj, float dt)
{
	for (int i = 0; i < entities_.size(); i++)
	{
		Entity *opp = entities_[i];
		glm::vec3 ep = opp->GetPosition();
		glm::vec3 pp = prj->GetPosition();

		if (opp->IsDestroyed())
			continue;
		bool hit = prj->RayCircleCollision(opp->GetHitbox(), dt);
		if (hit)
		{
			prj->HandleCollision(opp);
			//I dont know if you made a better system for this but this is just a bandaid solution
			//Projectile now keeps a type (either hits everything or only the player) and this is the only place its used
			//This is a fix for when ENEMY BULLETS HIT ENEMIES AND DESTROY THEM
			if(prj->GetProjectileType() == ENEMY_BULLET && opp != player_) {
				return true;
			}
			opp->Explode();
			return true;
		}
	}
	return false;
}

// Entity Collision ----------------
// Structs for 1D line comparisons
struct line;
struct endpoint
{
	bool starting;
	float value;
	line *parent;
	endpoint(float x, bool start, line *line) : value(x), starting(start), parent(line) {}
};

struct line
{
	endpoint *start;
	endpoint *end;
	int entity_index;
	line(int index) : entity_index(index) {}
};

bool CompareEndpoints(endpoint *e1, endpoint *e2)
{
	return e1->value < e2->value;
}

void Game::SweepAndPruneEntityCollisions()
{
	// Generate list of line segments representing 1D intervals
	line **lines_arr = new line *[entities_.size()];
	std::vector<endpoint *> endpoints;
	for (int i = 0; i < entities_.size(); i++)
	{
		Entity *e = entities_[i];

		line *l = new line(i);
		endpoint *start = new endpoint(e->GetPosition().x - e->GetHitbox().GetRadius(), true, l);
		endpoint *end = new endpoint(e->GetPosition().x + e->GetHitbox().GetRadius(), false, l);
		l->start = start;
		l->end = end;
		lines_arr[i] = l;

		endpoints.push_back(start);
		endpoints.push_back(end);
	}
	std::sort(endpoints.begin(), endpoints.end(), CompareEndpoints); // sort endpoints

	std::vector<line *> active_list;
	std::vector<std::pair<int, int>> potential_collisions;

	for (int i = 0; i < endpoints.size(); i++)
	{
		endpoint *e = endpoints[i];
		if (e->starting)
		{
			for (auto l : active_list)
			{
				// Check intersections between all of active line segments
				if (e->value < l->end->value)
				{
					potential_collisions.push_back({e->parent->entity_index, l->entity_index});
				}
			}
			active_list.push_back(e->parent);
		}
		else
		{
			// remove line segment from active list because we have reached the end point
			for (int j = 0; j < active_list.size(); j++)
			{
				if (active_list[j] == e->parent)
				{
					std::swap(active_list[j], active_list[active_list.size() - 1]);
					active_list.pop_back();
				}
			}
		}
	}

	// Cleanup memory
	for (int i = 0; i < entities_.size(); i++)
	{
		line *l = lines_arr[i];
		delete l->start;
		delete l->end;
		delete l;
	}
	delete[] lines_arr;

	// Evaluate all potential collisions
	for (auto c : potential_collisions)
	{
		Entity *e1 = entities_[c.first];
		Entity *e2 = entities_[c.second];

		bool hit = e1->CheckCollision(e2->GetHitbox());
		if (hit)
		{
			HandleEntityCollision(e1, e2);
		}
	}
}

// Collision tag system for entities
void Game::HandleEntityCollision(Entity *e1, Entity *e2)
{
	GLuint t1 = e1->GetType();
	GLuint t2 = e2->GetType();


	if (t1 == BUOY || t2 == BUOY)
	{
		// Elastic collision
		float m1 = e1->GetMass();
		float m2 = e2->GetMass();
		glm::vec3 v1 = e1->GetVelocity();
		glm::vec3 v2 = e2->GetVelocity();
		glm::vec3 n = glm::normalize(e1->GetPosition() - e2->GetPosition());

		glm::vec3 v1_new = v1 - (2 * m2 / (m1 + m2)) * (glm::dot(n, (v1 - v2))) * n;
		glm::vec3 v2_new = v2 - (2 * m1 / (m1 + m2)) * (glm::dot(n, (v2 - v1))) * n;

		e1->SetVelocity(v1_new);
		e2->SetVelocity(v2_new);

		// Move their hitboxes outside of each other so that they dont get stuck
		float max_dist = e1->GetHitbox().GetRadius() + e2->GetHitbox().GetRadius();
		float overlap = max_dist - glm::distance(e1->GetPosition(), e2->GetPosition());
		if (overlap > 0)
		{
			e1->SetPosition(e1->GetPosition() + n * (overlap * 2));
		}
	}else {
		e1->Explode();
		e2->Explode();
	}
}

// Slow O(n^2) collision check
bool Game::CheckCollisions(Entity *ent, int j)
{
	for (int i = j; i < entities_.size(); i++)
	{
		Entity *opp = entities_[i];
		if (opp->IsDestroyed())
			continue;
		bool rslt = ent->CheckCollision(opp->GetHitbox());
		if (rslt)
		{
			HandleEntityCollision(ent, opp);
			return true;
		}
	}
	return false;
}

void Game::SpawnBullet(GLuint proj_index, glm::vec3& position, Hitbox* instigator, float angle)
{
	Projectile *prj;
	if(proj_index == PROJECTILE_BULLET) {
		prj = new Projectile(*projectile_prototypes_[proj_index]);
	} else {
		prj = new Projectile(*enemy_projectile_prototypes_[proj_index]);
	}
	prj->SetPosition(position);
	prj->SetAngle(angle);
	prj->SetInstigator(instigator);
	projectiles_.push_back(prj);
}

void Game::SpawnExplosion(GLuint eff_index, glm::vec3 position, float scale){
	Effect* ex = new Effect(*effect_prototypes_[eff_index]);
	ex->SetScale(scale/4);
	ex->SetPosition(position);
	effects_.push_back(ex);
}

void Game::SpawnNotificiation(const std::string &text, float dur){
	glm::vec3 startingpos (0,0.65,0);
	float push_size = 0.1;
	TextObject* t = new TextObject(TEXT, startingpos, this, dur);
	t->SetText(text);
	if(text[0] == '-' || text == "Block"){
		t->SetInverted(true);
	}
	notifications_.push_back(t);
	for(int i = 0; i < notifications_.size(); i++){
		notifications_[i]->SetPosition(notifications_[i]->GetPosition() + glm::vec3(0, push_size, 0));
	}
}

// GETTERS AND FORWARDS
void Game::PlayerThrust(float dt)
{
	player_->Thrust(dt);
}

void Game::PlayerBreak()
{
	player_->Break();
}

void Game::PlayerTurn(int t, float dt)
{
	player_->Turn(t, dt);
}

void Game::PlayerShoot()
{
	player_->Shoot();
}

void Game::PlayerLook(glm::vec3 &v, float dt)
{
	player_->TurnTowards(v, dt);
}

void Game::PlayerLook(glm::vec3 &v)
{
	player_->LookAtPoint(v);
}

const std::vector<Entity *> &Game::GetEntities()
{
	return entities_;
}

const std::vector<GameObject *> &Game::GetBackgrounds()
{
	return backgrounds_;
}

const std::vector<Projectile *> &Game::GetProjectiles()
{
	return projectiles_;
}

const std::vector<Effect *> &Game::GetEffects()
{
	return effects_;
}

const std::vector<Collectible *> &Game::GetCollectibles()
{
	return collectibles_;
}
const std::vector<TextObject *>& Game::GetNotifications(){
	return notifications_;
}

Entity *Game::GetPlayer()
{
	return player_;
}

glm::vec3 &Game::GetPlayerPosition()
{
	return player_->GetPosition();
}

void Game::WinGame()
{
	game_over_ = true;
	ClearGame();
	SpawnNotificiation("-YOU WIN!");
}

void Game::GameOver()
{
	game_over_ = true;
	ClearGame();
	SpawnNotificiation("GAME OVER :(", 5.0f);
	SpawnNotificiation("PRESS R TO RESPAWN", 5.0f);
	return;
}

void Game::ClearGame()
{
	backgrounds_.clear();
	for(Entity* e : entities_) {
		if (e != player_) {
			e->SetDestroyed();
		}
	}
	for(Projectile* p : projectiles_) {
		p->SetDestroyed();
	}
	for(Collectible* c : collectibles_) {
		c->SetDestroyed();
	}
	for (TextObject* t : notifications_) {
		t->SetDestroyed();
	}
	for(Effect* e : effects_) {
		e->SetDestroyed();
	}
	return;
}
