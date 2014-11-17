#ifndef _ZCSOCKET_H_
#define _ZCSOCKET_H_

#include "ZSocket.h"

class ZCSocket : public ZSocket
{
public:
	ZCSocket(void);
	ZCSocket(tSockID sSockID);
	~ZCSocket(void);
};

#endif
