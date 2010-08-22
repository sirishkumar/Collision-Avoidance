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
#include <vector>

#include "ControlStation.h"
#include "Section.h"

using namespace std;


void vGetTestSections( std::vector<Section> &lsSections  )
{
    Section oSection;

   for( int i = 0;i<10;i++)
   {
	   oSection.vSetId(i);
	   oSection.vSetIsJunction(false);
	   oSection.vSetLength(5);

	   lsSections.push_back(oSection);
   }


   lsSections[5].vSetIsJunction(true);
   lsSections[8].vSetIsJunction(true);
}

void vGetTestTrackForTrain( std::vector<int> &refTracks )
{
   refTracks.push_back(3);
   refTracks.push_back(4);
   refTracks.push_back(5);
   refTracks.push_back(6);
   refTracks.push_back(7);
   refTracks.push_back(8);
}

int main() {
	openlog ("CollAvo: ", LOG_PID,LOG_USER);
	setlogmask (LOG_UPTO(LOG_INFO));
	syslog (LOG_INFO,"Starting Collision Detection System Control Station");

	std::vector<Section> lsTracks;
	vGetTestSections(lsTracks);

	ControlStation oStation("Hyderabad",lsTracks);

	TrainInfo oTrainInfo;
	oTrainInfo.id = 10;
	oTrainInfo.szName = "Hyd Exp";
	oTrainInfo.m_u32Length = 1;

	std::vector<int> route;
	vGetTestTrackForTrain(route);
	oTrainInfo.m_lsPath = route;

	oStation.enInstallTrain(oTrainInfo);

	//sleep(1000);

	oStation.vStopAllTrains();


	return 0;
}



