#ifndef __CSV_RECORD_H_15032019
#define  __CSV_RECORD_H_15032019

#include <string>
#include <vector>
#include "Util.h"
#include "TargetData.h"
#include "Logger.h"

const char* Laser_Trim = "Laser_Trim";

class CSVRecordPrimaryLimits
{
protected:
	std::vector<std::string> m_record;
	std::vector<TargetData> m_data;
	Logger* pLogger;
	public:
	enum Index
	{
		N = 0,
		ResFam = N + 1,
		Type = N + 2,
		Tolerance = N + 3,
		Min = N + 4,
		Max = N + 5,
		Target = N + 6,
		CV_Limit = N + 7,
		Special = N + 8,
		Yield = N + 9,

	};

public:
	CSVRecordPrimaryLimits()
	{
		pLogger = Logger::getInstance();
	}
	virtual ~CSVRecordPrimaryLimits()
	{
	}
	CSVRecordPrimaryLimits(const CSVRecordPrimaryLimits& rec)
			: m_record(rec.m_record), m_data(rec.m_data)
	{
		pLogger = Logger::getInstance();
	}
	CSVRecordPrimaryLimits(const std::vector<std::string> & rec)
			: m_record(rec)
	{
		InitTargetData();
	}
	CSVRecordPrimaryLimits& operator =(const CSVRecordPrimaryLimits& rec)
	{
		m_record = rec.m_record;
		m_data = rec.m_data;
		return *this;
	}

	std::string GetValue(Index key) const
			{
		std::string t = m_record.at(key);
		return Util::trim_copy(t);
	}

	std::string GetType() const
	{
		return GetValue(Index::Type);
	}
	std::string GetTolerance() const
	{
		return GetValue(Index::Tolerance);
	}
	std::string GetTarget() const
	{
		return GetValue(Index::Target);
	}
	std::string GetFamily() const
	{
		return GetValue(Index::ResFam);
	}
	size_t GetHash() const
	{
		std::hash<std::string> hash;
		std::string mapKey = GetType() + GetTarget() + GetTolerance(); // + GetFamily();
		return hash(mapKey);

	}
	TargetData GetTargetData(double Value);
	void InitTargetData();

};

void CSVRecordPrimaryLimits::InitTargetData()
{
	std::string trg = GetTarget();
	if (!Util::isequals(trg, Laser_Trim))
	{
		return;
	}
	double val_prev = 0.0;
	std::string tid = GetType() + "-" + GetTarget();
	for (size_t k = Index::Yield + 1; k < m_record.size() - 3; k += 3)
	{

		std::string Val = m_record.at(k);
		double dVal = Util::get_value(Val);
		if (dVal!=-1 && val_prev >dVal )
		{
			if (pLogger)
			{
				std::stringstream ss;
				ss <<GetType()<<","<<GetTarget()<<","<<GetTolerance()<<",["<<val_prev<<'<'<<dVal<<']';
				pLogger->error("LESS", ss);
			}
		}
		val_prev = dVal;
		std::string Lo = m_record.at(k + 1);
		std::string Hi = m_record.at(k + 2);
		TargetData data(Val, Lo, Hi);
		m_data.push_back(data);
	}
	return;
}

TargetData CSVRecordPrimaryLimits::GetTargetData(double Value)
{
	for (TargetData data : m_data)
	{
		if (Value <= data.Value)
		{
			return data;
		}
	}
	return TargetDataNull;
}

#endif// __CSV_RECORD_H_15032019
