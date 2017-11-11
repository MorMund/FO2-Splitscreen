#pragma once
#include <string>
#include <fstream>
#include <map>
#include <string.h>

typedef  void PressKeyCallback(DWORD key);

#define SR_ERROR 0
#define SR_SUCCESS 1
#define SR_WAIT 2
#define SR_EOF 3

class TestScriptRunner
{
public:
	TestScriptRunner(const wchar_t* scriptFile,PressKeyCallback pkCallback);
	~TestScriptRunner();
	void SetSetting(std::string setting, DWORD value);
	DWORD RunOnce();
private:
	std::ifstream m_scriptFile;
	int m_lineNum;
	ULONGLONG m_waitStart;
	int m_waitTime;
	std::unordered_map<std::string, DWORD> m_settings;
	PressKeyCallback* m_pkCallback;
};

