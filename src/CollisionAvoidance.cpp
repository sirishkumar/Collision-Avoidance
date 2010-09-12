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

   for( int i = 0;i<40;i++)
   {
	   oSection.vSetId(i);
	   oSection.vSetIsJunction(false);
	   oSection.vSetLength(5);

	   lsSections.push_back(oSection);
   }


   lsSections[5].vSetIsJunction(true);
   lsSections[6].vSetIsJunction(true);

   lsSections[12].vSetIsJunction(true);
   lsSections[13].vSetIsJunction(true);

   lsSections[1].vSetIsJunction(true);
   lsSections[24].vSetIsJunction(true);

   lsSections[17].vSetIsJunction(true);
   lsSections[18].vSetIsJunction(true);

   lsSections[27].vSetIsJunction(true);
   lsSections[28].vSetIsJunction(true);
   lsSections[29].vSetIsJunction(true);
   lsSections[30].vSetIsJunction(true);


}

void vGetTestTrackForTrain( std::vector<int> &refTracks )
{
  refTracks.clear();

  for(int i = 2;i<16;i++)
	refTracks.push_back(i);
}

void vGetTestTrackForTrain2( std::vector<int> &refTracks )
{
  refTracks.clear();

  for(int i = 16;i>=2;i--)
	refTracks.push_back(i);
}

int main() {
	openlog ("CollAvo: ", LOG_PID,LOG_USER);
	setlogmask (LOG_UPTO(LOG_INFO));
	syslog (LOG_INFO,"Starting Collision Detection System Control Station");


	std::vector<Section> lsTracks;
	vGetTestSections(lsTracks);

	ControlStation oStation("Hyderabad",lsTracks);

	//! Train 1
	TrainInfo oTrainInfo;
	oTrainInfo.id = 10;
	oTrainInfo.szName = "Hyd Exp";
	oTrainInfo.m_u32Length = 1;

	std::vector<int> route;
	vGetTestTrackForTrain(route);
	oTrainInfo.m_u32CurrentLocation = 2;
	oTrainInfo.m_lsPath = route;

	oStation.enInstallTrain(oTrainInfo);

	//! Train 1
	TrainInfo oTrainInfo2;
	oTrainInfo2.id = 11;
	oTrainInfo2.szName = "Bglr Exp";
	oTrainInfo2.m_u32Length = 1;

	std::vector<int> route2;
	vGetTestTrackForTrain2(route2);
	oTrainInfo2.m_u32CurrentLocation = 2;
	oTrainInfo2.m_lsPath = route2;

	oStation.enInstallTrain(oTrainInfo2);


	oStation.vStopAllTrains();


	return 0;
}



