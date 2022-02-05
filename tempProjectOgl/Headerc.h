#pragma once
#ifndef HEADERC_H
#define HEADERC_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};
const float YAW = 90.0f;
const float PITCH = 0.0f;
const float FOV = 45.0f;

class Camera
{

public:


	glm::vec3 myPos;
	glm::vec3 myUp;
	glm::vec3 myFront;
	glm::vec3 myRight;

	glm::vec3 Position;
	glm::vec3 Up;
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 upGlobal = glm::vec3(0.0f, 1.0f, 0.0f);




	float lastx = 800.0f / 2.0;
	float lasty = 600.0 / 2.0;

	float YAWcl;
	float PITCHcl;
	float FOVcl;

	Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 front, float y = YAW, float p = PITCH, float f = FOV)
	{
		Position = pos;
		Up = up;
		Front = front;
		YAWcl = y;
		PITCHcl = p;
		FOVcl = f;

		CameraDirection();

	};

	/*glm::mat4 myLookAt()
	{
		myFront = glm::normalize(Position-(Position+Front));
		myRight = glm::normalize(glm::cross(upGlobal,myFront));
		//Right = myRight;
		myUp = glm::cross(myFront, myRight);

		//Up = myUp;

		glm::mat4 matR = glm::mat4(1.0f);
		matR[3][0] = -Position.x;
		matR[3][1] = -Position.y;
		matR[3][2] = -Position.z;

		glm::mat4 matL = glm::mat4(1.0f);
		matL = glm::mat4(
			myRight.x, myUp.x, myFront.x, 0.0f,
			myRight.y, myUp.y, myFront.y, 0.0f,
			myRight.z, myUp.z, myFront.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);



		return matL * matR;


	}*/
	void ProcessKeyboard(Camera_Movement dir, float deltime)
	{

		const float speedCam = 4.5f * deltime;

		if (dir == FORWARD)
			Position += Front * speedCam;
		if (dir == BACKWARD)
			Position -= Front * speedCam;
		if (dir == LEFT)
			Position -= Right * speedCam;
		if (dir == RIGHT)
			Position += Right * speedCam;

	}

	void CamDirectionMouse(double xoffset, double yoffset, bool firstMouse)
	{


		const float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		YAWcl += xoffset;
		PITCHcl += yoffset;

		if (firstMouse == true)
		{
			if (PITCHcl > 89.0f)
			{
				PITCHcl = 89.0f;
			}
			if (PITCHcl < -89.0f)
			{
				PITCHcl = -89.0f;
			}
		}

		CameraDirection();
	}
	void CamZoomFOV(float Yoffset)
	{
		if (FOVcl >= 1.0f && FOVcl <= 45.0f)
			FOVcl -= Yoffset;
		if (FOVcl <= 1.0f)
			FOVcl = 1.0f;
		if (FOVcl >= 45.0f)
			FOVcl = 45.0f;
	}
	float GetFOV()
	{
		return FOVcl;
	}

	glm::mat4 GetViewMat()
	{
		return glm::lookAt(Position, Position + Front, Up);

	}


private:
	void CameraDirection()
	{


		glm::vec3 direction;
		direction.x = cos(glm::radians(YAWcl)) * cos(glm::radians(PITCHcl));
		direction.y = sin(glm::radians(PITCHcl));
		direction.z = sin(glm::radians(YAWcl)) * cos(glm::radians(PITCHcl));
		Front = glm::normalize(direction);

		//CamDir = Position + Front;
		Right = glm::normalize(glm::cross(Front, upGlobal));
		Up = glm::cross(Right, Front);

	}



};




#endif