/*
 * PrimaryLimitsnew.h
 *
 *  Created on: Mar 27, 2019
 *      Author: afriger
 */

#ifndef PRIMARYLIMITSNEW_H_
#define PRIMARYLIMITSNEW_H_
#include <string>
#include <vector>
#include "CSVReader.h"
#include "CallbackInterface.h"

class Primary_Limits_new: public CallbackInterface
{
public:
	Primary_Limits_new(const std::string& filename)
			: m_reader(filename, ",")
	{
		pLogger = Logger::getInstance();
	}
	virtual ~Primary_Limits_new()
	{
	}
	virtual void cbiCallbackFunction(const std::vector<std::string>& vec, const std::string& str)
	{

		if (!str.empty())
			{

				std::stringstream ss;
				ss << vec.at(CSVRecordPrimaryLimits::N) << ",";
				ss << vec.at(CSVRecordPrimaryLimits::Type)<<",";
				ss << vec.at(CSVRecordPrimaryLimits::Target) << ",";
				ss << vec.at(CSVRecordPrimaryLimits::Tolerance)<< ",";
				ss << str;
				pLogger->error("ODD CHARACTER", ss);
			}
	}
	std::vector<std::vector<std::string> > getData()
	{
		return m_reader.getData(this);
	}


protected:
	Logger* pLogger;
	CSVReader m_reader;
};

#endif /* PRIMARYLIMITSNEW_H_ */
