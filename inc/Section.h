

#ifndef _SECTION_H_
#define _SECTION_H_


#include <boost/thread/mutex.hpp>


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

   bool vGetIsJunction() const
   {
	   return m_bIsThisSectionJunction;
   }

   void vLock();

   void vUnLock();

   Section(){}

private:

	//! Unique ID of the section/Track
	unsigned int m_u32Id;
    //! Length of the section/Track
	unsigned int m_u32LenthOfTheSection;
	//! Is this section a junction
    bool m_bIsThisSectionJunction;

    //! @todo Mutex are only needed for junctions
 	//! Locks for Junctions
 	boost::mutex  *m_lsTrackLock;

};


#endif
