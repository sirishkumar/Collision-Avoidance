
#ifndef _TRAIN_H_
#define _TRAIN_H_


#include <vector>
#include <Section.h>

#include "Common.h"
#include <boost/interprocess/ipc/message_queue.hpp>

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

private:

    void vUpdateLocation( unsigned int u32Location );

	unsigned int m_u32Speed;
	unsigned int m_u32CurrentLocation;
	TrainInfo m_oTrainInfo;
	//! POinter to control station
	ControlStation * m_poControlStation;
	boost::interprocess::message_queue *m_poMQ;
};


#endif
