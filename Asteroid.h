#pragma once
#include "OBJObject.h"
#include "Material.h"
#include "shader.h"
#include "BoundingBox.h"
class Asteroid {

	//static fields
	static OBJObject* asteroidTemplates;
	static Material asteroidMaterial;

	//member fields
	OBJObject* asteroidOBJ;
	BoundingBox* boundingBox;

	glm::vec3 position;		//randomized
	glm::vec3 scale;		//randomized
	glm::vec3 spinAxis;		//randomized
	float rotationSpeed;	//randomized
	glm::vec3 velocity;		//randomized

	float rotation;

public:

	static void initStatics();
	static void cleanUpStatics();
	Asteroid(unsigned int id);
	~Asteroid();

	void update(float deltaTime);
	void draw();
	
	void setPosition(glm::vec3 pos);
	glm::vec3 getPosition();
	void setScale(glm::vec3 sc);
	glm::vec3 getScale();

	void setVelocity(glm::vec3 vel);
	glm::vec3 getVelocity();
	BoundingBox*  getBoundingBox();
	void setSpinAxis(glm::vec3 spa);
	void setRotationSpeed(float rs);

	glm::mat4 getUpdatedToWorldMatrix();
	
};