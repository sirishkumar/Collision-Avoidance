
#ifndef _CONTROLSTATION_H_
#define _CONTROLSTATION_H_

#include "Common.h"
#include "Train.h"
#include "Section.h"
#include "Timer.h"

#include <map>

#include <string>
#include <boost/thread/mutex.hpp>


//! Thread Handling
#include <boost/thread/thread.hpp>

/*!
 * @brief Control Station is repponsible for launching and controlling trains
 *
 * 1. Installs a train
 * 2. Gives information about all trains running along with their location
 * 3.
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

 	boost::mutex  *m_lsClearanceLock;

 	//! For Location simulation
 	event_timer<ControlStation> * m_et;
    asio::io_service io_service;
    boost::thread *m_posimASIOThread;


 	void vUpDateUIWithUnavailability( unsigned int Section, TrainInfo & oTrainInfo);
    void vSendMessageToTrain( std::string &pszTrainName, void*msg, enMessageType enType);

public:
 	//! Control station constructur
	ControlStation( std::string szControlStationName, std::vector<Section> &refolsSections );

	~ControlStation();
    /*!
     *  @brief Install new with TrainInfo oTrainInfo
     */
	enTrainInstallStatus enInstallTrain(  TrainInfo & oTrainInfo );

	//! Remove an exisitng train
	bool bRemoveTrain();

    //! Initalize function for control station
	bool bInitControlStation();

	//! Checks whether a train can be installed as per the requirement
	enTrainSpeed enGetClearance( TrainInfo & oTrainInfo );
	//! not used
    void vReleaseClearance( unsigned int u32Track );

    void vSimulateLocationSubsystem( TrainInfo &refTrainInfo);

    //! Stop all trains
    void vStopAllTrains();

    //! Get function for list of sections
    std::vector<Section>& lsGetSections()
    {
    	return m_lsTracks;
    }

    void vOnTimeOut();

};


#endif
