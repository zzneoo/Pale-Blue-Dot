#include "OpenGLState.h"

OpenGLState openGLState;

Camera& OpenGLState::Getcamera()
{
	return m_camera;
}

mat4& OpenGLState::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}

SIZE& OpenGLState::GetWindowClientSize()
{
	return m_windowClientSize;
}
