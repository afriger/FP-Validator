#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Logger.h"
#include <mutex>
#include <thread>

#if !__ECLIPSE_VI
#pragma warning(disable : 4996)
#endif

#define bConsol 0

Logger* Logger::m_Instance = 0;
const std::string logFileName = "fpvalidator.log";

Logger::Logger()
{
	m_bTrace = false;
	m_File.open(logFileName.c_str(), std::ios::out | std::ios::app);
}

Logger::~Logger()
{
	m_File.close();
}

Logger* Logger::getInstance() throw ()
{
	if (m_Instance == 0)
	{
		m_Instance = new Logger();
	}
	return m_Instance;
}

void Logger::IntoFile(std::string& data)
{
#if !__ECLIPSE_VI
	std::lock_guard<std::mutex> lock(m_mutex);
#endif
	if (bConsol)
	{
		std::cout << getCurrentTime() << "," << data << std::endl;
	} else
	{
		m_File << getCurrentTime() << "," << data << std::endl;
	}
}

std::string Logger::getCurrentTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
	std::string str(buffer);
	return str;
}

// Interface for Error Log
void Logger::error(const char* prompt, const char* text) throw ()
{
	std::string data;
	data.append("[");
	data.append(prompt);
	data.append("],");
	data.append(text);
	//std::cout << data;
	IntoFile(data);

}

void Logger::error(const char* prompt, std::string& text) throw ()
{
	error(prompt, text.data());
}

void Logger::error(const char* prompt, std::ostringstream& stream) throw ()
{
	std::string text = stream.str();
	error(prompt, text.data());
}

void Logger::error(const char * prompt, std::stringstream & ss) throw ()
{
	std::string str = ss.str();
	error(prompt, str);
}
