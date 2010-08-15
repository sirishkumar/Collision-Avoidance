
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

	bInitializeTrackMutex( m_lsTracks );
}

enTrainInstallStatus ControlStation::enInstallTrain( TrainInfo & oTrainInfo ){

	enTrainInstallStatus enInsStatus;

	if( m_lsTrains.find( oTrainInfo.szName.c_str() ) == m_lsTrains.end() ){

		m_lsTrains[ oTrainInfo.szName.c_str() ] = new Train(oTrainInfo);
		enInsStatus = TR_INS_SUCCESS;
	}
	else
	{
		enInsStatus = TR_INS_ALREADY_INSTALLED;
	}

	return enInsStatus;
}

bool ControlStation::bInitializeTrackMutex( std::vector<int> &refolsTracks ){

	for( vector<int>::const_iterator it = refolsTracks.begin(); it != refolsTracks.end(); it++ )
		m_lsTracksLocks[*it] = new interprocess_mutex;

	return true;
}
