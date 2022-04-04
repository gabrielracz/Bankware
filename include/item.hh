#ifndef ITEM_H
#define ITEM_H
#include "game_object.hh"
#include "data.hh"
#include <string>

class Item : public GameObject {
private:
    float path_tracker_ = 0;
	float path_radius_;
	glm::vec3 origin_ = glm::vec3(0);
public:
	Item(GLuint type, const glm::vec3& position, Game* game);
    void Update(float dt) override;
    //At first had Activate and Deactivate taking an Entity* player but include headers didnt like the recursion and gave errors
    //Will just have to use the GetPlayer() function on the game value :)
    inline virtual void Activate() {};
    inline virtual void Deactivate() { SetDestroyed(); };
    inline virtual bool Event(const std::string& event) { return false; };

    inline void SetRadiusProgress(float p) { path_tracker_ = p; } 
    inline float GetRadius() const { return path_radius_; }
};
#endif // ITEM_H