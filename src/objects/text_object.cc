#include "text_object.hh"

TextObject::TextObject(GLuint texture, const glm::vec3 &position, Game* game)
	: GameObject(texture, position, game) {
            text_ = "";
	}


std::string TextObject::GetText(void) const {
    return text_;
}


void TextObject::SetText(const std::string &text){
    text_ = text;
}
