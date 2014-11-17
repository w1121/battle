#include "ZSocket.h"

#include "winsock2.h"

//=====================================================================
//=Constructor=========================================================
ZSocket::ZSocket()
{
	Init();
	Create();
}
//=End of Constructor==================================================
//=====================================================================

//=====================================================================
//=Destructor==========================================================
ZSocket::~ZSocket()
{
	Destroy();
}
//=End of Destructor===================================================
//=====================================================================

//=============================================================================
//==Init=======================================================================
void ZSocket::Init()
{
	if (!EnvReady)
	{
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(1,1),&wsaData))
		{
			//throw WSAFailed();
			fprintf(stderr,"WinSock DLL initialization failed.");
		}
		EnvReady=true;
	}
	RefCounter++;
}
//==End of Init================================================================
//=============================================================================

//=============================================================================
//==Create=====================================================================
void ZSocket::Create()
{
	SockID=socket(AF_INET,SOCK_STREAM,0);
	if (SockID==INVALID_SOCKET)
	{
		//throw CreateFailed();
		fprintf(stderr,"Socket initialization failed.\n");
	}
}
//==End of Create==============================================================
//=============================================================================

//=====================================================================
//==Destroy============================================================
void ZSocket::Destroy()
{
	closesocket(SockID);

	RefCounter--;
	if (RefCounter==0)
	{
		WSACleanup();
	}
}
//=End of Destroy======================================================
//=====================================================================


//=============================================================================
//==ConnectToDomain============================================================
void ZSocket::ConnectToDomain(const std::string strInetAddr,unsigned short int Port)
{
	sockaddr_in cs_addr;
	cs_addr.sin_family=AF_INET;
	//cs_addr.sin_addr.S_un.S_addr=inet_addr(strIPAddr.c_str());
	cs_addr.sin_port=htons(Port);

	if (SOCKET_ERROR==connect(SockID,(sockaddr*)&cs_addr,sizeof(cs_addr)))
	{
		//throw ConnectionFailed();
		fprintf(stderr,"Socket's connecting failed.");
	}
}
//==End of ConnectToDomain=====================================================
//=============================================================================

//=============================================================================
//==ConnectToIP================================================================
void ZSocket::ConnectToIP(const string strIPAddr,unsigned short int Port)
{
	sockaddr_in cs_addr;
	cs_addr.sin_family=AF_INET;
	cs_addr.sin_addr.S_un.S_addr=inet_addr(strIPAddr.c_str());
	cs_addr.sin_port=htons(Port);

	if (SOCKET_ERROR==connect(SockID,(sockaddr*)&cs_addr,sizeof(cs_addr)))
	{
		//throw ConnectionFailed();
		fprintf(stderr,"Socket's connecting failed.\n");
	}
}
//==End of ConnectToIP=========================================================
//=============================================================================

//=============================================================================
//==GetSockAddr================================================================
sockaddr_in ZSocket::GetSockAddr(const std::string strIPAddr,unsigned short int Port)
{
	sockaddr_in res_addr;
	res_addr.sin_family=AF_INET;
	if (res_addr.sin_addr.S_un.S_addr=inet_addr(strIPAddr.c_str())==-1)
	{
		//throw WrongIPString();
		fprintf(stderr,"Wrong IP address string: %s\n",strIPAddr.c_str());
	}
	res_addr.sin_port=htons(Port);
	return res_addr;
}
//==End of GetSockAddr=========================================================
//=============================================================================

//=============================================================================
//==GetSockAddr================================================================
sockaddr_in ZSocket::GetSockAddr(unsigned short int Port)
{
	sockaddr_in res_addr;
	memset(&res_addr,0,sizeof(res_addr));
	res_addr.sin_family=AF_INET;
	res_addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	res_addr.sin_port=htons(Port);
	return res_addr;
}
//==End of GetSockAddr=========================================================
//=============================================================================

//=============================================================================
//==Pop========================================================================
int ZSocket::Pop(char* pBuffer, unsigned int MaxSize)
{
	int len;
	len=recv(SockID,pBuffer,MaxSize,0);
	if (len==SOCKET_ERROR)
	{
		//throw ReceiveFailed();
		fprintf(stderr,"Receiving data error.\n");
	}
	return len;
}
//==End of Pop=================================================================
//=============================================================================

//=============================================================================
//==Push=======================================================================
void ZSocket::Push(const char* pCHBuffer, int PushSize)
{
	int res;
	res=send(SockID,pCHBuffer,PushSize,0);
	if (res==SOCKET_ERROR)
	{
		//throw SendFailed();
		fprintf(stderr,"Sending data error.\n");
	}
}
//==End of Push================================================================
//=============================================================================

//=============================================================================
//==Push=======================================================================
void ZSocket::Push(u_long* pIBuffer, int PushSize)
{
	int i;
	union
	{
		u_long ulValue;
		char chBuf[4];
	} transULNS;

	for (i=0; i<PushSize; i++)
	{
		transULNS.ulValue=htonl(pIBuffer[i]);
		Push(transULNS.chBuf,4);	
	}
}
//==End of Push================================================================
//=============================================================================

//=============================================================================
//==Push=======================================================================
void ZSocket::Push(u_short *pSIBuffer, int PushSize)
{
	int i;
	union
	{
		u_short usValue;
		char chBuf[2];
	} transUSNS;

	for (i=0; i<PushSize; i++)
	{
		transUSNS.usValue=htons(pSIBuffer[i]);
		Push(transUSNS.chBuf,2);
	}
}
//==End of Push================================================================
//=============================================================================

