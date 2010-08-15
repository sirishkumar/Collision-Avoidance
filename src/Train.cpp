
#include <syslog.h>

#include "Common.h"
#include "Train.h"

#include <iostream>

Train::Train( TrainInfo & oTrainInfo )
 : m_u32Location(0),
   m_u32Speed(0),
   m_oTrainInfo( oTrainInfo )
{

}

void Train::Run(){

    std::cout<<"Running Train";
}

void Train::vUpdateLocation(unsigned int u32Location)
{
	m_u32Location =  u32Location;
}

unsigned int Train::u32GetSpeed() const{

	return m_u32Speed;
}

void Train::vSetSpeed(unsigned int u32Speed){

  m_u32Speed = u32Speed;
}

