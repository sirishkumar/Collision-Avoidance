#ifndef _COMMON_H_
#define _COMMON_H_

#include <syslog.h>
#include <string>
#include <vector>

#include "Section.h"

enum enTrainInstallStatus{

	TR_INS_SUCCESS,
	TR_INS_TRACK_BUSY,
	TR_INS_WAIT,
	TR_INS_ALREADY_INSTALLED,
	TR_INS_FAILURE
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
  std::vector<Section> m_lsPath;
};

#endif
