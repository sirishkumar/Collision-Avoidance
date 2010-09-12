
#include <syslog.h>

#include "Common.h"
#include "Train.h"
#include "ControlStation.h"

#include <iostream>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/thread/condition.hpp>

using namespace std;


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

	std::vector<Section> & lsSections = m_poControlStation->lsGetSections();
	bLockNextComingJunction(lsSections,*m_oTrainInfo.m_lsPath.begin(),true);

	std::cout<<"\n"<<m_oTrainInfo.szName<< ": Running Train ";

	while( !m_bStopped){

		boost::mutex::scoped_lock lock(m_mutexQ);
        while( m_lsMessageQueue.empty() )
        {
        	m_condQ.wait(lock);
        }

        while( !m_lsMessageQueue.empty() )
        {
           Message msg = m_lsMessageQueue.front();
           m_lsMessageQueue.pop();

           vProcessMessage(msg);
        }
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

	std::cout<<"\n"<<m_oTrainInfo.szName<< ": Location Update "<< u32Location;

	//! If previous section was a junction release the lock
	if( lsSections[m_u32CurrentLocation].bGetIsJunction() && m_prCurrentJunctionLock )
	{
       m_prCurrentJunctionLock->vUnLock();
       m_prCurrentJunctionLock = 0;
       m_prCurrentJunctionLock = m_NextJunctionEntryLock;
       m_NextJunctionEntryLock = 0;
	}

	m_oTrainInfo.m_u32CurrentLocation =  m_u32CurrentLocation = u32Location;

	//! if the next section from current section is junction check whether exit junction is unlocked and also the next entry junction

	//! If there are no more sections for junction lock
	 vector<int>::const_iterator it = find(m_oTrainInfo.m_lsPath.begin(),m_oTrainInfo.m_lsPath.end(),m_u32CurrentLocation);

	if( m_oTrainInfo.m_lsPath.end() - it > 3  )
    {

		if( lsSections[*(it+1)].bGetIsJunction() )
		{
			//! Check whether we have exit lock else stop and wait for the junction lock, exit lock is after entry section
			std::cout<<"\n"<<m_oTrainInfo.szName<< ":  Check Exit lock for Junction "<<*(it+2);

			if( lsSections[*(it+2)].bTryLock(m_oTrainInfo) == true )
			{
				std::cout<<"\n"<<m_oTrainInfo.szName<< ":  Junction "<< *(it+2)<<" Not Locked. Free to proceed";
			}
			else
			{
				//! TO be sent to location subsystem
				vSetSpeed(0);
				std::cout<<"\n"<<m_oTrainInfo.szName<< ":  Junction "<<*(it+2) << " is locked. Waiting for lock";
				lsSections[*(it+2)].bLock(m_oTrainInfo);
				std::cout<<"\n"<<m_oTrainInfo.szName<< ":  Junction "<<*(it+2)<<" UnLocked. Free to proceed";
				lsSections[*(it+2)].vUnLock();
				//! TO be sent to location subsystem
				vSetSpeed(m_u32Speed);
			}

			bLockNextComingJunction(lsSections,*(it+3));

		}
    }

    //! Update Speed of train to location subsystem
    if( m_oTrainInfo.m_u32CurrentLocation != *(m_oTrainInfo.m_lsPath.end()-1))
    {
       enTrainSpeed enSpeed = m_poControlStation->enGetClearance(m_oTrainInfo);
       vSetSpeed(enSpeed);
    }
    else
    {
    	cout<<"\n"<<m_oTrainInfo.szName<< ":  Train "<< m_oTrainInfo.szName <<" reached destination";

    	//! If there are any locks release them
    	if( m_prCurrentJunctionLock )
    	       m_prCurrentJunctionLock->vUnLock();

       	if( m_NextJunctionEntryLock)
       		m_NextJunctionEntryLock->vUnLock();

       	//! Train has reached destination, stop tracking
       	m_bStopped = true;

    }


	syslog (LOG_INFO," Location of Train %s : %d  ",  m_oTrainInfo.szName.c_str(), u32Location );
}

bool Train::bLockNextComingJunction(std::vector<Section> & lsSections,unsigned int u32NextSection, bool bIsFirstCall )
{
	vector<int> &reflsPath = m_oTrainInfo.m_lsPath;

	std::vector<int>::const_iterator it = find(reflsPath.begin(),reflsPath.end(),u32NextSection);


	for( ;it != reflsPath.end();it++ )
	{
       if( lsSections[*it].bGetIsJunction() )
       {
           //! Lock entry lock of next junction
    	   lsSections[*it].bLock(m_oTrainInfo);
    	   if( bIsFirstCall )
    		   m_prCurrentJunctionLock = &lsSections[*it];
    	   else
    	       m_NextJunctionEntryLock = &lsSections[*it];
    	   cout<<"\n Locking next entry junction " <<*it;
    	   break;
       }
	}
}

unsigned int Train::u32GetSpeed() const{

	return m_u32Speed;
}

void Train::vSetSpeed(unsigned int u32Speed){

  cout<<"\n"<<m_oTrainInfo.szName<< ":  Speed of Train set to "<< u32Speed;
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
