#define GLM_FORCE_RADIANS
#include"controller.hh"
#include"view.hh"
#include<iostream>

int main(void){
	Controller* app = new Controller();
	app->Start();
	return 0;
}
