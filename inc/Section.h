

#ifndef _SECTION_H_
#define _SECTION_H_


#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include "Common.h"


/*!
 *  Section class and its attributes
 */
class Section
{
public:
   Section( unsigned int u32Id, unsigned int u32Lenth, bool bIsJunction);

   void vSetId( unsigned int u32Id ){
	   m_u32Id = u32Id;
   }

   void vSetLength( unsigned int u32Length )
   {
	   m_u32LenthOfTheSection = u32Length;
   }

   void vSetIsJunction( bool bIsJunction )
   {
	   m_bIsThisSectionJunction = bIsJunction;
   }

   unsigned int u32GetId() const {
	   return m_u32Id ;
   }

   unsigned int u32GetLength( unsigned int u32Length ) const
   {
	   return m_u32LenthOfTheSection;
   }

   bool bGetIsJunction() const
   {
	   return m_bIsThisSectionJunction;
   }

   bool bGetIsSectionBusy(){
	   return m_bIsSectionBusy;
   }

   void vSetSectionBusy(){
	   m_bIsSectionBusy = true;
   }

   bool bLock( TrainInfo & oTrainInfo );

   void vUnLock();

   TrainInfo* poGetTrainInfoHoldingThisSection() const;

   Section(){}

private:

	//! Unique ID of the section/Track
	unsigned int m_u32Id;
    //! Length of the section/Track
	unsigned int m_u32LenthOfTheSection;
	//! Is this section a junction
    bool m_bIsThisSectionJunction;
    //! Is Section Currently Busy( i.e., A train is running on thie section )
    bool m_bIsSectionBusy;

    //! @todo Mutex are only needed for junctions
 	//! Locks for Junctions
 	boost::recursive_mutex  *m_lsTrackLock;


 	//! Hold the information of the train which runs on this section
 	TrainInfo * m_poTrainWhichHoldsSection;
};


#endif
