#include "game.hh"
#include "data.hh"
#include "effect.hh"
#include "collectible.hh"
#include "buoy.hh"
#include "entity.hh"
#include "projectile.hh"
#include"game_object.hh"
#include "blade.hh"
#include "powerup.hh"
#include "item.hh"
#include "shield.hh"
#include "shield_collectible.hh"
#include "weapon.hh"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <iostream>
#include <time.h>
#include<algorithm>
#include <utility>
#include <vector>
#include <array>
Game::Game(int width, int height){
	world_width_ = width;
	world_height_ = height;
}

Game::~Game(){
	for(auto ent : entities_){
		delete ent;
	}
	for(auto bkg : backgrounds_){
		delete bkg;
	}
	for(auto prj : projectiles_){
		delete prj;
	}
	for(auto eff : effects_){
		delete eff;
	}
}

void Game::InitPrototypes(){
	//Init effects
	effect_prototypes_[EFFECT_EXPLOSION] = new Effect(EXPLOSION, glm::vec3(0), this);

	//Init Projectiles
	projectile_prototypes_[PROJECTILE_BULLET] = new Projectile(BULLET, glm::vec3(0), 0, this);
}

//Init the initial game state
int Game::Init(){
	InitPrototypes();
	//Tile the world
	float tile_size = 2;
	int tiles_wide = world_width_/tile_size;
	int tiles_high = world_height_/tile_size;
	for(int y = 0; y < world_height_; y += tile_size){
		for(int x = 0; x < world_width_; x += tile_size){
			int r = rand() % 4;
			GLuint bgs[] = {BACKGROUND1, BACKGROUND2, BACKGROUND3, BACKGROUND4};
			GameObject* bkg = new GameObject(bgs[r], glm::vec3(x - world_width_/2, y - world_height_/2, 0), this);
			bkg->SetScale(tile_size);
			backgrounds_.push_back(bkg);
		}
	}
	
	//Initialze player
	player_ = new Entity(SHIP, glm::vec3(0,0,0), this, 0.45);

	//Blade* b = new Blade(BLADE, glm::vec3(0, 0, 0),this);
	//player_->AddChild(b);
	
	Weapon* right_cannon = new Weapon(BULLET, PROJECTILE_BULLET, glm::vec3(1, 0.5, 0), this);
	right_cannon->SetAimable(true);
	player_->AddWeapon(right_cannon);
	Weapon* left_cannon = new Weapon(BULLET, PROJECTILE_BULLET, glm::vec3(-1, 0.5, 0), this);
	left_cannon->SetAimable(true);
	player_->AddWeapon(left_cannon);
	//Weapon* booty_cannon = new Weapon(BULLET, PROJECTILE_BULLET, glm::vec3(0.0, -1, 0), this);
	//booty_cannon->SetAngle(M_PI);
	//player_->AddWeapon(booty_cannon);

	player_->AddItem(new Shield(SHIELD, glm::vec3(1.5,0,0), this));
	player_->AddItem(new Shield(SHIELD, glm::vec3(1.5,0,0), this));
	player_->AddItem(new Shield(SHIELD, glm::vec3(1.5,0,0), this));
	entities_.push_back(player_);
	
	//Initialize random enemies
	srand(time(NULL));
	for(int i = 0; i < 200; i++){
		float randx = rand()%world_width_ - (float)world_width_/2;
		float randy = rand()%world_height_ - (float)world_height_/2;
		entities_.emplace_back(new Enemy(ENEMY, glm::vec3(randx, randy, 0), glm::vec3(randx - rand()%3, randy, 0), this));
	}
	
	//Initialize Buoys
	int num_buoys = 40;
	float mass_values[] = {3, 8, 20};
	for(int i = 0; i < num_buoys; i++){
		float randx = rand()%world_width_ - (float)world_width_/2;
		float randy = rand()%world_height_ - (float)world_height_/2;
		float randmass = mass_values[rand() %  3];
		entities_.push_back(new Buoy(BUOY, glm::vec3(randx, randy, 0), this, randmass));
	}

	//Initialize Powerups
	int num_powerups = 20;
	for(int i = 0; i < num_powerups; i++){
		float randx = rand()%world_width_ - (float)world_width_/2;
		float randy = rand()%world_height_ - (float)world_height_/2;
		collectibles_.push_back(new Powerup(POWERUP, glm::vec3(randx, randy, 0), this));
	}
	
	//Initialize collectible shields
	int num_shields = 50;
	for(int i = 0; i < num_powerups; i++){
		float randx = rand()%world_width_ - (float)world_width_/2;
		float randy = rand()%world_height_ - (float)world_height_/2;
		collectibles_.push_back(new ShieldCollectible(SHIELD, glm::vec3(randx, randy, 0), this));
	}

	return 0;	
}

void Game::Update(float dt){
	//Called once per frame
	for(auto it = projectiles_.begin(); it != projectiles_.end();){
		Projectile* prj = *it;
		if(prj->IsDestroyed()){
			projectiles_.erase(it);
			delete prj;
		}else {
			CheckProjectileCollisions(prj, dt);
			prj->Update(dt);
			it++;
		}
	}
	
	SweepAndPruneEntityCollisions();
	for(auto it = entities_.begin(); it != entities_.end();){
		Entity* ent = *it;
		if(ent->IsDestroyed()){
			entities_.erase(it);
			if(ent != player_)
				delete ent;
		}else{
			ent->Update(dt);
			it++;
		}
	}

	for(auto it = collectibles_.begin(); it != collectibles_.end();){
		Collectible* col = *it;
		if(col->IsDestroyed()){
			collectibles_.erase(it);
			delete col;
		}else{
			CheckCollectibleCollision(col);
			it++;
		}
	}

	for(auto it = effects_.begin(); it != effects_.end();){
		Effect* eff = *it;
		if(eff->IsDestroyed()){
			effects_.erase(it);
			delete eff;
		}else{
			eff->Update(dt);
			it++;
		}
	}
}

//=============COLLISIONS=====================

//Collectibles only check against player
void Game::CheckCollectibleCollision(Collectible* col){
	if(player_->CheckCollision(col->GetHitbox())){
		col->HandleCollision(player_);
	}
}

//Projectiles only check against entities
bool Game::CheckProjectileCollisions(Projectile *prj, float dt){
	for(int i = 0; i < entities_.size(); i++){
		Entity* opp = entities_[i];
		glm::vec3 ep = opp->GetPosition();
		glm::vec3 pp = prj->GetPosition();
		//if(fabs(ep.x - pp.x) < prj->GetSpeed()*dt && fabs(ep.y - pp.y) < prj->GetSpeed()*dt)
			//continue;
		if(opp->IsDestroyed())
			continue;
		bool hit = prj->RayCircleCollision(opp->GetHitbox(), dt);
		if(hit){
			opp->Explode();
			prj->HandleCollision(opp);
			return true;
		}
	}
	return false;
}

//Entity Collision ----------------
//Structs for 1D line comparisons
struct line;
struct endpoint {
	bool starting;
	float value;
	line* parent;
	endpoint(float x, bool start, line* line) : value(x), starting(start), parent(line) {}
};

struct line {
	endpoint* start;
	endpoint* end;
	int entity_index;
	line(int index) : entity_index(index) {}
};

bool CompareEndpoints(endpoint* e1, endpoint* e2){
	return e1->value < e2->value;
}

void Game::SweepAndPruneEntityCollisions(){ 
	//Generate list of line segments representing 1D intervals
	line** lines_arr = new line*[entities_.size()];
	std::vector<endpoint*> endpoints;
	for(int i = 0; i < entities_.size(); i++){
		Entity* e = entities_[i];

		line* l = new line(i);
		endpoint* start = new endpoint(e->GetPosition().x - e->GetHitbox().GetRadius(), true, l);
		endpoint* end = new endpoint(e->GetPosition().x + e->GetHitbox().GetRadius(), false, l);
		l->start = start;
		l->end = end;
		lines_arr[i] = l;

		endpoints.push_back(start);
		endpoints.push_back(end);
	}
	std::sort(endpoints.begin(), endpoints.end(), CompareEndpoints); //sort endpoints

	std::vector<line*> active_list;
	std::vector<std::pair<int, int>> potential_collisions;
	
	for(int i = 0; i < endpoints.size(); i++){
		endpoint* e = endpoints[i];
		if(e->starting){
			for(auto l : active_list){
				//Check intersections between all of active line segments
				if(e->value < l->end->value){
					potential_collisions.push_back({e->parent->entity_index, l->entity_index});
				}
			}
			active_list.push_back(e->parent);
		}else{
			//remove line segment from active list because we have reached the end point
			for(int j = 0; j < active_list.size(); j++){
				if(active_list[j] == e->parent){
					std::swap(active_list[j], active_list[active_list.size() - 1]);
					active_list.pop_back();
				}
			}
		}
	}

	//Cleanup memory
	for(int i = 0; i < entities_.size(); i++){
		line* l = lines_arr[i];
		delete l->start;
		delete l->end;
		delete l;
	}
	delete[] lines_arr;

	//Evaluate all potential collisions
	for(auto c : potential_collisions){
		Entity* e1 = entities_[c.first];
		Entity* e2 = entities_[c.second];
		
		bool hit = e1->CheckCollision(e2->GetHitbox());
		if(hit){
			HandleEntityCollision(e1, e2);
		}
	}
}



//Collision tag system for entities
void Game::HandleEntityCollision(Entity *e1, Entity *e2){
	GLuint t1 = e1->GetType();
	GLuint t2 = e2->GetType();
	if((t1 == SHIP && t2 == ENEMY) || (t1 == ENEMY && t2 == SHIP)){
		e1->Explode();
		e2->Explode();
	}else if(t1 == BUOY || t2 == BUOY){
		// Elastic collision
		float m1 = e1->GetMass();
		float m2 = e2->GetMass();
		glm::vec3 v1 = e1->GetVelocity();
		glm::vec3 v2 = e2->GetVelocity();
		glm::vec3 n = glm::normalize(e1->GetPosition() - e2->GetPosition());

		glm::vec3 v1_new = v1 - (2*m2 / (m1 + m2))*(glm::dot(n, (v1 - v2)))*n;
		glm::vec3 v2_new = v2 - (2*m1 / (m1 + m2))*(glm::dot(n, (v2 - v1)))*n;

		e1->SetVelocity(v1_new);
		e2->SetVelocity(v2_new);

		//Move their hitboxes outside of each other so that they dont get stuck
		float max_dist = e1->GetHitbox().GetRadius() + e2->GetHitbox().GetRadius();
		float overlap = max_dist - glm::distance(e1->GetPosition(), e2->GetPosition()); 
		if(overlap > 0){
			e1->SetPosition(e1->GetPosition() + n*(overlap*2));
		}
	}
}

//Slow O(n^2) collision check
bool Game::CheckCollisions(Entity* ent, int j){
	for(int i = j; i < entities_.size(); i++){
		Entity* opp = entities_[i];
		if(opp->IsDestroyed()) continue;
		bool rslt = ent->CheckCollision(opp->GetHitbox());
		if(rslt){
			HandleEntityCollision(ent, opp);
			return true;
		}
	}
	return false;
}

void Game::SpawnBullet(GLuint proj_index, glm::vec3 position, float angle){
	Projectile* prj = new Projectile(*projectile_prototypes_[proj_index]);
	prj->SetPosition(position);
	prj->SetAngle(angle);
	projectiles_.push_back(prj);
}

void Game::SpawnExplosion(GLuint eff_index, glm::vec3 position, float scale){
	Effect* ex = new Effect(*effect_prototypes_[eff_index]);
	ex->SetScale(scale);
	ex->SetPosition(position);
	effects_.push_back(ex);
}

//GETTERS AND FORWARDS
void Game::PlayerThrust(float dt){
	player_->Thrust(dt);
}

void Game::PlayerBreak(){
	player_->Break();
}

void Game::PlayerTurn(int t, float dt){
	player_->Turn(t, dt);
}

void Game::PlayerShoot(){
	player_->Shoot();
}

void Game::PlayerLook(glm::vec3& v, float dt){
	player_->TurnTowards(v, dt);
}

void Game::PlayerLook(glm::vec3& v){
	player_->LookAtPoint(v);
}

std::vector<Entity*>& Game::GetEntities(){
	return entities_;
}

std::vector<GameObject*>& Game::GetBackgrounds(){
	return backgrounds_;
}

std::vector<Projectile*>& Game::GetProjectiles(){
	return projectiles_;
}

std::vector<Effect*>& Game::GetEffects(){
	return effects_;
}

std::vector<Collectible*>& Game::GetCollectibles(){
	return collectibles_;
}

Entity* Game::GetPlayer(){
	return player_;
}

glm::vec3& Game::GetPlayerPosition(){
	return player_->GetPosition();
}
