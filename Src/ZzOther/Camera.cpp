#include "stdafx.h"
#include "Camera.h"
#include "MyWin32.h"

Camera::Camera()
{
	CameraSpeed = 5.0f;
	CameraPosition = InitialCameraPosition;

	vmath::vec3 CameraForward= vmath::vec3(0.0);
	vmath::vec3 CameraRight = vmath::vec3(0.0);
	vmath::vec3 CameraUp = vmath::vec3(0.0);

	ViewMatrix = vmath::mat4::identity();
	InfiniteViewMatrix = vmath::mat4::identity();


//	Yaw = 0.0f;
//	Pitch = -90.0f;

	Yaw = -13.041560f;
	Pitch = -24.790516f;
}


Camera::~Camera()
{
}

void Camera::CameraTurboSpeed(GLuint Speed ,GLboolean bSpaceGroundToggle)
{
	if (bSpaceGroundToggle)
	{
		switch (Speed)
		{
		case 0:
			CameraSpeed = 200.0f;
			break;
		case 1:
			CameraSpeed = 4000.0f;
			break;
		case 2:
			CameraSpeed = 8000.0f;
			break;
		case 3:
			CameraSpeed = 16000.0f;
			break;
		case 4:
			CameraSpeed = 64000.0f;
			break;
		case 5:
			CameraSpeed = 256000.0f;
			break;
		case 6:
			CameraSpeed = 1024000.0f;
			break;
		case 7:
			CameraSpeed = 4096000.0f;
			break;
		case 8:
			CameraSpeed = 16384000.0f;
			break;
		case 9:
			CameraSpeed = 64384000.0f;
			break;

		default:
			CameraSpeed = 8000.0f;
			break;
		}
	}
	else
	{
		switch (Speed)
		{
		case 0:
			CameraSpeed = 0.1f;
			break;
		case 1:
			CameraSpeed = 1.0f;
			break;
		case 2:
			CameraSpeed = 2.0f;
			break;
		case 3:
			CameraSpeed = 4.0f;
			break;
		case 4:
			CameraSpeed = 8.0f;
			break;
		case 5:
			CameraSpeed = 16.0f;
			break;
		case 6:
			CameraSpeed = 32.0f;
			break;
		case 7:
			CameraSpeed = 64.0f;
			break;
		case 8:
			CameraSpeed = 128.0f;
			break;
		case 9:
			CameraSpeed = 256.0f;
			break;

		default:
			CameraSpeed = 1.0f;
			break;
		}
	}

}

void Camera::UpdateViewMatrix(HWND hwnd)
{

	POINT ClientCenter = { MyWin32::myClientSize.ClientWidth / 2,MyWin32::myClientSize.ClientHeight / 2 };
	POINT ClientToScreenCenter = ClientCenter;
	ClientToScreen(hwnd, (LPPOINT)&ClientToScreenCenter);

	if ( TRUE== MyWin32::bFirstFrame)  //firstFrame is true if toggleFullscreen
	{
		MyWin32::myCamStruct.mouseX=(MyWin32::myClientSize.ClientWidth / 2);
		MyWin32::myCamStruct.mouseY=(MyWin32::myClientSize.ClientHeight / 2);
		MyWin32::bFirstFrame = FALSE;
	}

	SetCursorPos(ClientToScreenCenter.x, ClientToScreenCenter.y);

	GLfloat dx = (GLfloat)(MyWin32::myCamStruct.mouseX - ClientCenter.x);
	GLfloat dy = (GLfloat)(ClientCenter.y - MyWin32::myCamStruct.mouseY);

	GLfloat MouseSensitivity = MyOpenGL::fDeltaTime *7.0f;

	dx *= MouseSensitivity;
	dy *= MouseSensitivity;

	Yaw += dx;
	Pitch += dy;

	if (Pitch > 80.0f)
		Pitch = 80.0f;
	if (Pitch < -80.0f)
		Pitch = -80.0f;

	//Pi/180=0.01745329251

	GLfloat fPitch = Pitch* 0.01745329251f;
	GLfloat fYaw = Yaw * 0.01745329251f;

	vmath::vec3 front;
	front[0] = cosf(fPitch) * cosf(fYaw);
	front[1] = sinf(fPitch);
	front[2] = cosf(fPitch) * sinf(fYaw);

	vmath::vec3 up = vmath::vec3(0.0, 1.0, 0.0);

	CameraForward = vmath::normalize(front);
	CameraRight = vmath::normalize(vmath::cross(CameraForward,up));
	CameraUp = vmath::cross(CameraRight, CameraForward);


	if (MyWin32::myCamStruct.bCameraMoving == TRUE)
	{
		if (MyWin32::myCamStruct.PressedKey == 0x57)//W
		{
			CameraPosition += CameraForward * MyOpenGL::fDeltaTime * CameraSpeed;
		}
		if (MyWin32::myCamStruct.PressedKey == 0x53)//S
		{
			CameraPosition -= CameraForward * MyOpenGL::fDeltaTime * CameraSpeed;
		}
		if (MyWin32::myCamStruct.PressedKey == 0x41)//A
		{
			CameraPosition -= CameraRight * MyOpenGL::fDeltaTime * CameraSpeed;
		}
		if (MyWin32::myCamStruct.PressedKey == 0x44)//D
		{
			CameraPosition += CameraRight * MyOpenGL::fDeltaTime * CameraSpeed;
		}
	}

//	CameraPosition[1] = 0.0f;

	//ViewMatrix
	//short for matrix multiplication C(angles) x T(position)
	 GLfloat RdotE = vmath::dot(CameraRight, CameraPosition);
	 GLfloat UdotE = vmath::dot(CameraUp, CameraPosition);
	 GLfloat FdotE = vmath::dot(CameraForward, CameraPosition);


	ViewMatrix= vmath::mat4(vmath::vec4(CameraRight[0], CameraUp[0], CameraForward[0], 0.0),
						    vmath::vec4(CameraRight[1], CameraUp[1], CameraForward[1], 0.0),
						    vmath::vec4(CameraRight[2], CameraUp[2], CameraForward[2], 0.0),
						    vmath::vec4(vmath::vec3(-RdotE, -UdotE, -FdotE), 1.0));

	//ViewMatrix=vmath::lookat(CameraPosition, CameraPosition+ CameraForward, vmath::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::UpdateInfiniteViewMatrix(void)
{
	GLfloat RdotE = vmath::dot(CameraRight, InitialCameraPosition);
	GLfloat UdotE = vmath::dot(CameraUp, InitialCameraPosition);
	GLfloat FdotE = vmath::dot(CameraForward, InitialCameraPosition);

	InfiniteViewMatrix= vmath::mat4(vmath::vec4(CameraRight[0], CameraUp[0], -CameraForward[0], 0.0),
									vmath::vec4(CameraRight[1], CameraUp[1], -CameraForward[1], 0.0),
									vmath::vec4(CameraRight[2], CameraUp[2], -CameraForward[2], 0.0),
									vmath::vec4(vmath::vec3(-RdotE, -UdotE, FdotE), 1.0));
}

void Camera::UpdateLightViewMatrix(vmath::vec3 sunPos, vmath::vec3 Target)
{

	vmath::vec3 UniversalUp = vmath::vec3(0.0, 1.0, 0.0);

	vmath::vec3 SunForward = normalize(Target - sunPos);
	vmath::vec3 SunRight = vmath::normalize(vmath::cross(SunForward, UniversalUp));
	//vmath::vec3 SunUp = vmath::cross(SunRight, SunForward);
	vmath::vec3 SunUp = vmath::cross(SunRight, SunForward);

	GLfloat RdotS = vmath::dot(SunRight, sunPos);
	GLfloat UdotS = vmath::dot(SunUp, sunPos);
	GLfloat FdotS = vmath::dot(SunForward, sunPos);

	LightViewMatrix = vmath::mat4(vmath::vec4(SunRight[0], SunUp[0], SunForward[0], 0.0),
								  vmath::vec4(SunRight[1], SunUp[1], SunForward[1], 0.0),
								  vmath::vec4(SunRight[2], SunUp[2], SunForward[2], 0.0),
								  vmath::vec4(vmath::vec3(-RdotS, -UdotS, -FdotS), 1.0));
}

