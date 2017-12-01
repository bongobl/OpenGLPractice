#version 330 core
// This is the skybox fragment shader.

// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 normalDataOutput;
in vec3 vertexDataOutput;

in vec3 TexCoords;
out vec4 color;

//how does frag shader get this?
uniform samplerCube skybox;

//not used in skybox
uniform vec3 inColor;

void main()
{
	color = texture(skybox, TexCoords);		//for skybox
	
}
