/*
 * ResultMessage.h
 *
 *  Created on: Apr 4, 2019
 *      Author: afrig
 */

#ifndef RESULTMESSAGE_H_
#define RESULTMESSAGE_H_
#include <string>
#include <map>
#include <vector>

enum _eRate
{
	NOTHING = -1,
	FATAL = 0,
	WARNING = FATAL + 1,
	NOTICE = FATAL + 2

};

class ResultMessage
{

public:
	ResultMessage(_eRate rating, const std::string& prompt)
			: m_id(-1), m_rating(rating), m_prompt(prompt)
	{
	}
	virtual ~ResultMessage()
	{
	}
	ResultMessage(const ResultMessage& msg)
	{
		m_id = msg.m_id;
		m_rating = msg.m_rating;
		m_prompt.assign(msg.m_prompt);

	}
	ResultMessage& operator =(const ResultMessage& msg)
	{
		m_id = msg.m_id;
		m_rating = msg.m_rating;
		m_prompt.assign(msg.m_prompt);

		return *this;
	}
	int Id() const
	{
		return m_id;
	}
	_eRate Rating() const
	{
		return m_rating;
	}
	std::string Message() const
	{
		return m_prompt;
	}

protected:
	int m_id;
	_eRate m_rating;
	std::string m_prompt;
};

class Messages
{
public:
	Messages()
	{
	}
	virtual ~Messages()
	{
	}
	void AddMessage(const ResultMessage& msg)
	{
		m_db.insert({msg.Rating(),msg});
	}
	void AddMessage(_eRate rating, const std::string& prompt)
	{
		ResultMessage msg(rating, prompt);
		AddMessage(msg);
	}
	inline std::vector<ResultMessage> GetMessages(_eRate rate)
	{
		std::vector<ResultMessage> vec;
		size_t count = m_db.count(rate);
		if (count == 0)
		{
			return vec;
		}
		std::pair<std::multimap<_eRate, ResultMessage>::iterator, std::multimap<_eRate, ResultMessage>::iterator>pos;
		pos = m_db.equal_range(rate);
		for (; pos.first != pos.second; pos.first++ )
		{
			ResultMessage rec(pos.first->second);
			vec.push_back(rec);

		}
		return vec;
	}
protected:
	std::string m_tag;
	std::multimap<_eRate, ResultMessage> m_db;
};

#endif /* RESULTMESSAGE_H_ */
