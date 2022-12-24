#include "Players.h"

lightBuilder::lightBuilder() : specPhong(0), specStr(0), ambStr(0), lumens(0), ambRGB(glm::vec3(0)),
ray(glm::vec3(0)), lightRGB(glm::vec3(0))
{}

lightBuilder::lightBuilder(lightBuilder * lb) : specPhong(lb->specPhong), specStr(lb->specStr), ambStr(lb->ambStr), lumens(lb->lumens),
ambRGB(lb->ambRGB), ray(lb->ray), lightRGB(lb->lightRGB)
{}

lightBuilder* lightBuilder::setSpecStr(float str)
{
	specStr = str;
	return this;
}

void lightBuilder::placeUnifs(GLint* uniforms)
{
	glUniform1f(uniforms[0], specPhong);
	glUniform1f(uniforms[1], specStr);
	glUniform1f(uniforms[2], ambStr);
	glUniform1f(uniforms[3], lumens);
	glUniform3fv(uniforms[4], 1, glm::value_ptr(ambRGB));
	glUniform3fv(uniforms[5], 1, glm::value_ptr(lightRGB));
	placeLight(uniforms[6]);
}
