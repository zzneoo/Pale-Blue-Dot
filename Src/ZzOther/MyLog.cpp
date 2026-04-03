#include "stdafx.h"
#include "MyLog.h"


MyLog::MyLog()
{
	this->gpFile = nullptr;
}


MyLog::~MyLog()
{
	if (gpFile)
	{
		fclose(gpFile);
		gpFile = nullptr;
	}
}

void MyLog::PrintLog(char* log)
{
	fprintf(gpFile, "%s", log);
}

void MyLog::StartLog(const char* fileName)
{

	if (fopen_s(&gpFile, fileName, "w") != 0)
	{
		TCHAR str[1024];
		wsprintf(str, "cannot open %s", fileName);
		MessageBox(NULL, str, TEXT("ERROR"), MB_OK | MB_TOPMOST);//MB_OK
	}
	else if(gpFile)
	{
			fprintf(gpFile, "%s is successfully opened\n", fileName);		
	}
}
void MyLog::EndLog(const char* fileName)
{
	if (gpFile)
	{
		fprintf(gpFile, "%s Is Successfully Closed.\n", fileName);
		fclose(gpFile);
		gpFile = nullptr;
	}
}
