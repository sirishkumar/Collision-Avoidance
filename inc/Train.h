
#ifndef _TRAIN_H_
#define _TRAIN_H_


#include <vector>
#include <Section.h>

#include "Common.h"

class ControlStation;

class Train{

public:


	//! Train constructor
	Train( TrainInfo &oTrainInfo ,ControlStation *poControlStation );


	unsigned int u32GetSpeed() const;
    void vSetSpeed(unsigned int u32Speed);

    //! Thread call back function. Train runs in this loop
    void Run();

    bool bCheckWhetherTrainCanBeInstalled( TrainInfo & refoTrain );

private:

    void vUpdateLocation( unsigned int u32Location );

	unsigned int m_u32Speed;
	unsigned int m_u32Location;
	TrainInfo m_oTrainInfo;
	//! POinter to control station
	ControlStation * m_poControlStation;
};


#endif
