#include <stdio.h>
#include <conio.h>

#include "../ZSocket/ZSocket.h"

int main(int argc,char **argv)
{
	char buffer[1001];
	memset(buffer,0,1001);

	ZSocket gSock;

	gSock.ConnectToIP("127.0.0.1",11111);
	printf("Connected to IP: 127.0.0.1:11111\n");

	gSock.Push("Hello",5);
	printf("Authorization \"Hello\" send.\n");

	char one;

	while(1)
	{

		//PopUntil
		int BufSize=1;
		char *pBuffer=&one;

		int res,tmpSize;
		tmpSize=0;
		while (tmpSize<1)
		{
			res = recv(gSock.GetSockID(),pBuffer,BufSize,0);
			if (res == 0)
			{
				//Connection closed
				tmpSize=0;
				break;
			}
			if (res == SOCKET_ERROR)
			{
				//throw ReceiveFailed();
				fprintf(stderr,"(ERR:%i) Receiving data error.\n",WSAGetLastError());
				tmpSize=-1;
				break;
			}
			tmpSize += res;
			pBuffer += res;
		}
		//return tmpSize;
		//End of PopUntil

		if (tmpSize==0)
		{
			break;
		}
		printf("Hex Char: >%X<\n",(unsigned char)one);
	}
	printf("Data received.\n");

	printf("Press any key to continue...\n");
	fflush(stdin);
	getch();
	return 0;
}
