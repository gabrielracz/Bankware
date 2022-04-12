#include "view.hh"
#include <GLFW/glfw3.h>
//#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <ios>
#include <ostream>
#include <sstream>
#include <string>
#include "collectible.hh"
#include "controller.hh"
#include "data.hh"
#include "game_object.hh"
#include "text_object.hh"
#include<iostream>
#include <iomanip>


View::View(const std::string& title, unsigned int window_width, unsigned int window_height, Controller* controller){
	window_title_ = title;
	window_width_ = window_width;
	window_height_ = window_height;
	controller_ = controller;
	background_color_ = glm::vec4(0, 0, 0, 1);
	uptime_ = 0;
}


View::~View(){
	glfwDestroyWindow(window_);
	glfwTerminate();
}


void View::Update(float dt){
	uptime_ += dt;
	if(glfwWindowShouldClose(window_)){
		controller_->HandleQuit();
		return;
	}
	glClearColor(background_color_.r, background_color_.g, background_color_.b, background_color_.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GameObject* player = controller_->GetPlayer();
	glm::mat4 translate_to_player = glm::translate(glm::mat4(1.0f), -player->GetPosition());
	camera_matrix_ = view_matrix_ * translate_to_player;

	RenderHUD();

	shader_ = &shaders_[SPRITE_SHADER];
	shader_->Enable();
	shader_->SetUniformMat4("view_matrix", camera_matrix_);

	//Draw game objects
	std::vector<Entity*> entities = controller_->GetEntities();
	for(auto obj : entities){
		if(IsVisible(obj))
			RenderObject(obj);
	}
	
	std::vector<Projectile*> projectiles = controller_->GetProjectiles();
	for(auto prj : projectiles){
		if(IsVisible(prj))
			RenderObject(prj);
	}

	std::vector<Collectible*> collectibles = controller_->GetCollectibles();
	for(auto col : collectibles){
		if(IsVisible(col))
			RenderObject(col);
	}
	
	std::vector<Effect*> effects = controller_->GetEffects();
	for(auto eff : effects){
		if(IsVisible(eff))
			RenderParticleSystem(eff);
	}

	shader_ = &shaders_[SPRITE_SHADER];
	shader_->Enable();
	shader_->SetSpriteAttributes();

	std::vector<GameObject*> backgrounds = controller_->GetBackgrounds();
	for(auto bkg : backgrounds){
		if(IsVisible(bkg))
			RenderObject(bkg);
	}

	//Draw buffers
	glfwSwapBuffers(window_);
	glfwPollEvents();
	MouseMovementInput();
}

//Check coordinates against the camera matrix to see if it is currently visible
//If the position of the object relative to camera lies between 1 and -1
//on bot x and y it is visible in the current window
bool View::IsVisible(GameObject *obj, float f){
	glm::vec4 p = -camera_matrix_ * glm::vec4(obj->GetPosition(), 1.0);
	float s = 1;
	return (p.x < (s + camera_zoom_)) && (p.x > (-s - camera_zoom_)) && (p.y < (s + camera_zoom_) && p.y > (-s - camera_zoom_));
}


void View::RenderObject(GameObject* obj, const glm::mat4& parent_matrix){
	if(obj->GetType() == FIRE || obj->GetType() == PARTICLE_EXPLOSION){
		RenderParticleSystem(obj, parent_matrix);
		shader_ = &shaders_[SPRITE_SHADER];
		shader_->SetSpriteAttributes();
		shader_->Enable();
		return;
	}
	glBindTexture(GL_TEXTURE_2D, tex_[obj->GetType()]);
	//
	//Create the transformation matrix
	float s = obj->GetScale();
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(s, s, 1.0));
	glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), obj->GetPosition());
	glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), obj->GetAngle(), glm::vec3(0,0,1)) ; 
	glm::mat4 transformation_matrix =  translation_matrix * rotation_matrix * scaling_matrix;   //TORS
	shader_->SetUniformMat4("transformation_matrix", parent_matrix * transformation_matrix);

	
	//Hack to make color of player change
	bool draw_inverted = false;
	if(obj->IsInverted()){
		draw_inverted = true;
	}
	shader_->SetUniform1i("inverted", draw_inverted);	
	glDrawElements(GL_TRIANGLES, shader_->GetSpriteSize(), GL_UNSIGNED_INT, 0);
    // Draw the entity
	std::vector<GameObject*> children = obj->GetChildren();
	if(children.size() > 0){
		for(int i = 0; i < children.size(); i++){
			RenderObject(children[i], transformation_matrix);
		}
	}
}

void View::RenderParticleSystem(GameObject* obj, const glm::mat4& parent_matrix){
	if(obj->GetType() == FIRE)
		shader_ = &shaders_[PARTICLE_SHADER];
	else
		shader_ = &shaders_[EXPLOSION_SHADER];
		
	shader_->Enable();
	shader_->SetParticleAttributes();

    glBindTexture(GL_TEXTURE_2D, tex_[obj->GetType()]);

	float s = obj->GetScale();
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(s, s, 1.0));
	glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), obj->GetPosition());
	glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), obj->GetAngle(), glm::vec3(0,0,1)) ; 
	glm::mat4 transformation_matrix =  translation_matrix * rotation_matrix * scaling_matrix;   //TORS
	shader_->SetUniformMat4("view_matrix", camera_matrix_);
	shader_->SetUniformMat4("transformation_matrix", parent_matrix * transformation_matrix);

    // Set the time in the shader
    shader_->SetUniform1f("time", uptime_);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, shader_->GetParticleSize(), GL_UNSIGNED_INT, 0);
}
void View::RenderHUD(){
	std::stringstream time;
	time << std::fixed <<  std::setprecision(3) << uptime_;
	RenderText(time.str(), glm::vec3(-0.80, 0.94, 0));

	std::vector<TextObject*> notifs = controller_->GetNotifications();
	for(auto n : notifs){
		RenderText(n->GetText(), n->GetPosition());
	}



}

void View::RenderText(const std::string &text, glm::vec3 pos, float size){
#define TEXT_LENGTH 15
    glBindTexture(GL_TEXTURE_2D, tex_[TEXT]);
	shader_ = &shaders_[TEXT_SHADER];
	shader_->Enable();
	shader_->SetSpriteAttributes();

	// Set text length
	int final_size = text.size();
	if (final_size > TEXT_LENGTH){
		final_size = TEXT_LENGTH;
	}

	shader_->SetUniform1i("text_len", final_size);
	float h = camera_zoom_*1;
	float w = final_size*0.75*(h);
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(w,h,1));

    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), pos);
	
	shader_->SetUniformMat4("view_matrix", camera_matrix_);
    shader_->SetUniformMat4("transformation_matrix", translation_matrix * scaling_matrix);
	//std::cout << text << std::endl;


	// Set the text data
	GLint data[TEXT_LENGTH];
	for (int i = 0; i < final_size; i++){
		data[i] = text[i];
	}
	shader_->SetUniformIntArray("text_content", final_size, data);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, shader_->GetSpriteSize(), GL_UNSIGNED_INT, 0);
}

void View::MouseMovementInput(){
	if (glfwGetWindowAttrib(window_, GLFW_HOVERED)){
		glm::vec3 cursor;
		controller_->PlayerLook(GetCursorPos(&cursor), cursor);
	}
}

void View::KeyInput(GLFWwindow* window, int key, int scancode, int action, int mods){
	//Allow this static function to talk to the controller
	//Write the actual implentation in the controller class and just 
	//use this funtion to call the handle functions
	Controller* controller = (Controller*) glfwGetWindowUserPointer(window);
	if (key == GLFW_KEY_Q && action == GLFW_PRESS){
		glfwWindowShouldClose(window);
		controller->HandleQuit();
	}
	if (key == GLFW_KEY_W || key == GLFW_KEY_LEFT_SHIFT){
		if(action == GLFW_PRESS){
			controller->HandleThrust(true);
		}else if (action == GLFW_RELEASE){
			controller->HandleThrust(false);
		}
	}
	if (key == GLFW_KEY_S){
		if(action == GLFW_PRESS){
			controller->HandleBreak(true);
		}else if (action == GLFW_RELEASE){
			controller->HandleBreak(false);
		}
	}
	if (key == GLFW_KEY_D){
		if(action == GLFW_RELEASE){
			controller->HandleLeftTurn(false);
		}else{
			controller->HandleLeftTurn(true);
		}
	}
	if (key == GLFW_KEY_A){
		if(action == GLFW_RELEASE){
			controller->HandleRightTurn(false);
		}else{
			controller->HandleRightTurn(true);
		}
	}
	if (key == GLFW_KEY_SPACE){
		if(action == GLFW_RELEASE){
			controller->HandleShoot(false);
		}else{
			controller->HandleShoot(true);
		}
	}
}

//Mouse input callback
void View::MouseInput(GLFWwindow* window, int key, int action, int mods){
	Controller* controller = (Controller*) glfwGetWindowUserPointer(window);
	if(key == GLFW_MOUSE_BUTTON_LEFT){
		if(action == GLFW_PRESS){
			controller->HandleShoot(true);
		}else if (action == GLFW_RELEASE){
			controller->HandleShoot(false);
		}
	}
	if(key == GLFW_MOUSE_BUTTON_RIGHT){
		if(action == GLFW_PRESS) {
			controller->HandleAbility(true, "dash");
		} else if (action == GLFW_RELEASE) {
			controller->HandleAbility(false, "dash");
		}
	}
}

bool View::GetCursorPos(glm::vec3 *vec){
	double x, y;
	glfwGetCursorPos(window_, &x, &y);
	int window_width, window_height;
	glfwGetWindowSize(window_, &window_width, &window_height);
	
	//Check if within window
	if (x < 0 || x > window_width || y < 0 || y > window_height) return false;

	float w = window_width;
	float h = window_height;
	float aspect_ratio = w / h;
	float cursor_x_pos = ((2.0f * x - w) * aspect_ratio) / (w * camera_zoom_);
	float cursor_y_pos = (-2.0f * y + h) / (h * camera_zoom_);
	//*vec = glm::vec3(cursor_x_pos, cursor_y_pos, 0);
	glm::vec3 player_pos = controller_->GetPlayer()->GetPosition();
    *vec = glm::vec3(cursor_x_pos + player_pos.x, cursor_y_pos + player_pos.y, 0);
	return true;
}
int View::Init(){
    // Initialize the window management library (GLFW)
    if (!glfwInit()) {
        throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
    }

    

	// Set window to not resizable
    // Required or else the calculation to get cursor pos to screenspace will be incorrect
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

    // Create a window and its OpenGL context
    window_ = glfwCreateWindow(window_width_, window_height_, window_title_.c_str(), NULL, NULL); 
	if (!window_) {
        glfwTerminate();
        throw(std::runtime_error(std::string("Could not create window")));
    }
	glfwSetWindowUserPointer(window_, this->controller_);
	

    // Make the window's OpenGL context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
    }

    // Set event callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Set up square geometry
    //size_ = CreateSprite();

    // Initialize shaders
	shaders_[SPRITE_SHADER].Init((resources_directory_+std::string("/shaders/sprite_vertex_shader.glsl")).c_str(), (resources_directory_+std::string("/shaders/sprite_fragment_shader.glsl")).c_str());
	shaders_[SPRITE_SHADER].CreateSprite();

	shaders_[PARTICLE_SHADER].Init((resources_directory_+std::string("/shaders/particle_vertex_shader.glsl")).c_str(), (resources_directory_+std::string("/shaders/sprite_fragment_shader.glsl")).c_str());
	shaders_[PARTICLE_SHADER].CreateParticles();

	shaders_[EXPLOSION_SHADER].Init((resources_directory_+std::string("/shaders/particle_vertex_shader.glsl")).c_str(), (resources_directory_+std::string("/shaders/sprite_fragment_shader.glsl")).c_str());
	shaders_[EXPLOSION_SHADER].CreateParticleExplosion();

	shaders_[TEXT_SHADER].Init((resources_directory_+std::string("/shaders/text_vertex_shader.glsl")).c_str(), (resources_directory_+std::string("/shaders/text_fragment_shader.glsl")).c_str());
	shaders_[TEXT_SHADER].CreateSprite();

	//shader_ = &shaders_[SPRITE_SHADER];
	//shader_->Enable();
    //shader_->SetSpriteAttributes();
	
	//Init the view matrix
	camera_zoom_ = fmin(window_width_, window_height_) / 15000.0f;
	view_matrix_ = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom_, camera_zoom_, camera_zoom_));

    // Set up z-buffer for rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable Alpha blending
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);
	
	//Use this to disable vsync
	//glfwSwapInterval(0);

	SetAllTextures();
	
	//glfwSetInputMode(window_, GLFW_STICKY_KEYS, GLFW_TRUE);
	//Queue up the events on every frame
	glfwSetKeyCallback(window_, KeyInput);
	glfwSetMouseButtonCallback(window_, MouseInput);
	return true;
}

// Create the geometry for a sprite (a squared composed of two triangles)
// Return the number of array elements that form the square
int View::CreateSprite(void)
{
    // The face of the square is defined by four vertices and two triangles

    // Number of attributes for vertices and faces
    // const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
    // const int face_att = 3; // Vertex indices (3)

    GLfloat vertex[]  = {
        // Four vertices of a square
        // Position      Color                Texture coordinates
        -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f, // Top-left
         0.5f,  0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f, // Top-right
         0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f  // Bottom-left
    };

    // Two triangles referencing the vertices
    GLuint face[] = {
        0, 1, 2, // t1
        2, 3, 0  //t2
    };

    // OpenGL buffers
    GLuint vbo, ebo;

    // Create buffer for vertices
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    // Create buffer for faces (index buffer)
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

    // Return number of elements in array buffer (6 in this case)
    return (int)sizeof(face) / sizeof(GLuint);
}

//Actually read the image data and feed it to openGl for later use
void View::SetTexture(GLuint w, const char *fname)
{
    // Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, w);

    // Load texture from a file to the buffer
    int width, height;
    unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    // Texture Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//Read textures from resources_directory_ and initialize the tex_ array
void View::SetAllTextures(void)
{
    
    glGenTextures(NUM_TEXTURES, tex_);
	SetTexture(tex_[SHIP], (resources_directory_+std::string("/amberocket.png")).c_str());
	SetTexture(tex_[BACKGROUND1], (resources_directory_+std::string("/space1.png")).c_str());
	SetTexture(tex_[BACKGROUND2], (resources_directory_+std::string("/space2.png")).c_str());
	SetTexture(tex_[BACKGROUND3], (resources_directory_+std::string("/space3.png")).c_str());
	SetTexture(tex_[BACKGROUND4], (resources_directory_+std::string("/space4.png")).c_str());
	SetTexture(tex_[ENEMY], (resources_directory_+std::string("/amberdisc.png")).c_str());
	SetTexture(tex_[BLADE], (resources_directory_+std::string("/Sprites/blade-export.png")).c_str());
	SetTexture(tex_[BULLET], (resources_directory_+std::string("/bullet.png")).c_str());
	SetTexture(tex_[EXPLOSION], (resources_directory_+std::string("/Sprites/explosion.png")).c_str());
	SetTexture(tex_[SHIELD], (resources_directory_+std::string("/shield.png")).c_str());
	SetTexture(tex_[POWERUP], (resources_directory_+std::string("/powerup.png")).c_str());
	SetTexture(tex_[BUOY], (resources_directory_+std::string("/buoy.png")).c_str());
	SetTexture(tex_[FIRE], (resources_directory_+std::string("/fire.png")).c_str());
	SetTexture(tex_[PARTICLE_EXPLOSION], (resources_directory_+std::string("/fire.png")).c_str());
	SetTexture(tex_[TEXT], (resources_directory_+std::string("/charmap-cellphone_white.png")).c_str());
	SetTexture(tex_[DASH], (resources_directory_+std::string("/dash.png")).c_str());
	SetTexture(tex_[BUFF], (resources_directory_+std::string("/buff.png")).c_str());
	SetTexture(tex_[E_BULLET], (resources_directory_+std::string("/enemy_bullet.png")).c_str());
	SetTexture(tex_[NONE], (resources_directory_+std::string("/none.png")).c_str());
    // glBindTexture(GL_TEXTURE_2D, tex_[0]);
}


void View::ResizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void View::SetTitle(const char *title){
	glfwSetWindowTitle(window_, title);
}
