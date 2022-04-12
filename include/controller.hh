#include "text_object.hh"
#include "view.hh"

#include "game.hh"

#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
private:
	bool running_;
	bool apply_thrust_;
	bool apply_break_;
	bool apply_shoot_;
	int apply_turn_;
	bool apply_right_turn_;
	bool apply_left_turn_;
	bool mouse_look_;
	bool use_dash_;
	bool can_handle_ability_;

	glm::vec3 mousepos;

	std::string title_;
public:
	View* view_;
	Game* game_;

	Controller();
	int Start();
	void Update();

	void HandleQuit();
	void HandleThrust(bool t);
	void HandleTurn(int t);
	void HandleRightTurn(bool t);
	void HandleLeftTurn(bool t);
	void HandleBreak(bool t);
	void HandleShoot(bool t);
	void PlayerLook(bool t, glm::vec3& v);
	void HandleAbility(bool t, const std::string& ability);
	void HandleGameOver(bool t);
	void HandleArcadeMode(bool t);
	inline bool GetGameOver() const { return game_->GetGameOver(); }

	inline const std::vector<Entity*>& GetEntities(){return game_->GetEntities();};
	inline const std::vector<GameObject*>& GetBackgrounds(){return game_->GetBackgrounds();};
	inline const std::vector<Projectile*>& GetProjectiles(){return game_->GetProjectiles();};
	inline const std::vector<Effect*>& GetEffects(){return game_->GetEffects();};
	inline const std::vector<Collectible*>& GetCollectibles(){return game_->GetCollectibles();};
	inline const std::vector<TextObject*>& GetNotifications() {return game_->GetNotifications();}
	inline Entity* GetPlayer(){return game_->GetPlayer();};
};
#endif
