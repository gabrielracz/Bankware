#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include <string>

#include "game_object.hh"

class TextObject : public GameObject {
public:
	TextObject(GLuint texture, const glm::vec3 &position, Game* game);

	// Text to be displayed
	std::string GetText(void) const;
	void SetText(const std::string &text);

private:
	std::string text_;

}; 

#endif
