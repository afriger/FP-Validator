/*
 * TargetData.h
 *
 *  Created on: Mar 18, 2019
 *      Author: afriger
 */

#ifndef TARGETDATA_H_
#define TARGETDATA_H_
#include <string>

const char* NullVal = "-7";

struct TargetData
{
	double Value, Lo, Hi, Md;
	TargetData(const std::string& value, const std::string& lo, const std::string& hi)
			: Value(Util::get_value(value))
					, Lo(Util::get_value(lo))
					, Hi(Util::get_value(hi))
					, Md((Hi + Lo) / 2)
	{
	}
	TargetData(const TargetData& data)
			: Value(data.Value), Lo(data.Lo), Hi(data.Hi), Md(data.Md)
	{
	}
	TargetData& operator =(const TargetData& data)
	{
		Value = data.Value;
		Lo = data.Lo;
		Hi = data.Hi;
		Md = data.Md;
		return *this;
	}
};

TargetData TargetDataNull(NullVal,"","");

#endif /* TARGETDATA_H_ */
