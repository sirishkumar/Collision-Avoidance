
#include <iostream>
#include <boost/thread.hpp>

#include "ControlStation.h"
#include "Common.h"
#include "Train.h"
#include <boost/interprocess/ipc/message_queue.hpp>
using namespace boost::interprocess;

using namespace std;


ControlStation::ControlStation( std::string szName, std::vector<Section> &refolsSections  ):
m_szName( szName ),
m_lsTracks( refolsSections ){

	m_lsClearanceLock = new boost::mutex;
}

ControlStation::~ControlStation()
{
	m_posimASIOThread->join();
}

bool ControlStation::bInitControlStation()
{
	m_et = new event_timer<ControlStation>(io_service,1,1,this);
	boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&asio::io_service::run, &io_service)));
	m_posimASIOThread = thread.get();
	m_et->start();
}

enTrainInstallStatus ControlStation::enInstallTrain( TrainInfo & oTrainInfo ){

	boost::lock_guard<boost::mutex> lock(*m_lsClearanceLock);

	enTrainInstallStatus enInsStatus = TR_INS_FAILURE;
	enTrainSpeed enSpeed;

	syslog (LOG_INFO,"ControlStation::enInstallTrain:%s ", oTrainInfo.szName.c_str());

	if( m_lsTrains.find( oTrainInfo.szName.c_str() ) == m_lsTrains.end() )
	{

		enSpeed = enGetClearance(oTrainInfo);

        printf("\n Control Station: Add Train To Network -> GetClearance for Train %s With First Four Sections %d %d %d %d",
        		oTrainInfo.szName.c_str(),
        		oTrainInfo.m_lsPath[0],
        		oTrainInfo.m_lsPath[1],
        		oTrainInfo.m_lsPath[2],
        		oTrainInfo.m_lsPath[3]);

		//! Install train only if next four sections are not occupied
		if( enSpeed == SPEED_MAX )
		{

      		m_lsTrains[ oTrainInfo.szName.c_str() ] = new Train(oTrainInfo,this);
		    enInsStatus = TR_INS_SUCCESS;
		}
		else
		{
			cout<<"\n Train Installation failed because a train is either running in next four sections or one of the junction is locked";
			enInsStatus = TR_INS_TRACK_BUSY;
		}
	}
	else
	{
		enInsStatus = TR_INS_ALREADY_INSTALLED;
	}

	if( enInsStatus == TR_INS_SUCCESS )
	{
		cout<<"\n "<<oTrainInfo.szName.c_str()<<" Train can be added to the network ";
		m_gTrains.add_thread(new boost::thread(boost::bind(&Train::Run,m_lsTrains[ oTrainInfo.szName ])));

//    vSimulateLocationSubsystem(oTrainInfo);
	}
	else
	{
		cout<<"\n "<<oTrainInfo.szName.c_str()<<" Train cannot be added to the network ";
       //! Train Installation failed.
	}

	sleep(5);
	return enInsStatus;
}


enTrainSpeed ControlStation::enGetClearance(  TrainInfo & oTrainInfo ){

	int NextFourSection[4] = { -1 };
	enTrainSpeed enSpeed = SPEED_SLOW;


	vector<int>::const_iterator it =  std::find( oTrainInfo.m_lsPath.begin(), oTrainInfo.m_lsPath.end(),oTrainInfo.m_u32CurrentLocation);

	for( int i = 0; i<4 && it != oTrainInfo.m_lsPath.end();it++,i++ )
	{
         NextFourSection[i] = *it;
         //std::cout<<"\n Next Section "<<*it;
	}

    if ( NextFourSection[0] != -1 && m_lsTracks[NextFourSection[0]].bAmIHoldingTheSection( &oTrainInfo ) == false  )
    {
    	return enSpeed;
    }
    else
    {
    	enSpeed = SPEED_APPLY_BRAKES;
    }

    if( NextFourSection[1] != -1 && m_lsTracks[NextFourSection[1]].bAmIHoldingTheSection( &oTrainInfo ) ==false )
    {
         return enSpeed;
    }
    else
    {
    	if( m_lsTracks[NextFourSection[1]].bGetIsJunction() )
    	{
    		if ( !(m_lsTracks[NextFourSection[1]].bTryLock( oTrainInfo )))
    		{
                vUpDateUIWithUnavailability( NextFourSection[1], oTrainInfo );
    		}
    	}

    	enSpeed = SPEED_SLOW;
    }

    if( NextFourSection[2] != -1 && m_lsTracks[NextFourSection[2]].bAmIHoldingTheSection( &oTrainInfo ) ==false )
    {
    	if( m_lsTracks[NextFourSection[2]].bGetIsJunction() )
    	{
    		if ( ! m_lsTracks[NextFourSection[2]].bTryLock(oTrainInfo ) )
    		{
                vUpDateUIWithUnavailability( NextFourSection[2], oTrainInfo );
    		}
    	}

    	return enSpeed;
    }
    else
    {

        enSpeed = SPEED_MEDIUM;
    }

    if( NextFourSection[3] != -1 && m_lsTracks[NextFourSection[3]].bAmIHoldingTheSection( &oTrainInfo ) == false )
    {
         return enSpeed;
    }
    else
    {
    	if( m_lsTracks[NextFourSection[3]].bGetIsJunction() )
    	{
    		if ( ! m_lsTracks[NextFourSection[3]].bTryLock( oTrainInfo ) )
    		{
                vUpDateUIWithUnavailability( NextFourSection[3], oTrainInfo );
    		}
    	}

    	enSpeed = SPEED_MAX;
    }

    return enSpeed;
}


void ControlStation::vReleaseClearance( unsigned int u32Track ){
	m_lsTracks[u32Track].vUnLock();
}


void ControlStation::vStopAllTrains()
{

	//m_gTrains.interrupt_all();
	m_gTrains.join_all();
	std::cout<<"\n Stopped All trains\n";
}


void ControlStation::vUpDateUIWithUnavailability( unsigned int Section, TrainInfo & oTrainInfo)
{
}


void ControlStation::vSimulateLocationSubsystem( TrainInfo &refTrainInfo)
{
	Train *poTrain = m_lsTrains[ refTrainInfo.szName.c_str() ];


	for( vector<int>::const_iterator it = refTrainInfo.m_lsPath.begin(); it != refTrainInfo.m_lsPath.end();it++ )
	{
		Message oMsg;
		oMsg.enType = MSG_LOCATION;
		LocationUpdate *poLocationUpdate = new LocationUpdate;
		poLocationUpdate->m_u32CurrentSection = *it;
		syslog (LOG_INFO," Simulate Location %d ",  *it);
		//std::cout<<"\n Simulate Location "<<poLocationUpdate->m_u32CurrentSection;
		oMsg.m_pomsg = poLocationUpdate;
		poTrain->vPutMessageToQueue(oMsg);
	}
}

void ControlStation::vOnTimeOut()
{
	boost::lock_guard<boost::mutex> lock(*m_lsClearanceLock);

	std::map<std::string,Train *>::const_iterator it = m_lsTrains.begin();

	for( ; it != m_lsTrains.end();it++ )
	{
       unsigned int u32CurrentLocation = it->second->u32GetLocation();
//       std::cout<<"\n Location Subsys Simulation Update Current Location of "<<it->first<<" : "<<u32CurrentLocation;
       std::vector<int>::iterator itLocation =  std::find( it->second->m_oTrainInfo.m_lsPath.begin(),it->second->m_oTrainInfo.m_lsPath.end(),u32CurrentLocation);

       if( itLocation == it->second->m_oTrainInfo.m_lsPath.end()  )
    	   continue;

       itLocation++;

       if( itLocation != it->second->m_oTrainInfo.m_lsPath.end() )
       {
    		Message oMsg;
    		oMsg.enType = MSG_LOCATION;
    		LocationUpdate *poLocationUpdate = new LocationUpdate;
    		poLocationUpdate->m_u32CurrentSection = *itLocation;
   		    //std::cout<<"\n Simulate Location "<<poLocationUpdate->m_u32CurrentSection;
    		oMsg.m_pomsg = poLocationUpdate;
    		it->second->vPutMessageToQueue(oMsg);
       }

	}
}
