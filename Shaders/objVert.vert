#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 aTex;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

out vec2 texCoord;
out vec3 normCoord;

void main(){
	gl_Position = projection * view * transform * vec4(aPos, 1.0);
	mat3 modelMat = mat3(transpose(inverse(transform)));

	texCoord = aTex;
	normCoord = modelMat * vertexNormal;
	out vec3 fragPos = aPos;
}