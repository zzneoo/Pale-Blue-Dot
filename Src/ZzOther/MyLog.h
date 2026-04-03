#pragma once
class MyLog
{
public:
	MyLog();
	~MyLog();

	void PrintLog(char* log);
	void StartLog(const char* fileName);
	void EndLog(const char* fileName);
private:
	FILE* gpFile;
};

