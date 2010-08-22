
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

class Message
{
public:
	enMessageType enType;
	void *m_pomsg;
};


class LocationUpdate
{
public:
	unsigned int Location;
};

#endif
