/*
 * CSVReader.h
 *
 *  Created on: Mar 2, 2019
 *      Author: afrig
 * A class to read data from a csv file.
 */

#ifndef CSVREADER_H_
#define CSVREADER_H_
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <iterator>
#include <string>
#include <algorithm>
#include <sstream>

#include <sys/stat.h>
#include <unistd.h>
#include "time.h"

#include "Logger.h"
#include "CallbackInterface.h"
#include "CSVRecordPrimaryLimits.h"

//const char* fileOriginal = "H:/Production/FlyProbe_Monitor/base_data/Primary_Limits_new.csv";

std::string gInvalidChar;
bool static invalidChar(char c)
{
	bool ret = !(c >= 0 && c < 128);
	if (ret)
	{
		std::stringstream ss;
		ss << std::hex << static_cast<int>(static_cast<unsigned char>(c));
		std::string temp = ss.str();
//		gInvalidChar += "0x" + ss.str();
		gInvalidChar += ss.str();
	}
	return ret;
}

class CSVReader
{
	std::string m_fileName;
	std::string m_delimeter;
	Logger* pLogger;

public:
	CSVReader(const std::string& filename, const std::string& delm)
			: m_fileName(filename), m_delimeter(delm)
	{
		pLogger = Logger::getInstance();
	}
	virtual ~CSVReader()
	{
	}
	std::string fileName() const
	{
		return m_fileName;
	}
public:
	// Function to fetch data from a CSV File
	std::map<std::string, std::string> getPairs();
	std::vector<std::vector<std::string> > getData(CallbackInterface* cb);
	void getVector(CallbackInterface* cb);
	std::time_t LastModificationTime(const char* file) const;
	void checkUpdate(const std::string& current, const std::string& original) const;
	protected:
	std::string StripInvalidCharacters(std::string& str);
	std::vector<std::string> split(std::string& inp, std::string delimeter, std::string& InvalidCharacters);
	std::vector<std::string> split(std::string& inp, std::string delimeter);
};

void CSVReader::checkUpdate(const std::string& current, const std::string& original) const
		{
	std::time_t result = std::time(nullptr);
	std::time_t org = LastModificationTime(original.c_str());
	std::time_t cur = LastModificationTime(current.c_str());
	std::cout << "TIME:[" << Util::time_to_string(result) << "]" << Util::time_to_string(org) << "<>" << Util::time_to_string(cur) << ";" << std::endl;

	if (org > cur)
	{
		std::stringstream ss;
		ss << ("File \"") << current << "\" is not up to date.";
		pLogger->error("UPDATED", ss);
	}

}
std::time_t CSVReader::LastModificationTime(const char* file) const
{
	struct stat buf;
	if (!stat(file, &buf))
	{
		return buf.st_mtime;
	}
	std::stringstream ss;
	ss << ("Can't get (atime) ") << file;
	pLogger->error("ERROR", ss);
	return -1;
}

std::map<std::string, std::string> CSVReader::getPairs()
{
	std::ifstream file(m_fileName);
	std::map<std::string, std::string> map;
	std::string line = "";
	// Iterate through each line and split the content using delimeter
	while (getline(file, line))
	{
		std::vector<std::string> vec;
		vec = split(line, m_delimeter);
		if (vec.size() == 2)
		{
			map.insert( { Util::trim_copy(vec.at(0)), Util::trim_copy(vec.at(1)) });
			//std::cout << Util::trim_copy(vec.at(0)) << "=" << Util::trim_copy(vec.at(1)) << std::endl;
		}
	}
	if (file.bad())
	{
		std::stringstream ss;
		ss << "\"" << m_fileName << "\"" << " IO error";
		pLogger->error("ERROR", ss);
	} else if (!file.eof())
	{
		std::stringstream ss;
		ss << "\"" << m_fileName << "\"" << " format error.";
		pLogger->error("ERROR", ss);
	} else
	{
	}

	// Close the File
	file.close();
	return map;
}
std::vector<std::vector<std::string> > CSVReader::getData(CallbackInterface* cb)
{
	std::ifstream file(m_fileName);
	std::vector<std::vector<std::string> > dataList;
	std::string line = "";
	// Iterate through each line and split the content using delimeter
	std::string InvalidCharacters;
	while (getline(file, line))
	{
		std::vector<std::string> vec;
		vec = split(line, m_delimeter, InvalidCharacters);

		if (cb)
		{
			cb->cbiCallbackFunction(vec, InvalidCharacters);
		}
		dataList.push_back(vec);

	}
	// Close the File
	file.close();
	return dataList;
}
void CSVReader::getVector(CallbackInterface* cb)
{
	std::ifstream file(m_fileName);
	std::vector<std::vector<std::string> > dataList;
	std::string line = "";
	// Iterate through each line and split the content using delimeter
	std::string InvalidCharacters = "";
	while (getline(file, line))
	{
		std::vector<std::string> vec;
		vec = split(line, m_delimeter, InvalidCharacters);
		if (cb)
		{
			cb->cbiCallbackFunction(vec, InvalidCharacters);
		}
	}
	// Close the File
	file.close();
	return;
}

inline std::string CSVReader::StripInvalidCharacters(std::string & str)
{
	std::string ret;
	gInvalidChar.clear();
	str.erase(remove_if(str.begin(), str.end(), invalidChar), str.end()); //Strip non-ASCII Characters from string
	if (!gInvalidChar.empty())
	{
		ret += '[' + str + gInvalidChar + ']';
	}
	return ret;
}

std::vector<std::string> CSVReader::split(std::string& inp, std::string delimeter)
{
	std::string InvalidCharacters;
	return split(inp, delimeter, InvalidCharacters);
}

std::vector<std::string> CSVReader::split(std::string& stringToBeSplitted, std::string delimeter, std::string& InvalidCharacters)
{
	InvalidCharacters.assign("");
	std::vector<std::string> splittedString;
	if (stringToBeSplitted.empty() || stringToBeSplitted.length() < 3)
	{
		return splittedString;
	}

	size_t startIndex = 0;
	size_t endIndex = 0;
	while ((endIndex = stringToBeSplitted.find(delimeter, startIndex)) < stringToBeSplitted.size())
	{
		std::string val = stringToBeSplitted.substr(startIndex, endIndex - startIndex);
		InvalidCharacters += StripInvalidCharacters(val);
		Util::trim(val);
		splittedString.push_back(val);
		startIndex = endIndex + delimeter.size();
	}
	if (startIndex < stringToBeSplitted.size())
	{
		std::string val = stringToBeSplitted.substr(startIndex);
		InvalidCharacters += StripInvalidCharacters(val);
		Util::trim(val);
		splittedString.push_back(val);
	}

	return splittedString;
}

#endif /* CSVREADER_H_ */
