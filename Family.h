/*
 * Family.h
 *
 *  Created on: Mar 28, 2019
 *      Author: afrig
 */

#ifndef FAMILY_H_
#define FAMILY_H_
#include <map>
#include "Logger.h"
#include "CSVReader.h"
#include "INIFile.h"
#include "ResultMessage.h"

class Family: public CallbackInterface
{
	enum Index
	{
		null = 0,
		Family_Id = null,
		Prod_id = null + 1,
		dbc_Default = null + 2,
		Length_mil = null + 3,
		Width_mil = null + 4,
		Length_mm = null + 5,
		Width_mm = null + 6,
		Rows = null + 7,
		Columns = null + 8,
		pitch = null + 9,
		hight = null + 10,
		Special_Type = null + 11,
		Factor = null + 12,
		RankCorrection = null + 13,
		ProbeFactor = null + 14
	};
	public:
	Family(const std::string& filename)
			: m_reader(filename, ","), m_counter(0)
	{
		pLogger = Logger::getInstance();
	}
	virtual ~Family()
	{
	}
	static void Verify(INIFile& ini)
	{
		std::string base_data_path = ini.getValue("base_data_path");
		std::string family_file = base_data_path + "/" + ini.getValue("path_family");
		std::string family_original = ini.getValue("original_family");
		ini.checkUpdate(family_file, family_original);
		Family family(family_file);
		family.getData();
	}

	virtual void cbiCallbackFunction(const std::vector<std::string>& vec, const std::string& str)
	{

		if (!str.empty())
		{

			std::stringstream ss;
			ss << m_counter << ",";
			ss << vec.at(Family::Family_Id) << ",";
			ss << vec.at(Family::Prod_id) << ",";
			ss << vec.at(Family::dbc_Default) << ",";
			ss << str;
			pLogger->error("ODD CHARACTER", ss);
		}
		Insert(vec);
	}
	bool Insert(const std::vector<std::string>& vec)
	{
		m_counter++;
		size_t hash = GetHash(vec.at(Family::Family_Id), vec.at(Family::Prod_id), vec.at(Family::dbc_Default));
		if (m_map.find(hash) != m_map.end())
		{
			// found

			std::stringstream ss;
			ss << m_counter << ",";
			ss << vec.at(Family::Family_Id) << ",";
			ss << vec.at(Family::Prod_id) << ",";
			ss << vec.at(Family::dbc_Default) << ",";

			if (pLogger)
			{
				pLogger->error("DOUBLE", ss);
			}
		}
		m_map.insert( { hash, vec });
		return false;
	}
	void getData()
	{
		m_counter = 0;
		m_reader.getVector(this);
	}

	size_t GetHash(const std::string& family_id, const std::string& prod_id, const std::string& dbc_default)
	{
		std::hash<std::string> hash;
		std::string mapKey = family_id + dbc_default; //+*//* prod_id+ */ dbc_default;
		return hash(mapKey);
	}

protected:
	Logger* pLogger;
	CSVReader m_reader;
	std::map<size_t, std::vector<std::string>> m_map;
	int m_counter;
};

#endif /* FAMILY_H_ */
