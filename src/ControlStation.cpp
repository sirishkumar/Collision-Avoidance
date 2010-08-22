
#include <iostream>
#include <boost/thread.hpp>

#include "ControlStation.h"
#include "Common.h"
#include "Train.h"

using namespace std;


ControlStation::ControlStation( std::string szName, std::vector<Section> &refolsSections  ):
m_szName( szName ),
m_lsTracks( refolsSections ){

}

bool ControlStation::bInitControlStation(){
}

enTrainInstallStatus ControlStation::enInstallTrain( TrainInfo & oTrainInfo ){

	enTrainInstallStatus enInsStatus;

	syslog (LOG_INFO,"ControlStation::enInstallTrain:%s ", oTrainInfo.szName.c_str());

	if( m_lsTrains.find( oTrainInfo.szName.c_str() ) == m_lsTrains.end() ){

		if( bGetClearance( *(oTrainInfo.m_lsPath.begin()) ) )
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
		std::cout<<"Running Train";
		syslog (LOG_INFO," TR_INS_SUCCESS ", oTrainInfo.szName.c_str());
		m_gTrains.add_thread(new boost::thread(boost::bind(&Train::Run,m_lsTrains[ oTrainInfo.szName ])));
	}

	return enInsStatus;
}


bool ControlStation::bGetClearance( unsigned int u32Track ){
	m_lsTracks[u32Track].vLock();


}

void ControlStation::vReleaseClearance( unsigned int u32Track ){
	m_lsTracks[u32Track].vUnLock();
}


void ControlStation::vStopAllTrains(){

	m_gTrains.interrupt_all();
	m_gTrains.join_all();
}


