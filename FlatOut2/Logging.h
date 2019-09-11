#pragma once
constexpr auto LOGGING_SHAREDBUFFER = L"Local\\FO2_SS_LogShared";
constexpr auto LOGGING_FILECREATEMUTEX = L"Local\\FO2_SS_LogCreateMutex";
constexpr auto LOGGING_THREADMUTEX = L"Local\\FO2_SS_LogThreadMutex";
constexpr auto LOGGING_WRITEMUTEX = L"Local\\FO2_SS_LogWriteMutex";
constexpr auto LOGGING_SIGNALLOGTHREAD = L"Local\\FO2_SS_LogWriteSignal";
constexpr auto LOGGING_BUFFERSIZE = 0x1000;

struct LoggingBuffer {
	DWORD writePos = 0;
	BYTE buffer[LOGGING_BUFFERSIZE - sizeof(DWORD)];
};

class Logging
{
public:
	static Logging& getInstance() {
		static Logging instance;
		return instance;
	}
	enum Level : int { Off, Error, Warn, Info, Debug, Trace };
	void error(const char* tag, std::string msg) { log(Level::Error, tag, msg); }
	void warn(const char* tag, std::string msg) { log(Level::Warn, tag, msg); }
	void info(const char* tag, std::string msg) { log(Level::Info, tag, msg); }
	void debug(const char* tag, std::string msg) { log(Level::Debug, tag, msg); }
	void trace(const char* tag, std::string msg) { log(Level::Trace, tag, msg); }
private:
	Logging();
	void log(Level level, const char* tag, std::string msg);
	void writeBuffer(std::string formattedMsg);
	static DWORD WINAPI threadEntry(LPVOID p) { return static_cast<Logging*>(p)->threadLoop(); }
	DWORD threadLoop();
	LoggingBuffer* m_sharedBuffer;
	HANDLE m_sharedBufferMapping;
	HANDLE m_createMappingsMutex;
	HANDLE m_bufferMutex;
	HANDLE m_loggerThreadMutex;
	HANDLE m_loggerThreadSignal;
public:
	Logging(Logging const&) = delete;
	void operator=(Logging const&) = delete;
};
