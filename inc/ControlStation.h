
#ifndef _CONTROLSTATION_H_
#define _CONTROLSTATION_H_

#include "Common.h"
#include "Train.h"
#include "Section.h"
#include <map>

#include <string>


//! Thread Handling
#include <boost/thread/thread.hpp>

/*!
 * @brief Control Station is repponsible for launching and controlling trains
 */
class ControlStation{

private:
	//! NAme of the controlling station
    std::string m_szName;
    //! List of trains started by this station
 	std::map<std::string,Train *>  m_lsTrains;

 	//! List of tracks present and their status
 	std::vector<Section> m_lsTracks;

 	//! Thread group which runs the threads in background
 	boost::thread_group m_gTrains;

public:
 	//! Control station constructur
	ControlStation( std::string szControlStationName, std::vector<Section> &refolsSections );

    /*!
     *  @brief Install new with TrainInfo oTrainInfo
     */
	enTrainInstallStatus enInstallTrain(  TrainInfo & oTrainInfo );

	//! Remove an exisitng train
	bool bRemoveTrain();

    //! Initalize function for control station
	bool bInitControlStation();

	//! Checks whether a train can be installed as per the requirement
	bool bGetClearance( unsigned int u32Track );
	//! not used
    void vReleaseClearance( unsigned int u32Track );

    //! Stop all trains
    void vStopAllTrains();
};


#endif
