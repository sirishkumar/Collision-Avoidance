/*
 * LocationSubSysSim.cpp
 *
 *  Created on: 12-Sep-2010
 *      Author: sirishkumar
 */

#include "LocationSubSysSim.h"

LocationSubSysSim  LocationSubSysSim::m_oLocSim;

LocationSubSysSim::LocationSubSysSim()
{}

LocationSubSysSim * LocationSubSysSim::getLocationSubSysInstance()
{
   return &m_oLocSim;
}
