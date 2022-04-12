#include "controller.hh"
#include "view.hh"
#include <time.h>
#include <string>

Controller::Controller(){
	title_ = "Assignment4 - Gabriel Racz";		
	view_ = new View(title_, 1000, 1000, this);
	view_->Init();

	game_ = new Game(200, 200);
	game_->Init();

	//Flags
	running_ = true;
	apply_thrust_ = false;
	apply_break_ = false;
	apply_shoot_ = false;
	apply_turn_ = 0;
	mouse_look_ = false;
	can_handle_ability_ = true;
}

int Controller::Start(){
	srand(110);
	float last_time = glfwGetTime();
	float current_time = 0;
	float delta_time = 0;
	float acc_delta_time = 0;
	unsigned int frame_counter = 0;
	while(running_){
		//Get frame rate
		frame_counter++;
		current_time = glfwGetTime();
		delta_time = current_time - last_time;
		acc_delta_time += delta_time;
		if(frame_counter % 30 == 0){
			std::string newtitle = title_ + " " + std::to_string(30/acc_delta_time) + "fps";
			view_->SetTitle(newtitle.c_str());
			acc_delta_time = 0;
		}
		last_time = current_time;

		//Forward player controls
		if(apply_thrust_) game_->PlayerThrust(delta_time);
		if(apply_break_) game_->PlayerBreak();

		if(apply_right_turn_) game_->PlayerTurn(1, delta_time);
		else if(apply_left_turn_) game_->PlayerTurn(-1, delta_time);

		if(mouse_look_) game_->PlayerLook(mousepos, delta_time);

		if(apply_shoot_) game_->PlayerShoot();

		view_->Update(delta_time);
		game_->Update(delta_time);
	}
	return 0;
}

void Controller::HandleQuit(){
	running_ = false;
}

void Controller::HandleThrust(bool t){
	if(apply_thrust_ == !t)
	apply_thrust_ = t;
}

void Controller::HandleTurn(int t){
	apply_turn_ = t;	
}

void Controller::HandleBreak(bool t){
	if(apply_break_ == !t)
	apply_break_ = t;
}

void Controller::HandleShoot(bool t){
	if(apply_shoot_ == !t)
		apply_shoot_ = t;
}

void Controller::HandleRightTurn(bool t){
	if(apply_right_turn_ == !t)
		apply_right_turn_ = t;
}

void Controller::HandleLeftTurn(bool t){
	if(apply_left_turn_ == !t)
		apply_left_turn_ = t;
}

void Controller::PlayerLook(bool t, glm::vec3& v){
	//mouse_look_ = t;
	//mousepos = v;
	game_->PlayerLook(v);
}

void Controller::HandleAbility(bool t, const std::string& ability)
{
	if(t && can_handle_ability_) {
		game_->GetPlayer()->RaiseEvent(ability);
		can_handle_ability_ = false;
	}
	if(!t && !can_handle_ability_) {
		can_handle_ability_ = true;
	}
}

void Controller::HandleGameOver(bool t) {
	if (t && game_->GetGameOver()) {
		game_->ClearNotifications();
		game_->Init();
		view_->ResetUptime();
	}
}