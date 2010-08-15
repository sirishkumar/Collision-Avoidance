
#ifndef _CONTROLSTATION_H_
#define _CONTROLSTATION_H_

#include "Common.h"
#include "Train.h"
#include <map>

#include <string>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

class ControlStation{

private:
    std::string m_szName;
 	std::map<std::string,Train *>  m_lsTrains;
 	std::map<unsigned int ,boost::interprocess::interprocess_mutex *>  m_lsTracksLocks;
 	 std::vector<int> m_lsTracks;

public:
	ControlStation( std::string szControlStationName, std::vector<int> &refolsTracks );

	enTrainInstallStatus enInstallTrain(  TrainInfo & oTrainInfo );
	bool bRemoveTrain();
	bool bInitializeTrackMutex( std::vector<int> &refolsTracks );
	bool bInitControlStation();

};


#endif
