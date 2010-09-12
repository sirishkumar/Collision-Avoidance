
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
   m_bStopped(false),
   m_prCurrentJunctionLock(0),
   m_NextJunctionEntryLock(0)
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
	std::vector<Section> & lsSections = m_poControlStation->lsGetSections();

	//! If previous section was a junction release the lock
	if( lsSections[m_u32CurrentLocation].bGetIsJunction() && m_prCurrentJunctionLock )
	{
       m_prCurrentJunctionLock->vUnLock();
       m_prCurrentJunctionLock = 0;
	}

	m_u32CurrentLocation =  u32Location;

	//! if the second section from current section is junction lock the exit junction and also the next entry junction
    if(  m_u32CurrentLocation+2 <= *(m_oTrainInfo.m_lsPath.end()-1 ) )
    {

		if( lsSections[m_u32CurrentLocation+2].bGetIsJunction() )
		{
			//! Check whether we have exit lock else stop and wait for the junction lock
			std::cout<<"\n Check lock for Junction "<<m_u32Id;

			if( lsSections[m_u32CurrentLocation+3].bTryLock(m_oTrainInfo) == true )
			{
				std::cout<<"\n Junction "<<m_u32Id<<" Not Locked. Free to proceed";
			}
			else
			{
				//! TO be sent to location subsystem
				vSetSpeed(0);
				std::cout<<"\n Junction "<<m_u32Id << " is locked. Waiting for lock";
				lsSections[m_u32CurrentLocation+3].bLock(m_oTrainInfo);
				std::cout<<"\n Junction "<<m_u32Id<<" UnLocked. Free to proceed";
				lsSections[m_u32CurrentLocation+3].vUnLock();
				//! TO be sent to location subsystem
				vSetSpeed(m_u32Speed);
			}

			bLockNextComingJunction(lsSections);

		}
    }

    //! Update Speed of train to location subsystem
    enTrainSpeed enSpeed = m_poControlStation->enGetClearance();
    vSetSpeed(enTrainSpeed);


	syslog (LOG_INFO," Location of Train %s : %d  ",  m_oTrainInfo.szName.c_str(), u32Location );
}

bool Train::bLockNextComingJunction(std::vector<Section> & lsSections)
{
	vector<int> &reflsPath = m_oTrainInfo.m_lsPath;

	//! +4 is next non-junction section
	for( vector<int>::const_iterator it = reflsPath.begin()+4 ;it != reflsPath.end();it++ )
	{
       if( lsSections[*it].bGetIsJunction() )
       {

       }
	}
}

unsigned int Train::u32GetSpeed() const{

	return m_u32Speed;
}

void Train::vSetSpeed(unsigned int u32Speed){

  cout<<"\n Speed of Train set to "<< u32Speed;
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
