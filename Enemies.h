#include "Models.h"
class EnemyClass : public ModelClass
{
public:
	glm::vec3 enemyPos;
	glm::vec3 enemyRot;
	float enemyScale;
	EnemyClass(std::string path,
		glm::vec3 pos,
		glm::vec3 rot,
		float scale);

	void draw(GLuint shaderProgram);
};
