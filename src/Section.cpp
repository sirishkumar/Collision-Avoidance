#include <syslog.h>

#include "Common.h"
#include "Section.h"
#include "iostream"
#include "string.h"

Section::Section( unsigned int u32Id,unsigned int u32Lenth,bool bIsJunction)
:m_u32Id(u32Id),
 m_u32LenthOfTheSection(u32Lenth),
m_bIsThisSectionJunction(bIsJunction),
m_poTrainWhichHoldsSection(0),
m_poTrainWhichHoldsTheLock(0)
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
	bool bIsJunctionUnLocked = true;

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


	if( m_poTrainWhichHoldsTheLock && ( m_poTrainWhichHoldsTheLock->szName == oTrainInfo.szName ) )
	{
		return bIsJunctionLocked;
	}

	m_lsTrackLock->lock();
	std::cout<<"\n Section "<< m_u32Id << " Locked by "<<oTrainInfo.szName;
	m_poTrainWhichHoldsTheLock = &oTrainInfo;

   return bIsJunctionLocked;
}



void Section::vUnLock()
{
	std::cout<<"\n Section "<< m_u32Id << " UnLocked by "<<m_poTrainWhichHoldsTheLock->szName;
	m_lsTrackLock->unlock();
	m_poTrainWhichHoldsTheLock = 0;
}


TrainInfo* Section::poGetTrainInfoHoldingThisSection() const
{

	return m_poTrainWhichHoldsSection;
}

bool Section::bAmIHoldingTheSection( TrainInfo * poTrainInfo )
{
	bool bResult = false;

	if( m_poTrainWhichHoldsSection == 0 || ( m_poTrainWhichHoldsSection && m_poTrainWhichHoldsSection->id == poTrainInfo->id ) )
	{
		bResult = true;
	}

	return bResult;
}

bool Section::bAmIHoldingTheSectionLock( TrainInfo * poTrainInfo )
{
	bool bResult = false;

	if( m_poTrainWhichHoldsTheLock && (m_poTrainWhichHoldsTheLock->id == poTrainInfo->id ))
	{
		bResult = true;
	}

	return bResult;
}


bool Section::bSetTrain(TrainInfo * poTrainInfo )
{
	bool bResult = false;

	if( m_poTrainWhichHoldsSection )
	{
		if( !bAmIHoldingTheSection(poTrainInfo) )
		    std::cout<<"\n Section "<< m_u32Id << ": Train "<< m_poTrainWhichHoldsSection->szName <<" is Holding this Section ";
	}
	else
	{
       m_poTrainWhichHoldsSection = poTrainInfo ;
       bResult = true;
	}

	return bResult;
}

void Section::vReleaseSection( TrainInfo * poTrainInfo  )
{
    if( bAmIHoldingTheSection( poTrainInfo ) )
    {
    	m_poTrainWhichHoldsSection  = 0;
    }
}

