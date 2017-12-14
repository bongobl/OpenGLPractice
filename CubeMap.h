#ifndef _CUBE_H_
#define _CUBE_H_

#define GLFW_INCLUDE_GLEXT
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

class CubeMap
{
	float scale;
	glm::vec3 position;

	GLuint shaderProgram;
	GLuint textureID;
public:
	CubeMap();
	~CubeMap();

	glm::mat4 toWorld;

	void setScale(float sc);
	void setPosition(glm::vec3 pos);
	glm::vec3 getPosition();
	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;

	void loadCubeMapTexture(std::vector<std::string> faces);
	unsigned char* loadPPM(const char* filename, int& width, int& height);

	friend class Scene;
};

// Define the coordinates and indices needed to draw the cube. Note that it is not necessary
// to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
// This just looks nicer since it's easy to tell what coordinates/indices belong where.
const GLfloat cube_vertices[8][3] = {
	// "Front" vertices
	{-2.0, -2.0,  2.0}, {2.0, -2.0,  2.0}, {2.0,  2.0,  2.0}, {-2.0,  2.0,  2.0},
	// "Back" vertices
	{-2.0, -2.0, -2.0}, {2.0, -2.0, -2.0}, {2.0,  2.0, -2.0}, {-2.0,  2.0, -2.0}
};

// Note that GL_QUADS is deprecated in modern OpenGL (and removed from OSX systems).
// This is why we need to draw each face as 2 triangles instead of 1 quadrilateral

const GLuint cube_indices[36] = { 0, 1, 2, 2, 3, 0,  1, 5, 6, 6, 2, 1, 7, 6, 5, 5, 4, 7,  4, 0, 3, 3, 7, 4,  4, 5, 1, 1, 0, 4,  3, 2, 6, 6, 7, 3 };

#endif

