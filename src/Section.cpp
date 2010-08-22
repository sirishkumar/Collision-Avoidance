
#include "Common.h"
#include "Section.h"


Section::Section( unsigned int u32Id,unsigned int u32Lenth,bool bIsJunction)
:m_u32Id(u32Id),
 m_u32LenthOfTheSection(u32Lenth),
m_bIsThisSectionJunction(bIsJunction)
{
	syslog( LOG_INFO, "Initialising  Section %d", m_u32Id);

 	m_lsTrackLock = new boost::mutex();

}


void Section::vLock()
{
	m_lsTrackLock->lock();
}


void Section::vUnLock()
{
	m_lsTrackLock->unlock();
}


