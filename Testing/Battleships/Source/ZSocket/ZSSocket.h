#ifndef _ZSSOCKET_H_
#define _ZSSOCKET_H_

#include "ZSocket.h"
#include "ZCSocket.h"

class ZSSocket: public ZSocket
{
public:
	ZSSocket() {};
	~ZSSocket() {};

	void BindToPort(unsigned short int Port);
	void WaitForCall(struct sockaddr *CallerAddress);
	void SetClient(ZCSocket *pCSocket);

	int PopUntil(char* pBuffer, int BufSize);

protected:
	ZSocket toClientSocket;
};

#endif
