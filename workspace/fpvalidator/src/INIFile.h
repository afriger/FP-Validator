/*
 * INIFile.h
 *
 *  Created on: Mar 27, 2019
 *      Author: afriger
 */

#ifndef INIFILE_H_
#define INIFILE_H_
#include <string>
#include <map>
#include "CSVReader.h"
class INIFile
{
public:
	INIFile(const std::string& filename)
			: m_reader(filename, "=")
	{
	}
	virtual ~INIFile()
	{
	}
	void Ini()
	{
		m_map = m_reader.getPairs();
	}
	std::string getValue(const std::string& name)
	{
		std::map<std::string, std::string>::iterator it = m_map.find(name);
		if (it != m_map.end())
		{
			return it->second;
		}
		return "";
	}
	void checkUpdate(const std::string& current, const std::string& original) const
	{
		m_reader.checkUpdate(current,original);
	}
protected:
	CSVReader m_reader;
	std::map<std::string, std::string> m_map;
};

#endif /* INIFILE_H_ */
