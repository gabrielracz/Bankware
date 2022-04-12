#include "text_object.hh"
#include <iostream>

TextObject::TextObject(GLuint texture, const glm::vec3 &position, Game* game, float lifetime)
	: GameObject(texture, position, game) {
            text_ = "";
			lifetime_ = lifetime;
			uptime_ = 0;
	}

void TextObject::Update(float dt){
	if(uptime_ < lifetime_){
		uptime_ += dt;
	}else{
		SetDestroyed();
	}
	GameObject::Update(dt);
}

std::string TextObject::GetText(void) const {
    return text_;
}


void TextObject::SetText(const std::string &text){
    text_ = text;
}
