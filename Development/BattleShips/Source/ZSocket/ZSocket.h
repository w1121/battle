#ifndef _ZSOCKET_H_
#define _ZSOCKET_H_

#include "ZSocketData.h"

#include <string>

using namespace std;

class ZSocket : public ZSocketData
{
//Exceptions
private:
	class WSAFailed {};
	class CreateFailed {};	
	class ConnectionFailed {};
	class ReceiveFailed {};
	class SendFailed {};
	class WrongIPString {};
	class WaitingFailed {};

//Interface
public:
	ZSocket();
	virtual ~ZSocket();

	void ConnectToDomain(const std::string strInetAddr,unsigned short int Port);
	void ConnectToIP(const std::string strIPAddr,unsigned short int Port);

	int Pop(char *pBuffer, unsigned int MaxSize);
	void Push(const char *pCHBuffer, int PushSize);
	void Push(u_short *pSIBuffer, int PushSize);
	void Push(u_long *pIBuffer, int PushSize);
	
//Additional functions
protected:
	sockaddr_in GetSockAddr(const std::string strIPAddr,unsigned short int Port);
	sockaddr_in GetSockAddr(unsigned short int Port);

//Inner implementation
private:
	void Init();
	void Destroy();
	void Create();
};

#endif  
