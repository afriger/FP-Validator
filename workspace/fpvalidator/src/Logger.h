#ifndef _LOGGER_H_
#define _LOGGER_H_

// C++ Header File(s)
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <errno.h>
#include <mutex>
#include <sstream>
#if !__ECLIPSE_VI
#pragma warning(disable : 4996)
#endif

// Direct Interface for logging into log file or console using MACRO(s)
#define LOG_ERROR(x)	Logger::getInstance()->error(x)

class Logger
{
public:
	static Logger* getInstance() throw ();

	void error(const char* prompt, const char* text) throw ();
	void error(const char* prompt, std::string& text) throw ();
	void error(const char* prompt, std::ostringstream& stream) throw ();
	void error(const char* prompt, std::stringstream& ss) throw ();

	void trace(const char* prompt, const char* text) throw ()
	{
		if (m_bTrace)
		{
			error(prompt, text);
		}
	}
	void trace(const char* prompt, std::string& text) throw ()
	{
		if (m_bTrace)
		{
			error(prompt, text);
		}
	}
	void trace(const char* prompt, std::ostringstream& stream) throw ()
	{
		if (m_bTrace)
		{
			error(prompt, stream);
		}
	}
	void trace(const char* prompt, std::stringstream& ss) throw ()
	{
		if (m_bTrace)
		{
			error(prompt, ss);
		}
	}
	void Trace(bool b)
	{
		m_bTrace = b;
	}
protected:
	Logger();
	~Logger();
	std::string getCurrentTime();

private:
	bool m_bTrace;
	void IntoFile(std::string& data);
	Logger(const Logger& obj)
			: m_bTrace(obj.m_bTrace)
	{
	}
	Logger operator=(const Logger& obj)
	{
		m_bTrace = obj.m_bTrace;
		return *this;
	}

private:
	static Logger* m_Instance;
	std::ofstream m_File;
#if !__ECLIPSE_VI
	std::mutex m_mutex;
#endif

};

#endif // End of _LOGGER_H_

