#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include<vector>
#include<glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp> 
#include <glm/gtx/vector_angle.hpp> 
#include <GL/glew.h>

class Game;

class GameObject {
protected:
	Game* game_;
	glm::vec3 position_;
	GLuint type_;
	//GLint num_elements_;
	float scale_; 
	float angle_;
	bool destroyed_;
	bool draw_inverted_;

	std::vector<GameObject*> children_;


public:
	GameObject(GLuint type, const glm::vec3 &position, Game* game);
	~GameObject();

	virtual void Update(float delta_time);

	void AddChild(GameObject*);
	std::vector<GameObject*>& GetChildren();

	void SetDestroyed();
	inline void SetPosition(const glm::vec3& position) { position_ = position; }
	inline void SetScale(float scale) { scale_ = scale; }
	inline void SetAngle(float angle) { angle_ = angle; }
	
	inline glm::vec3& GetPosition(void) { return position_; }
	inline float GetScale(void) { return scale_; }
	inline float GetAngle(void) { return angle_; }
	inline float *GetAnglePointer(void) {return &angle_; }
	glm::vec3 GetAngleVector(void);
	inline bool IsDestroyed(void) { return destroyed_; }
	inline bool IsInverted(void) {return draw_inverted_;}
	inline void SetInverted(bool t) {draw_inverted_ = t;}
	inline GLuint GetType() {return type_;}

	
};

#endif
