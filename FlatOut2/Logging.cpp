#include "stdafx.h"
#include "Logging.h"

Logging::Logging() {
	if (InstanceSettings::GetSettings()->GetLogFileVerbosity() == Off) {
		return;
	}

	DWORD waitResult;
	m_createMappingsMutex = CreateMutex(NULL, FALSE, LOGGING_FILECREATEMUTEX);
	m_bufferMutex = CreateMutex(NULL, FALSE, LOGGING_WRITEMUTEX);
	m_loggerThreadSignal = CreateEvent(NULL, TRUE, FALSE, LOGGING_SIGNALLOGTHREAD);
	if (!m_createMappingsMutex || !m_bufferMutex || !m_loggerThreadSignal) {
		throw std::exception("Failed to create logging mutex.");
	}
	waitResult = WaitForSingleObject(m_createMappingsMutex, 0);
	if (waitResult == WAIT_OBJECT_0) {
		// The first process has to create the shared memory
		m_sharedBufferMapping = CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(LoggingBuffer),
			LOGGING_SHAREDBUFFER);
	}
	else {
		m_sharedBufferMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, LOGGING_SHAREDBUFFER);
	}

	if (!m_sharedBufferMapping) {
		throw std::exception("Failed to create logging shared memory mapping!");
	}

	m_sharedBuffer = static_cast<LoggingBuffer*>(MapViewOfFile(m_sharedBufferMapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(LoggingBuffer)));
	if (!m_sharedBuffer) {
		throw std::exception("Failed to map view of logging shared memory!");
	}

	// Create a logging thread for each instance so fail over is ensured if an instance exits
	DWORD id = 0;
	CreateThread(NULL, 0, threadEntry, this, 0, &id);
}

void Logging::log(Logging::Level level, const char* tag, std::string msg) {
	std::ostringstream formattedMsg;
	auto settings = InstanceSettings::GetSettings();
	formattedMsg << '[' << tag << "|" << settings->GetInstanceID() << "] " << msg << std::endl;
	if (InstanceSettings::GetSettings()->GetLogFileVerbosity() >= level) {
		writeBuffer(formattedMsg.str());
	}

	if (InstanceSettings::GetSettings()->GetConsoleVerbosity() >= level) {
		std::cout << formattedMsg.str();
	}
}

void Logging::writeBuffer(std::string formattedMsg) {
	DWORD waitResult;
	auto msgBuffer = formattedMsg.c_str();
	int copyPos = 0;
	waitResult = WaitForSingleObject(m_bufferMutex, INFINITE);
	while (copyPos < formattedMsg.length()) {
		SetEvent(m_loggerThreadSignal);
		auto availableSpace = sizeof(m_sharedBuffer->buffer) - m_sharedBuffer->writePos;
		if (availableSpace > 0) {
			auto bytesToWrite = min(availableSpace, formattedMsg.length() - copyPos);
			memcpy(m_sharedBuffer->buffer + m_sharedBuffer->writePos, msgBuffer + copyPos, bytesToWrite);
			copyPos += bytesToWrite;
			m_sharedBuffer->writePos += bytesToWrite;
		}
		else {
			// Release the mutex so the logging thread has a chance to write out the full buffer
			ReleaseMutex(m_bufferMutex);
			waitResult = WaitForSingleObject(m_bufferMutex, INFINITE);
		}
		
	}

	ReleaseMutex(m_bufferMutex);

}

DWORD Logging::threadLoop() {
	DWORD waitResult;
	m_loggerThreadMutex = CreateMutex(NULL, FALSE, LOGGING_THREADMUTEX);
	waitResult = WaitForSingleObject(m_loggerThreadMutex, INFINITE);
	HANDLE logFile = CreateFile(L"log.txt", FILE_GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	for (;;) {
		BYTE ioBuffer[sizeof(m_sharedBuffer->buffer)];
		DWORD bytesToWrite;
		DWORD bytesWritten = 0;
		waitResult = WaitForSingleObject(m_loggerThreadSignal, INFINITE);
		waitResult = WaitForSingleObject(m_bufferMutex, INFINITE);
		bytesToWrite = m_sharedBuffer->writePos;
		memcpy_s(ioBuffer, sizeof(ioBuffer), m_sharedBuffer->buffer, m_sharedBuffer->writePos);
		m_sharedBuffer->writePos = 0;
		ZeroMemory(m_sharedBuffer->buffer, sizeof(m_sharedBuffer->buffer));
		ResetEvent(m_loggerThreadSignal);
		ReleaseMutex(m_bufferMutex);
		WriteFile(logFile, ioBuffer, bytesToWrite, &bytesWritten, NULL);	
	}
}