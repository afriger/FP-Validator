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
#define  MAX_SIZE 10000000

Logger* Logger::m_Instance = 0;
const std::string logFileName = "fpvalidator.log";
const char* LeaveOld = "1";

Logger::Logger()
{
	m_bTrace = false;
	std::string newName = replaceFile();
	m_File.open(logFileName.c_str(), std::ios::out | std::ios::app);
	if (0 == newName.compare(LeaveOld))
	{
		return;
	}
	if (newName.empty())
	{
		error("REPLACE", "Error renaming file");

	}
	else
	{
		newName += ",file renamed successfully";
		error("REPLACE", newName);
	}
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

std::string Logger::replaceFile()
{

	std::ifstream::pos_type size = fileSize(logFileName.c_str());
	if (size < MAX_SIZE)
	{
		return LeaveOld;
	}
	std::string newFileName = getFileName();
	int result = rename(logFileName.c_str(), newFileName.c_str());
	if (result != 0)
	{
		return "";
	}

// clear data
	std::ofstream ofs;
	ofs.open("test.txt", std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	return newFileName;

}

void Logger::IntoFile(const std::string& data)
{
#if !__ECLIPSE_VI
	std::lock_guard<std::mutex> lock(m_mutex);
#endif
	if (bConsol)
	{
		std::cout << getCurrentTime() << "," << data << std::endl;
	} else
	{
		std::cout << getCurrentTime() << "," << data << std::endl;
		m_File << getCurrentTime() << "," << data << std::endl;
	}
}
std::string Logger::getFileName()
{
	return currentTime("%d-%m-%Y_%H%M%S.log");
}
std::string Logger::getCurrentTime()
{
	return currentTime("%d-%m-%Y %H:%M:%S");
}

std::ifstream::pos_type Logger::fileSize(const char* filename)
{
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}

std::string Logger::currentTime(const char* format)
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), format, timeinfo);
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
