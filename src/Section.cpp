
#include "Common.h"
#include "Section.h"


Section::Section( unsigned int u32Id,unsigned int u32Lenth,bool bIsJunction)
:m_u32Id(u32Id),
 m_u32LenthOfTheSection(u32Lenth),
m_bIsThisSectionJunction(bIsJunction),
m_poTrainWhichHoldsSection(0)
{
	syslog( LOG_INFO, "Initialising  Section %d", m_u32Id);

 	m_lsTrackLock = new boost::recursive_mutex();

}


bool Section::bLock( TrainInfo & oTrainInfo )
{
	bool bIsJunctionLocked = false;

	if( ( bIsJunctionLocked == m_lsTrackLock->try_lock() ) )
	{
	   m_poTrainWhichHoldsSection = &oTrainInfo;
	}

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


