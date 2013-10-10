#pragma once

#include "../winsock2_wrapper/iEventHandler.h"
#include <tuple>
#include "PatientMoniterReactor.h"


class DemuxTable
{
public:


private:
	std::tuple<iEventHandler, EVENT_TYPE> sets[FD_SETSIZE];

};

