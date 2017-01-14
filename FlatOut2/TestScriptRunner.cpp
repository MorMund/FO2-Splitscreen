#include "stdafx.h"
#include "TestScriptRunner.h"


TestScriptRunner::TestScriptRunner(LPWSTR scriptFile, PressKeyCallback pkCallback)
{
	try {
		m_scriptFile.open(scriptFile, std::ios::in);
	}
	catch (std::ios_base::failure& e) {
		MessageBoxA(NULL, e.what(), NULL, MB_OK);
	}

	m_pkCallback = pkCallback;
	m_waitTime = 0;
}


TestScriptRunner::~TestScriptRunner()
{
	m_scriptFile.close();
}

void TestScriptRunner::SetSetting(std::string setting, DWORD value)
{
	m_settings[setting] = value;
}

ULONGLONG GetSysTime()
{
	SYSTEMTIME st;
	FILETIME ft;
	LARGE_INTEGER lit;
	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);
	lit.HighPart = ft.dwHighDateTime;
	lit.LowPart = ft.dwLowDateTime;
	return lit.QuadPart;
}

DWORD TestScriptRunner::RunOnce()
{
	if (m_waitTime)
	{
		ULONGLONG ct = GetSysTime() / 10000;
		ULONGLONG wt = m_waitStart / 10000;
		ULONGLONG timeDeltaMS = ct - wt;
		if (m_waitTime > timeDeltaMS)
		{
			return SR_WAIT;
		}
		else
		{
			m_waitTime = 0;
		}
	}

	int trueIfDepth = 0;
	int ifDepth = 0;
	while (true)
	{
		if (m_scriptFile.eof())
		{
			return SR_EOF;
		}
		else if (m_scriptFile.bad() || m_scriptFile.fail())
		{
			return SR_ERROR;
		}

		std::string str;
		std::getline(m_scriptFile, str);
		m_lineNum++;
		int falseIfDepth = ifDepth - trueIfDepth;
		if (falseIfDepth < -1)
		{
			throw new std::exception();
		}

		if (0 == str.compare("ENDIF"))
		{
			ifDepth--;
			if (0 == falseIfDepth)
			{
				trueIfDepth--;
			}		
		}
		else if ( 0 == str.compare("ELSE"))
		{
			if (0 < falseIfDepth)
			{
				trueIfDepth++;
			}
			else
			{
				trueIfDepth--;
			}
		}
		else if (falseIfDepth == 0)
		{
			if (0 == _strnicmp("IF", str.c_str(), 2))
			{
				ifDepth++;
				std::string exp = str.substr(3, str.npos);
				if (m_settings.count(exp) && m_settings[exp])
				{
					trueIfDepth++;
				}
			}
			else if (0 == _strnicmp("PRESS", str.c_str(), 5))
			{
				std::string exp = str.substr(6, str.npos);
				int key = std::stoi(exp);
				m_pkCallback(key);
				return SR_SUCCESS;
			}
			else if (0 == _strnicmp("WAIT", str.c_str(), 4))
			{
				std::string exp = str.substr(5, str.npos);
				int wait = std::stoi(exp);
				m_waitTime = wait;
				m_waitStart = GetSysTime();
				return SR_WAIT;
			}
			else if (0 == _strnicmp("PRINT", str.c_str(), 5))
			{
				std::string exp = str.substr(6, str.npos);
				std::cout << "[SCRIPT]" << exp << std::endl;
			}
		}
	}
}
