#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>


	class MyCamera {
	protected:
		glm::vec3 cameraPos;
		glm::vec3 cameraCenter;
		glm::vec3 worldUp;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::vec3 forward;

	public:

		void setCameraPos(glm::vec3 c_Pos);
		
		void setCameraCenter(glm::vec3 c_Center);
		
		void setWorldUp(glm::vec3 w_Up);
		
		void setForward();
		
		void setForward(glm::vec3* vecD);
		
		glm::vec3 getForward();
		
		void setView();
		
		virtual void moveCam(glm::vec3*) = 0;
		
		glm::vec3 getCameraPos();
		
		glm::vec3 getCameraCenter();
		
		glm::mat4 getProjectionMatrix();
		
		glm::mat4 getViewMatrix();

		virtual void kbCallBack(GLFWwindow* window,
								int key, 
								int scancode, 
								int action, 
								int mods) = 0;
	};

	

	class PerspectiveCamera : public MyCamera {
	public:
		inline void setProjection(float fov, float width, float height) {
			this->projectionMatrix = 
				glm::perspective(
									glm::radians(fov), 
									width / height, 
									0.1f, 100.0f
				);
		}
	};



