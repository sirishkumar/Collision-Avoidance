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

using namespace std;



int main() {
	openlog ("CollAvo: ", LOG_PID,LOG_USER);
	setlogmask (LOG_UPTO(LOG_INFO));
	syslog (LOG_INFO,"Starting Collision Detection System Control Station");

	return 0;
}



