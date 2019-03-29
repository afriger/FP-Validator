#ifndef __DB_LIMITS_H_16032019
#define  __DB_LIMITS_H_16032019
#include <map>
#include <iostream>
#include <sstream>

#include "CSVRecordPrimaryLimits.h"
#include "Logger.h"

class DBLimits
{

public:
	DBLimits();
	~DBLimits();
	bool Insert(const CSVRecordPrimaryLimits& rec);
	std::vector<CSVRecordPrimaryLimits> GetElements(size_t key);
	void Counter(int n)
	{
		m_counter = n;
	}
	int Counter() const
	{
		return m_counter;
	}
	size_t GetHash(const std::string& Type, const std::string& Target, const std::string& Tolerance)
	{
		std::hash<std::string> hash;
		std::string mapKey = Type + Target + Tolerance; // + GetFamily();
		return hash(mapKey);
	}
	size_t GetSize() const
	{
		return m_db.size();
	}
protected:
	std::multimap<size_t, CSVRecordPrimaryLimits> m_db;
	int m_counter;
	Logger* pLogger;
};

DBLimits::DBLimits()
		: m_counter(0)
{
	pLogger = Logger::getInstance();
}

DBLimits::~DBLimits()
{
}

inline std::vector<CSVRecordPrimaryLimits> DBLimits::GetElements(size_t key)
{
	std::vector<CSVRecordPrimaryLimits> vec;
	size_t count = m_db.count(key);
	if (count == 0)
	{
		return vec;
	}
	std::pair<std::multimap<size_t, CSVRecordPrimaryLimits>::iterator, std::multimap<size_t, CSVRecordPrimaryLimits>::iterator>pos;
	pos = m_db.equal_range(key);
	for (; pos.first != pos.second; pos.first++ )
	{
		CSVRecordPrimaryLimits rec(pos.first->second);
		vec.push_back(rec);

	}
	return vec;
}

inline bool DBLimits::Insert(const CSVRecordPrimaryLimits & rec)
{
	size_t hash = rec.GetHash();
	if (m_db.find(hash) != m_db.end())
	{
		// found
		m_counter++;
		std::stringstream ss;
		ss << rec.GetValue(CSVRecordPrimaryLimits::N) << ",";
		ss << rec.GetType() << ",";
		ss << rec.GetTarget() << ",";
		ss << rec.GetTolerance()<< ",";
		if (pLogger)
		{
			pLogger->error("DOUBLE", ss);
		}
	}
	m_db.insert( { hash, rec });
	return false;
}

#endif// __DB_LIMITS_H_16032019
