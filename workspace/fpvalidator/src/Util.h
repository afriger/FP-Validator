#ifndef __UTIL_H_15032019
#define  __UTIL_H_15032019
#include <algorithm> 
#include <cctype>
#include <locale>
#include <string>
#include <sstream>

#include <sys/stat.h>
#include <time.h>
#include <ctime>
class Util
{
public:
	// trim from start (in place)
	static inline void ltrim(std::string &s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch)
		{
			return !std::isspace(ch);
		}));
	}

	// trim from end (in place)
	static inline void rtrim(std::string &s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch)
		{
			return !std::isspace(ch);
		}).base(), s.end());
	}

	// trim from both ends (in place)
	static inline void trim(std::string &s)
	{
		ltrim(s);
		rtrim(s);
	}

	// trim from start (copying)
	static inline std::string ltrim_copy(std::string s)
	{
		ltrim(s);
		return s;
	}
	// trim from end (copying)
	static inline std::string rtrim_copy(std::string s)
	{
		rtrim(s);
		return s;
	}
	static std::string trim_copy(std::string s)
	{
		std::string t = ltrim_copy(s);
		return rtrim_copy(t);
	}

	static double get_value(const std::string& str)
	{
		std::string s(str);
		std::transform(s.begin(), s.end(), s.begin(), toupper);

		if (s.empty() || s.length() == 0)
		{
			return -1;
		}
		size_t found = s.find("R");
		if (found != std::string::npos)
		{
			std::replace(s.begin(), s.end(), 'R', '.');
			double d = str_to_double(s);
			return d;
		}
		found = s.find("K");
		if (found != std::string::npos)
		{
			std::replace(s.begin(), s.end(), 'K', '.');
			double d = str_to_double(s);
			return d * 1000;
		}
		found = s.find("M");
		if (found != std::string::npos)
		{
			std::replace(s.begin(), s.end(), 'M', '.');
			double d = str_to_double(s);
			return d * 1000000;

		}
		double d = str_to_double(s);
		return d;

	}
	static double str_to_double(const std::string& str)
	{
		std::string s(str);
		std::stringstream ss(s);
		double d;
		ss >> d;
		return d;
	}
	static bool isequals(const std::string& a, const std::string& b)
	{
		return std::equal(a.begin(), a.end(),
				b.begin(), b.end(),
				[](char a, char b)
				{
					return tolower(a) == tolower(b);
				});
	}

	static std::string time_to_string(const std::time_t& t)
	{
		struct tm * timeinfo;
		char buffer[80];
		timeinfo = localtime(&t);
		strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
		std::string str(buffer);
		return str;
	}
};
#endif//__UTIL_H_15032019

