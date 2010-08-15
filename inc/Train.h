
#ifndef _TRAIN_H_
#define _TRAIN_H_


#include <vector>
#include <Section.h>

#include "Common.h"

class Train{

public:

    Train( TrainInfo &oTrainInfo  );

    unsigned int u32GetSpeed() const;
    void vSetSpeed(unsigned int u32Speed);

    void Run();

private:

    void vUpdateLocation( unsigned int u32Location );

	unsigned int m_u32Speed;
	unsigned int m_u32Location;
	TrainInfo m_oTrainInfo;
};


#endif
