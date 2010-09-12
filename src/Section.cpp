#include <syslog.h>

#include "Common.h"
#include "Section.h"
#include "iostream"

Section::Section( unsigned int u32Id,unsigned int u32Lenth,bool bIsJunction)
:m_u32Id(u32Id),
 m_u32LenthOfTheSection(u32Lenth),
m_bIsThisSectionJunction(bIsJunction),
m_poTrainWhichHoldsSection(0)
{
	syslog( LOG_INFO, "Initialising  Section %d", m_u32Id);

 	m_lsTrackLock = new boost::recursive_mutex();

}

Section::Section()
:m_u32Id(0),
 m_u32LenthOfTheSection(0),
 m_bIsThisSectionJunction(false),
 m_poTrainWhichHoldsSection(0)
{
	m_lsTrackLock = new boost::recursive_mutex();
}

bool Section::bTryLock( TrainInfo & oTrainInfo )
{
	bool bIsJunctionUnLocked = false;

	syslog (LOG_INFO,"\n Section %d",m_u32Id);

	if( ( bIsJunctionUnLocked = m_lsTrackLock->try_lock() ) )
	{
		 m_lsTrackLock->unlock();
	}

   return bIsJunctionUnLocked;
}

bool Section::bLock( TrainInfo & oTrainInfo )
{
	bool bIsJunctionLocked = true;

	m_lsTrackLock->lock();

   return bIsJunctionLocked;
}

void Section::vUnLock()
{
	m_lsTrackLock->unlock();
	m_poTrainWhichHoldsSection = 0;
}


TrainInfo* Section::poGetTrainInfoHoldingThisSection() const{
	return m_poTrainWhichHoldsSection;
}


