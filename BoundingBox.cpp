#include "BoundingBox.h"
#include "Scene.h"
BoundingBox::BoundingBox(std::vector<glm::vec3> verts) {
	meshVertices = verts;

	//init box vertices
	for (int i = 0; i < 24; ++i) {
		boxVertices.push_back(glm::vec3(0, 0, 0));
	}

	//init material
	material.setSurfaceColor(glm::vec3(0, 0, 1));
	material.setUseSurfaceColor(true);
	

	//prepare buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBufferData(GL_ARRAY_BUFFER, boxVertices.size() * sizeof(glm::vec3), boxVertices.data(), GL_STATIC_DRAW);
}
BoundingBox::~BoundingBox() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);	
}

void BoundingBox::updateToWorld(glm::mat4 toWorld) {


	lowest = toWorld * glm::vec4(meshVertices.at(0).x, meshVertices.at(0).y, meshVertices.at(0).z, 1);
	highest = toWorld * glm::vec4(meshVertices.at(0).x, meshVertices.at(0).y, meshVertices.at(0).z, 1);
	for (unsigned int i = 0; i < meshVertices.size(); ++i) {
		
		//find current vertex coords in world coordinates
		glm::vec4 currVertex = toWorld * glm::vec4(meshVertices.at(i).x, meshVertices.at(i).y, meshVertices.at(i).z, 1);

		if (currVertex.x < lowest.x)
			lowest.x = currVertex.x;
		if (currVertex.y < lowest.y)
			lowest.y = currVertex.y;
		if (currVertex.z < lowest.z)
			lowest.z = currVertex.z;

		if (currVertex.x > highest.x)
			highest.x = currVertex.x;
		if (currVertex.y > highest.y)
			highest.y = currVertex.y;
		if (currVertex.z > highest.z)
			highest.z = currVertex.z;

	}//END FOR
	
	//X
	boxVertices.at(0) = glm::vec3(lowest.x, lowest.y, lowest.z);
	boxVertices.at(1) = glm::vec3(highest.x, lowest.y, lowest.z);
	boxVertices.at(2) = glm::vec3(lowest.x, lowest.y, highest.z);
	boxVertices.at(3) = glm::vec3(highest.x, lowest.y, highest.z);
	boxVertices.at(4) = glm::vec3(lowest.x, highest.y, lowest.z);
	boxVertices.at(5) = glm::vec3(highest.x, highest.y, lowest.z);
	boxVertices.at(6) = glm::vec3(lowest.x, highest.y, highest.z);
	boxVertices.at(7) = glm::vec3(highest.x, highest.y, highest.z);
	
	//Y
	boxVertices.at(8) = glm::vec3(lowest.x, lowest.y, lowest.z);
	boxVertices.at(9) = glm::vec3(lowest.x, highest.y, lowest.z);
	boxVertices.at(10) = glm::vec3(lowest.x, lowest.y, highest.z);
	boxVertices.at(11) = glm::vec3(lowest.x, highest.y, highest.z);
	boxVertices.at(12) = glm::vec3(highest.x, lowest.y, lowest.z);
	boxVertices.at(13) = glm::vec3(highest.x, highest.y, lowest.z);
	boxVertices.at(14) = glm::vec3(highest.x, lowest.y, highest.z);
	boxVertices.at(15) = glm::vec3(highest.x, highest.y, highest.z);

	//Y
	boxVertices.at(16) = glm::vec3(lowest.x, lowest.y, lowest.z);
	boxVertices.at(17) = glm::vec3(lowest.x, lowest.y, highest.z);
	boxVertices.at(18) = glm::vec3(lowest.x, highest.y, lowest.z);
	boxVertices.at(19) = glm::vec3(lowest.x, highest.y, highest.z);
	boxVertices.at(20) = glm::vec3(highest.x, lowest.y, lowest.z);
	boxVertices.at(21) = glm::vec3(highest.x, lowest.y, highest.z);
	boxVertices.at(22) = glm::vec3(highest.x, highest.y, lowest.z);
	boxVertices.at(23) = glm::vec3(highest.x, highest.y, highest.z);

	//update VBO for new box vertices positions
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, boxVertices.size() * sizeof(glm::vec3), boxVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void BoundingBox::draw(Scene* currScene) {

	glUseProgram(Material::getShaderProgram());

	Camera* activeCamera = currScene->getActiveCamera();

	//apply object boundingbox properties	
	glUniformMatrix4fv(glGetUniformLocation(material.getShaderProgram(), "toWorld"), 1, GL_FALSE, &glm::mat4(1.0f)[0][0]);

	//apply camera properties
	activeCamera->applySettings(Material::getShaderProgram());
	
	//apply material properties
	material.applySettings();

	//Bind VAO for box and draw 
	glBindVertexArray(VAO);
	glLineWidth(3);
	glDrawArrays(GL_LINES, 0, boxVertices.size());
	glBindVertexArray(0);

}

bool BoundingBox::isCollidingWith(const BoundingBox* other) {

	if(this->highest.x < other->lowest.x)
		return false;
	if (this->lowest.x > other->highest.x)
		return false;
	if (this->highest.y < other->lowest.y)
		return false;
	if (this->lowest.y > other->highest.y)
		return false;
	if (this->highest.z < other->lowest.z)
		return false;
	if (this->lowest.z > other->highest.z)
		return false;

	return true;
}