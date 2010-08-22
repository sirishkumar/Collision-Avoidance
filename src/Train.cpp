
#include <syslog.h>

#include "Common.h"
#include "Train.h"
#include "ControlStation.h"

#include <iostream>


using namespace boost::interprocess;


Train::Train( TrainInfo & oTrainInfo , ControlStation *poControlStation)
 : m_u32CurrentLocation(0),
   m_u32Speed(0),
   m_oTrainInfo( oTrainInfo ),
   m_poControlStation( poControlStation )
{
	m_poMQ = new message_queue(create_only, oTrainInfo.szName.c_str(), 1024, 1024);
}

void Train::Run(){


	std::cout<<"Running Train";
}

void Train::vUpdateLocation(unsigned int u32Location)
{
	m_u32CurrentLocation =  u32Location;
}

unsigned int Train::u32GetSpeed() const{

	return m_u32Speed;
}

void Train::vSetSpeed(unsigned int u32Speed){

  m_u32Speed = u32Speed;
}


bool Train::bCheckWhetherTrainCanBeInstalled( TrainInfo & refoTrain ){
//
//	boost::mutex::scoped_lock & lock[4];
//
//	for( int i =0,vector<int>::const_iteratorit = refoTrain.m_lsPath.begin(); it != refoTrain.m_lsPath.end(),i< MIN_LOCKS_REQUIRED_TO_INSTALL; it++ ,i++){
//		lock[1] = m_poControlStation->bGetClearance(*it);
//    }

}
