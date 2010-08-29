
#include <syslog.h>

#include "Common.h"
#include "Train.h"
#include "ControlStation.h"

#include <iostream>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/thread/condition.hpp>


Train::Train( TrainInfo & oTrainInfo , ControlStation *poControlStation)
 : m_u32CurrentLocation(0),
   m_u32Speed(0),
   m_oTrainInfo( oTrainInfo ),
   m_poControlStation( poControlStation ),
   m_bStopped(false)
{
}

void Train::Run(){


	std::cout<<"\nRunning Train";

	while( !m_bStopped){

		boost::mutex::scoped_lock lock(m_mutexQ);
        while( m_lsMessageQueue.empty() )
        {
        	m_condQ.wait(lock);
        }

        Message msg = m_lsMessageQueue.front();
        m_lsMessageQueue.pop();

        vProcessMessage(msg);
	}
}

void Train::vProcessMessage( Message &oMsg )
{
   if( oMsg.enType ==  MSG_LOCATION )
   {
	   LocationUpdate *poLocation = ( LocationUpdate * )oMsg.m_pomsg;
	   vUpdateLocation(poLocation->m_u32CurrentSection);
   }
}

void Train::vUpdateLocation(unsigned int u32Location)
{
	m_u32CurrentLocation =  u32Location;

	syslog (LOG_INFO," Location of Train %s : %d  ",  m_oTrainInfo.szName.c_str(), u32Location );
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


void Train::vPutMessageToQueue( Message &refMsg )
{

	boost::mutex::scoped_lock lock(m_mutexQ);
   	m_lsMessageQueue.push(refMsg);

	m_condQ.notify_one();

}
