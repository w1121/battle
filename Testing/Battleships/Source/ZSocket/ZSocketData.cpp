#include "ZSocketData.h"

//=====================================================================
//Static members definitions ==========================================

bool ZSocketData::EnvReady=false;
int ZSocketData::RefCounter=0;

//End of Static members definitions
//=====================================================================

ZSocketData::ZSocketData()
{
}

ZSocketData::~ZSocketData()
{
}

SOCKET ZSocketData::GetSockID()
{
	return SockID;
}
void ZSocketData::SetSockID(SOCKET sSockID)
{
	SockID=sSockID;
}




