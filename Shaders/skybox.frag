#version 330 core

out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;
uniform bool bgState;
void main() {
	FragColor = texture(skybox, texCoords);
	if(bgState) {
		FragColor *= vec4(0.5, 2, 0.5, 1);

	}

}