#pragma once
#include "common.h"
#include "Camera.h"

//Sahil&Ajinkya trial  (**Not my file,Unused)

class OpenGLState
{
	Camera m_camera;
	mat4 m_ProjectionMatrix;//projects from world coords to NDC
	SIZE m_windowClientSize = {0,0};
public:
	Camera& Getcamera();

	mat4& GetProjectionMatrix();

	SIZE& GetWindowClientSize();
};

extern OpenGLState openGLState;