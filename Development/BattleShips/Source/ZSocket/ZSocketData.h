#ifndef _ZSOCKETDATA_H_
#define _ZSOCKETDATA_H_

//Windows implementation
#include <winsock2.h>

typedef SOCKET tSockID;

class ZSocketData  
{
public:
	ZSocketData();
	virtual ~ZSocketData();

//Platform depended static data
public:
	static bool EnvReady;
	static int RefCounter;

//Platform depended data

	SOCKET GetSockID();
	void SetSockID(tSockID sSockID);

protected:
	tSockID SockID;

};

#endif
