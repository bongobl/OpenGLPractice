#pragma once
#include "OBJObject.h"
class ControlPoint {
	

	//static fields
	static OBJObject *visual;
	static GLuint PointShaderProgram;

	//member fields
	glm::vec3 position;
	glm::vec3 color;

public:
	
	static void InitStatics();
	static void cleanUpStatics();

	ControlPoint(glm::vec3 col);

	void update();
	void draw();
	void drawAsSelected();

	void move(glm::vec3 deltaPos);
	void setColor(glm::vec3 newCol);

	glm::vec3 getPosition();

};