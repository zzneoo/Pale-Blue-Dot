#pragma once
#include"stdafx.h"

class Camera
{
public:
	Camera();
	~Camera();

	void UpdateViewMatrix(HWND hwnd);
	void UpdateInfiniteViewMatrix(void);
	void UpdateLightViewMatrix(vmath::vec3 sunPos, vmath::vec3 Target);

	//getters
	GLfloat GetYaw(void)
	{
		return Yaw;
	}
	GLfloat GetPitch(void)
	{
		return Pitch;
	}

	vmath::vec3 GetCameraPos(void)
	{
		return CameraPosition;
	}
	void SetCameraPos(vmath::vec3 newPos)
	{
		 CameraPosition=vmath::vec3(newPos);
	}

	vmath::vec3 GetCameraDirection(void)
	{
		return CameraForward;
	}

	GLfloat GetCameraMagnitude(void)
	{
		return vmath::length(CameraPosition);
	}

	vmath::vec3 GetInitialCameraPosition(void)
	{
		return InitialCameraPosition;
	}

	vmath::mat4 GetViewMatrix(void)
	{
		return ViewMatrix;
	}

	vmath::mat4 GetLightViewMatrix(void)
	{
		return LightViewMatrix;
	}

	vmath::mat4 GetInfiniteViewMatrix(void)
	{
		return InfiniteViewMatrix;
	}

	vmath::vec3 GetCameraRightVector(void)
	{
		return CameraRight;
	}

	//setter
	void SetYaw(GLfloat fYaw)
	{
		Yaw = fYaw;
	}
	void SetPitch(GLfloat fPitch)
	{
		Pitch = fPitch;
	}

	void CameraTurboSpeed(GLuint Speed, GLboolean bSpaceGroundToggle);

private:
	vmath::vec3 CameraPosition;
	
	vmath::vec3 CameraForward;
	vmath::vec3 CameraRight;
	vmath::vec3 CameraUp;

	vmath::mat4 ViewMatrix;
	vmath::mat4 LightViewMatrix;
	vmath::mat4 InfiniteViewMatrix;

	const vmath::vec3 InitialCameraPosition=vmath::vec3(0.0f, 0.0f, 0.0f);

	GLfloat CameraSpeed ;

	GLfloat Yaw;
	GLfloat Pitch;
};

