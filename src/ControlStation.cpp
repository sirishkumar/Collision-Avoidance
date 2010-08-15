
#include <iostream>
#include <boost/thread.hpp>

#include "ControlStation.h"
#include "Common.h"
#include "Train.h"

using namespace std;

using namespace boost::interprocess;

ControlStation::ControlStation( std::string szName, std::vector<int> &refolsTracks ):
m_szName( szName ),
m_lsTracks( refolsTracks ){

}

bool ControlStation::bInitControlStation(){

	return bInitializeTrackMutex( m_lsTracks );
}

enTrainInstallStatus ControlStation::enInstallTrain( TrainInfo & oTrainInfo ){

	enTrainInstallStatus enInsStatus;

	syslog (LOG_INFO,"ControlStation::enInstallTrain:%s ", oTrainInfo.szName.c_str());

	if( m_lsTrains.find( oTrainInfo.szName.c_str() ) == m_lsTrains.end() ){

		m_lsTrains[ oTrainInfo.szName.c_str() ] = new Train(oTrainInfo);
		enInsStatus = TR_INS_SUCCESS;
	}
	else
	{
		enInsStatus = TR_INS_ALREADY_INSTALLED;
	}

	if( enInsStatus == TR_INS_SUCCESS ){
		std::cout<<"Running Train";
		syslog (LOG_INFO," TR_INS_SUCCESS ", oTrainInfo.szName.c_str());
		m_gTrains.add_thread(new boost::thread(boost::bind(&Train::Run,m_lsTrains[ oTrainInfo.szName ])));
	}

	return enInsStatus;
}

bool ControlStation::bInitializeTrackMutex( std::vector<int> &refolsTracks ){

	for( vector<int>::const_iterator it = refolsTracks.begin(); it != refolsTracks.end(); it++ )
		m_lsTracksLocks[*it] = new interprocess_mutex;

	return true;
}


bool ControlStation::bGetClearance( unsigned int u32Track ){
	m_lsTracksLocks[u32Track]->lock();
}

void ControlStation::vReleaseClearance( unsigned int u32Track ){

	m_lsTracksLocks[u32Track]->unlock();
}


void ControlStation::vStopAllTrains(){

	m_gTrains.interrupt_all();
	m_gTrains.join_all();
}
