/*
 * Validator.h
 *
 *  Created on: Apr 4, 2019
 *      Author: afrig
 */

#ifndef VALIDATOR_H_
#define VALIDATOR_H_
#include <string>
#include "INIFile.h"
#include "DBLimits.h"
#include "TargetData.h"
#include "INIFile.h"
#include "PrimaryLimitsNew.h"
#include "Family.h"
#include "ResultMessage.h"
#include "Util.h"

class Validator
{
public:
	Validator(const std::string& file_ini)
			: m_ini(file_ini)
	{
		m_ini.Ini();
		pLogger = Logger::getInstance();
	}
	virtual ~Validator()
	{
	}
	void Validate();
	void CheckPresenceFiles();
	std::string cmdPath()
	{
		return m_ini.getValue("path_cmd");
	}
protected:
	INIFile m_ini;
	Logger* pLogger;
};

void Validator::Validate()
{
	DBLimits db = Primary_Limits_new::Verify(m_ini);
	Primary_Limits_new::GetTargetData(m_ini, db);
	Family::Verify(m_ini);

	CheckPresenceFiles();

}
void Validator::CheckPresenceFiles()
{
	std::string base_data_path = m_ini.getValue("base_data_path");
	std::string fileNames = m_ini.getValue("presence_files");
	std::vector<std::string> vec = Util::str_split(fileNames, ",");
	std::cout << "EXIST " << vec.size() << std::endl;
	for (std::string name : vec)
	{
		std::string file = base_data_path + "/" + name;
		if (-1 == Util::LastModificationTime(file.c_str()))
		{
			pLogger->error("ABSENT", file);
		}
	}

}
#endif /* VALIDATOR_H_ */
