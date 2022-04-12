#ifndef VIEW_H
#define VIEW_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/string_cast.hpp>
#include <SOIL/SOIL.h>
#include <algorithm>
#include <iostream>
#include "path_config.h"
#include "shader.hh"
#include "data.hh"

class Controller;
class GameObject;
class TextObject;

#define NUM_SHADERS 5
enum Shaders {
	SPRITE_SHADER = 0,
	PARTICLE_SHADER,
	EXPLOSION_SHADER,
	TEXT_SHADER,
	MAP_SHADER
};

class View {

private:
	Controller* controller_;
	
	float uptime_;
	//WINDOW
	std::string window_title_;
	unsigned int window_width_;
	unsigned int window_height_;
	glm::vec4 background_color_;
	GLFWwindow *window_;
	
	//Camera
	float camera_zoom_;
	glm::mat4 view_matrix_;
	glm::mat4 camera_matrix_;
	Shader* shader_;
	Shader shaders_[NUM_SHADERS];

	//TEXTURES
	std::string resources_directory_ = RESOURCES_DIRECTORY;
	int size_;
	
	//Shader shader_;
	GLuint tex_[NUM_TEXTURES];

	//CALLBACKS
	static void ResizeCallback(GLFWwindow* window, int width, int height);
	static void KeyInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseInput(GLFWwindow* window, int button, int action, int mods);

public:
	View(const std::string& title, unsigned int width, unsigned int height, Controller* controller);	
	~View();

	int Init();
	void Update(float dt);

	int CreateSprite(void);
	void SetTexture(GLuint w, const char *fname);
	void SetAllTextures(void);
	void SetTitle(const char* title);
	void RenderObject(GameObject* obj, const glm::mat4& parent_matrix = glm::mat4(1));
	void RenderParticleSystem(GameObject* obj, const glm::mat4& parent_matrix = glm::mat4(1));
	void RenderHUD();
	//void RenderText(TextObject* obj);
	bool IsVisible(GameObject* obj, float f = 1.0f);
	bool GetCursorPos(glm::vec3 *vec);
	void MouseMovementInput();
};

#endif
