#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.h"
class Scene;
class SceneManager {

	//Window Variables
	static int windowWidth;
	static int windowHeight;

	//time keeping
	static float currTime;
	static float prevTime;
	static float deltaTime;
	//Scene
	static Scene* currScene;

	
	//Gaussian Blur Data
	static GLuint blurShaderProgram;
	static GLuint frameBufferID;
	static Texture frameTexture;
	static GLuint renderBufferID;
	static GLenum drawBuffers[1];
	static GLuint VAO_ScreenQuad;
	static GLuint VBO_SceenQuadPositions;
	static GLfloat screenQuadVertexPositions[];
	static GLuint EB0_ScreenQuad;
	static GLuint meshIndices[];
	
	static float testFloat[1];

public:
	
	static GLFWwindow* window;

	static int createWindow(const char* title, int window_width, int window_height);
	static void initObjects();
	static void dispose();
	static void update();
	static void draw();
	

	//callbacks
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow * window, double xpos, double ypos);
	static void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset);
	
	static bool isWindowOpen();

	//Utilities 
	static float getDeltaTime();

private:

	static void initFrameBufferObjects();
	static void resizeFrameBufferObjects();
};