#ifndef SHIELD_H
#define SHIELD_H
#include "item.hh"

class Shield : public Item {
private:
	float path_radius_ = 2;
public:
	Shield(GLuint type, const glm::vec3& position, Game* game);
    virtual bool Event(const std::string& event) override;
};

#endif // SHIELD_H
