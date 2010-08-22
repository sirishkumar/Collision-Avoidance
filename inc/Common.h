#ifndef _COMMON_H_
#define _COMMON_H_

#include <syslog.h>
#include <string>
#include <vector>


#define MIN_LOCKS_REQUIRED_TO_INSTALL 4

enum enTrainInstallStatus{

	TR_INS_SUCCESS,
	TR_INS_TRACK_BUSY,
	TR_INS_WAIT,
	TR_INS_ALREADY_INSTALLED,
	TR_INS_FAILURE
};

enum enTrainSpeed
{
	SPEED_APPLY_EMERGENCY_BRAKES,
    SPEED_APPLY_BRAKES,
    SPEED_SLOW,
    SPEED_MEDIUM,
    SPEED_MAX
};

class TrainInfo
{
public:

  unsigned int id;
  std::string szName;
  std::string szStartStaionName;
  std::string szEndStationName;
  unsigned int m_u32Length;
  unsigned int m_u32CurrentLocation;
  //! Route of the train is stored here
  std::vector<int> m_lsPath;

  float m_fMass;
  short m_u16Priority;

  void vSetMass( float fMass ){
    m_fMass = fMass;
  }

  float fGetMass() const{

	  return m_fMass;
  }

  void setPriority( short u16Priority ){
	  m_u16Priority = u16Priority;
  }

  short u16GetPriority() const{

    return m_u16Priority;
  }




};

#endif
