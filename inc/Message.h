
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "Common.h"


class Message
{
public:
	enMessageType enType;
	void *m_pomsg;
};


class LocationUpdate
{
public:
	unsigned int m_u32CurrentSection;
};

#endif
