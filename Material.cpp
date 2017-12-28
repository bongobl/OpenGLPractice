#include "Material.h"
#include "shader.h"

GLuint Material::shaderProgram = -1;
std::vector<GLuint> Material::allIDs;
void Material::initStatics() {
	shaderProgram = LoadShaders("../shader.vert", "../shader_material.frag");

}
void Material::cleanUpStatics() {
	for (unsigned int i = 0; i < allIDs.size(); ++i) {
		glDeleteTextures(1, &allIDs.at(i));
	}
	glDeleteProgram(shaderProgram);
}

Material Material::basic() {
	return Material();
}
Material::Material() {
	useDiffuse = false;
	useSpecular = false;
	useTexture = false;
	useNormalMap = false;
	useReflectionTexture = false;
	useLighting = true;

	textureID = 0;
	normalMapID = 0;
	reflectionTextureID = 0;
	diffuse = glm::vec3(0, 0, 0);
	specular = glm::vec3(0, 0, 0);
}
Material::~Material() {
	
}

void Material::setUseLighting(bool opt) {
	useLighting = opt;
}
void Material::setDiffuseColor(glm::vec3 c) {
	diffuse = c;
	useDiffuse = true;
}

void Material::setSpecularColor(glm::vec3 c) {
	specular = c;
	useSpecular = true;
}
void Material::loadTexture(const char* filename) {

	loadImage(filename, textureID);
	useTexture = true;
}
void Material::loadNormalMap(const char* filename) {

	loadImage(filename, normalMapID);
	useNormalMap = true;
}
void Material::loadReflectionTexture(std::vector<std::string> faces) {
	
	loadCubeMapTexture(faces, reflectionTextureID);
	useReflectionTexture = true;
	
}
GLuint Material::getTextureID() {
	return textureID;	
}

GLuint Material::getNormalMapID() {
	return normalMapID;
}

GLuint Material::getShaderProgram() {
	return shaderProgram;
}

glm::vec3 Material::getDiffuseColor() {
	return diffuse;
}

glm::vec3 Material::getSpecularColor() {
	return specular;
}
void Material::applySettings() {

	//material properties	
	glUniform1i(glGetUniformLocation(shaderProgram, "material.useDiffuse"), useDiffuse);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.useSpecular"), useSpecular);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.useTexture"), useTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.useNormalMap"), useNormalMap);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.useReflectionTexture"), useReflectionTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.useLighting"), useLighting);
	if (useDiffuse) {
		glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), diffuse.r, diffuse.g, diffuse.b);
	}
	if (useSpecular) {
		glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), specular.r, specular.g, specular.b);
	}
	if (useTexture) {
		glUniform1i(glGetUniformLocation(shaderProgram, "material.texture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
	if (useNormalMap) {
		glUniform1i(glGetUniformLocation(shaderProgram, "material.normalMap"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMapID);
	}
	
	if (useReflectionTexture) {
		glUniform1i(glGetUniformLocation(shaderProgram, "material.reflectionTexture"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, reflectionTextureID);
	}
}

void Material::loadImage(const char* filename, GLuint &currID) {

	int twidth, theight;   // texture width/height [pixels]
	unsigned char* tdata;  // texture pixel data

						   // Load image file
	tdata = loadPPM(filename, twidth, theight);
	if (tdata == NULL) return;

	// Create ID for texture
	glGenTextures(1, &currID);

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, currID);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);

	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	allIDs.push_back(currID);
}

void Material::loadCubeMapTexture(std::vector<std::string> faces, GLuint &currID) {

	glGenTextures(1, &currID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, currID);

	int width, height;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = loadPPM(faces[i].c_str(), width, height);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	std::cout << "loaded cubemap texture" << std::endl;

	allIDs.push_back(currID);
}


unsigned char* Material::loadPPM(const char* filename, int& width, int& height) {

	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}