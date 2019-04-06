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
			ss << vec.at(CSVRecordPrimaryLimits::Type) << ",";
			ss << vec.at(CSVRecordPrimaryLimits::Target) << ",";
			ss << vec.at(CSVRecordPrimaryLimits::Tolerance) << ",";
			ss << str;
			pLogger->error("ODD CHARACTER", ss);
		}
	}
	std::vector<std::vector<std::string> > getData()
	{
		return m_reader.getData(this);
	}
	static DBLimits Verify(INIFile& ini);
	static void GetTargetData(INIFile& ini, DBLimits& db);

protected:
	Logger* pLogger;
	CSVReader m_reader;
};

DBLimits Primary_Limits_new::Verify(INIFile& ini)
{
	DBLimits db;
	std::string base_data_path = ini.getValue("base_data_path");
	std::string file_csv = base_data_path + "/" + ini.getValue("path_limits");
	std::string original = ini.getValue("original_limits");
	if (-1 == ini.checkUpdate(file_csv, original))
	{
		return db;
	}
	Primary_Limits_new reader(file_csv);
	std::vector<std::vector<std::string> > dataList = reader.getData();
	db.Counter(0);
	for (std::vector<std::string> record : dataList)
	{
		CSVRecordPrimaryLimits rec(record);
		db.Insert(rec);
	}
	return db;
}

void Primary_Limits_new::GetTargetData(INIFile& ini, DBLimits& db)
{
	std::string type = ini.getValue("type");
	std::string target = ini.getValue("target");
	std::string tolerance = ini.getValue("tolerance");
	size_t key = db.GetHash(type, target, tolerance);
	std::vector<CSVRecordPrimaryLimits> res = db.GetElements(key);

	std::cout << "KEYS COUNT: " << res.size() << std::endl;
	for (CSVRecordPrimaryLimits& r : res)
	{
		TargetData data = r.GetTargetData(0.051);
		std::cout << "data 0.051: [" << data.Value << "]" << data.Lo << ";" << data.Hi << ";" << data.Md;
		std::cout << '\n';
	}
}

#endif /* PRIMARYLIMITSNEW_H_ */
