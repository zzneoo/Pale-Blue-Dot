#include "RayCaster.h"
#include"MyWin32.h"

GLfloat RayCaster::fUniformScale = 0.0f;

GLfloat RayCaster::fRotAngleX = 0.0f;
GLfloat RayCaster::fRotAngleY = 0.0f;
GLfloat RayCaster::fRotAngleZ = 0.0f;
vmath::vec3 RayCaster::WorldPos = vmath::vec3(0.0f);

vmath::vec3 RayCaster::ViewportToNDC(GLfloat depth)
{
	GLfloat x = (MyWin32::myCamStruct.mouseX * 2.0f) / MyWin32::myClientSize.ClientWidth - 1.0f;
	GLfloat y = ((MyWin32::myClientSize.ClientHeight - MyWin32::myCamStruct.mouseY) * 2.0f) / MyWin32::myClientSize.ClientHeight - 1.0f;
	GLfloat z = depth ;

	return vmath::vec3(x, y, z);
}

vmath::vec4 RayCaster::NDC_SpaceToWorldSpace(vmath::vec3 NDC)
{
	vmath::vec4 worldSpace = vmath::vec4(NDC, 1.0) * vmath::inverse(MyWin32::gProjectionMatrix).transpose() *vmath::inverse(MyWin32::gViewMatrix).transpose();
	worldSpace /= worldSpace[3];
	return worldSpace;
}

void RayCaster::getWorldSpacePosFromScreenSpace()
{
	GLfloat depth = 0.0f;
	glReadnPixels(MyWin32::myCamStruct.mouseX, MyWin32::myClientSize.ClientHeight-MyWin32::myCamStruct.mouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT,sizeof(GLfloat), &depth);

	//char str[255];
	//sprintf_s(str, "%f", depth);
	//SetWindowText(hwnd, str);

	vmath::vec4 worldSpace=  NDC_SpaceToWorldSpace(ViewportToNDC(depth));

	WorldPos = vmath::vec3(worldSpace[0], worldSpace[1], worldSpace[2]);
}
