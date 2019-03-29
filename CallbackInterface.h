/*
 * CallbackInterface.h
 *
 *  Created on: Mar 27, 2019
 *      Author: afriger
 */

#ifndef CALLBACKINTERFACE_H_
#define CALLBACKINTERFACE_H_

class CallbackInterface
{
public:
	// The prefix "cbi" is to prevent naming clashes.
	virtual void cbiCallbackFunction(const std::vector<std::string>& vec, const std::string& str) = 0;
	virtual ~CallbackInterface()
	{
	}
};

#endif /* CALLBACKINTERFACE_H_ */
