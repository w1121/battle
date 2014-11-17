#include "ZSSocket.h"

//=============================================================================
//=BindToIP====================================================================
void ZSSocket::BindToPort(unsigned short int Port)
{
	sockaddr_in cs_addr = GetSockAddr(Port);

	if (SOCKET_ERROR==bind(SockID,(sockaddr*)&cs_addr,sizeof(cs_addr)))
	{
		//throw ConnectionFailed();
		fprintf(stderr,"Socket's binding failed.\n");
	}
	if (SOCKET_ERROR==listen(SockID,5))
	{
		//throw ListenFailed();
		fprintf(stderr,"(%i) Socket listen failed.\n",::WSAGetLastError());
	}
}
//=End of BindToIP=============================================================
//=============================================================================

//=============================================================================
//==WaitForCall================================================================
void ZSSocket::WaitForCall(struct sockaddr *CallerAddress)
{
	int *CASize = 0;

	SOCKET res_acc = accept(SockID,CallerAddress,CASize);
	if ( res_acc == INVALID_SOCKET)
	{
		//throw WaitingFailed;
		fprintf(stderr,"Socket accepting failed\n");
	} else 
	{
		toClientSocket.SetSockID(res_acc);
	}
}
//==End of WaitForCall=========================================================
//=============================================================================

//=============================================================================
//==PopUntil===================================================================
int ZSSocket::PopUntil(char* pBuffer, int BufSize)
{
	int res,tmpSize;
	tmpSize=0;
	while (tmpSize<BufSize)
	{
		res = recv(toClientSocket.GetSockID(),pBuffer,BufSize,0);
		if (res == 0)
		{
			//Connection closed
			return 0;
		}
		if (res == SOCKET_ERROR)
		{
			//throw ReceiveFailed();
			fprintf(stderr,"(ERR:%i) Receiving data error.\n",WSAGetLastError());
			return -1;
		}
		tmpSize += res;
		pBuffer += res;
	}
	return tmpSize;
}
//==End of PopUntil============================================================
//=============================================================================

//=============================================================================
//=============================================================================
void ZSSocket::SetClient(ZCSocket *pCSocket)
{
	pCSocket->SetSockID(toClientSocket.GetSockID());
}
//=============================================================================
//=============================================================================

