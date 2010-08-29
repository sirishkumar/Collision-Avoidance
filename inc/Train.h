
#ifndef _TRAIN_H_
#define _TRAIN_H_


#include <vector>
#include <Section.h>
#include <queue>
#include <string>
#include <boost/thread/locks.hpp>
#include <boost/thread/thread.hpp>

#include "Common.h"
#include "Message.h"

class ControlStation;

class Train{

public:


	//! Train constructor
	Train( TrainInfo &oTrainInfo ,ControlStation *poControlStation );

	unsigned int u32GetSpeed() const;
    void vSetSpeed(unsigned int u32Speed);

    unsigned int u32GetLocation() const
    {
       return m_u32CurrentLocation;
    }

    //! Thread call back function. Train runs in this loop
    void Run();

    bool bCheckWhetherTrainCanBeInstalled( TrainInfo & refoTrain );

    //! Message are processed in this function( Location Subsystem etc..,)
    void vProcessMessage( Message &poMsg );

    //! Called when there is message for train. This queues message to m_lsMessageQueue
    void vPutMessageToQueue( Message &refMsg );

private:

    //! This function is called on update from location subsystem
    void vUpdateLocation( unsigned int u32Location );


	unsigned int m_u32Speed;
	unsigned int m_u32CurrentLocation;
	TrainInfo m_oTrainInfo;
	//! POinter to control station
	ControlStation * m_poControlStation;
	std::queue<Message>  m_lsMessageQueue;
	boost::mutex m_mutexQ;               // mutex protecting the queue
	boost::condition_variable m_condQ;
	//! If Train reaches destination set this
	bool m_bStopped;
};


#endif
