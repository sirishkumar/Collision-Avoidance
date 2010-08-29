
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

bool ControlStation::bInitControlStation(){
}

enTrainInstallStatus ControlStation::enInstallTrain( TrainInfo & oTrainInfo ){

	enTrainInstallStatus enInsStatus;
	enTrainSpeed enSpeed;

	syslog (LOG_INFO,"ControlStation::enInstallTrain:%s ", oTrainInfo.szName.c_str());

	if( m_lsTrains.find( oTrainInfo.szName.c_str() ) == m_lsTrains.end() ){

		enSpeed = enGetClearance(oTrainInfo);

		std::cout<< "\n Clearance for Train "<<enSpeed;

		//! Install train only if next four sections are not occupied
		if( enSpeed == SPEED_MAX )
		{

      		m_lsTrains[ oTrainInfo.szName.c_str() ] = new Train(oTrainInfo,this);
		    enInsStatus = TR_INS_SUCCESS;
		}
	}
	else
	{
		enInsStatus = TR_INS_ALREADY_INSTALLED;
	}

	if( enInsStatus == TR_INS_SUCCESS ){
		syslog (LOG_INFO," TR_INS_SUCCESS -> %s", oTrainInfo.szName.c_str());
		m_gTrains.add_thread(new boost::thread(boost::bind(&Train::Run,m_lsTrains[ oTrainInfo.szName ])));

       vSimulateLocationSubsystem(oTrainInfo);
	}
	else
	{
       //! Train Installation failed.
	}

	return enInsStatus;
}


enTrainSpeed ControlStation::enGetClearance(  TrainInfo & oTrainInfo ){

	int NextFourSection[4] = { -1 };
	enTrainSpeed enSpeed = SPEED_SLOW;

	boost::lock_guard<boost::mutex> lock(*m_lsClearanceLock);

	vector<int>::const_iterator it =  std::find( oTrainInfo.m_lsPath.begin(), oTrainInfo.m_lsPath.end(),oTrainInfo.m_u32CurrentLocation);

	for( int i = 0; i<4 && it != oTrainInfo.m_lsPath.end();it++,i++ )
	{
         NextFourSection[i] = *it;
         std::cout<<"\n Next Section "<<*it;
	}

    if ( NextFourSection[0] != -1 && m_lsTracks[NextFourSection[0]].poGetTrainInfoHoldingThisSection() != 0 )
    {
    	return enSpeed;
    }
    else
    {
    	enSpeed = SPEED_APPLY_BRAKES;
    }

    if( NextFourSection[1] != -1 && m_lsTracks[NextFourSection[1]].poGetTrainInfoHoldingThisSection() != 0 )
    {
         return enSpeed;
    }
    else
    {
    	if( m_lsTracks[NextFourSection[1]].bGetIsJunction() )
    	{
    		if ( !(m_lsTracks[NextFourSection[1]].bLock( oTrainInfo )))
    		{
                vUpDateUIWithUnavailability( NextFourSection[1], oTrainInfo );
    		}
    	}

    	enSpeed = SPEED_SLOW;
    }

    if( NextFourSection[2] != -1 && m_lsTracks[NextFourSection[2]].poGetTrainInfoHoldingThisSection() != 0 )
    {
    	if( m_lsTracks[NextFourSection[2]].bGetIsJunction() )
    	{
    		if ( ! m_lsTracks[NextFourSection[2]].bLock(oTrainInfo ) )
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

    if( NextFourSection[3] != -1 && m_lsTracks[NextFourSection[3]].poGetTrainInfoHoldingThisSection() != 0 )
    {
         return enSpeed;
    }
    else
    {
    	if( m_lsTracks[NextFourSection[3]].bGetIsJunction() )
    	{
    		if ( ! m_lsTracks[NextFourSection[3]].bLock( oTrainInfo ) )
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
	std::cout<<"\n Stopped All trains";
}


void ControlStation::vUpDateUIWithUnavailability( unsigned int Section, TrainInfo & oTrainInfo)
{
}


void ControlStation::vSimulateLocationSubsystem( TrainInfo &refTrainInfo)
{
	Train *poTrain = m_lsTrains[ refTrainInfo.szName.c_str() ];

	LocationUpdate oLocationUpdate;

	Message oMsg;
	oMsg.enType = MSG_LOCATION;

	for( vector<int>::const_iterator it = refTrainInfo.m_lsPath.begin(); it != refTrainInfo.m_lsPath.end();it++ )
	{
		oLocationUpdate.m_u32CurrentSection = *it;
		syslog (LOG_INFO," Simulate Location %d ",  *it);
		oMsg.m_pomsg = &oLocationUpdate;
		poTrain->vPutMessageToQueue(oMsg);
	}
}

