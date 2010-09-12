
#ifndef _LocationSubSysSim
#define _LocationSubSysSim

#include "Timer.h"

class LocationSubSysSim
{
	LocationSubSysSim();
   static LocationSubSysSim m_oLocSim;
   static event_timer oTimer;

public:

   static LocationSubSysSim *getLocationSubSysInstance();
};



#endif
