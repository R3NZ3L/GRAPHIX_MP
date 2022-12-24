#pragma once
#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>


class lightBuilder
{
private:
	float specPhong;
	float specStr;
	float ambStr;
	float lumens;
	glm::vec3 ambRGB;
	glm::vec3 ray;
	glm::vec3 lightRGB;

public:
	lightBuilder();

	lightBuilder(lightBuilder* lb);

	lightBuilder* setSpecStr(float str);

	float getSpecStr()
	{
		return this->specStr;
	}

	lightBuilder* setSpecPhong(float phong)
	{
		specPhong = phong;
		return this;
	}

	inline float getSpecPhong()
	{
		return this->specPhong;
	}

	inline lightBuilder* setAmbStr(float str)
	{
		ambStr = str;
		return this;
	}

	inline float getAmbStr()
	{
		return ambStr;
	}

	inline lightBuilder* setLumens(float str)
	{
		lumens = str;
		return this;
	}

	inline float getLumens()
	{
		return lumens;
	}

	inline lightBuilder* setAmbColor(glm::vec3* color)
	{
		ambRGB = *color;
		return this;
	}

	inline glm::vec3 getAmbRGB()
	{
		return ambRGB;
	}

	inline lightBuilder* setLightColor(glm::vec3* color)
	{

		lightRGB = *color;
		return this;
	}

	inline glm::vec3 getLightRGB()
	{
		return lightRGB;
	}

	inline lightBuilder* setLightVec(glm::vec3* dir)
	{
		ray = *dir;
		return this;
	}

	inline glm::vec3 getLightVec()
	{
		return ray;
	}

	inline void placeLight(GLint unif)
	{
		glUniform3fv(unif, 1, glm::value_ptr(ray));
	}

	void placeUnifs(GLint* uniforms);
};
