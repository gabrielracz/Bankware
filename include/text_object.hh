#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include <string>

#include "game_object.hh"

class TextObject : public GameObject {
public:
	TextObject(GLuint texture, const glm::vec3 &position, Game* game, float lifetime_ = 1.0f);

	// Text to be displayed
	void Update(float dt) override;
	std::string GetText(void) const;
	void SetText(const std::string &text);

	inline float GetLifetime() {return lifetime_;}
	inline float GetUptime() {return uptime_;}

private:
	std::string text_;
	float lifetime_;
	float uptime_;

}; 

#endif
