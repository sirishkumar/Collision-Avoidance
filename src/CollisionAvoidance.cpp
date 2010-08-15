//============================================================================
// Name        : CollisionAvoidance.cpp
// Author      : SIrishKumar
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Common.h"

#include <iostream>
#include <string.h>

#include "ControlStation.h"

using namespace std;



int main() {
	openlog ("CollAvo: ", LOG_PID,LOG_USER);
	setlogmask (LOG_UPTO(LOG_INFO));
	syslog (LOG_INFO,"Starting Collision Detection System Control Station");

	int listOfTracks[] = {1,2,3,4,5};
	std::vector<int> lsTracks(listOfTracks,listOfTracks+sizeof(listOfTracks));

	ControlStation oStation("Hyderabad",lsTracks);

	TrainInfo oTrainInfo;
	oTrainInfo.id = 10;
	oTrainInfo.szName = "Hyd Exp";
	oTrainInfo.m_u32Length = 1;

	oStation.enInstallTrain(oTrainInfo);

	//sleep(1000);

	oStation.vStopAllTrains();


	return 0;
}



