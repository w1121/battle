#include "../ZSocket/ZSSocket.h"
#include "../ZSocket/ZCSocket.h"

#include "ZSWrap.h"
#include <list>

void PushHeader(char ID, int LEN);
void FlushIntBuffer(void);
void FlushShipBuffer(void);

//===============================================================================
//=Ship depended part============================================================
typedef struct tagShip
{
    u_long fixPart[12];
	std::list<char> varPart;
}StructShip;

std::list<StructShip> shipBuffer;
StructShip *curShip;
//=End of Ship depended part=====================================================
//===============================================================================

//===============================================================================
//=Global Variables==============================================================
enum
{
	NonConnected,
	Authorizing,
	Idle,
	PushingParams,
	PushingFixedShipPart,
	PushingVariableShipPart,
	PushingShots
} State = NonConnected;

ZSSocket gSSock;
ZCSocket gCSock;
std::list<char> charBuffer;
std::list<u_long> intBuffer;
int g_NumElements;
int g_ByteSize;
//=End of Global Variables=======================================================
//===============================================================================

//===============================================================================
//=Interface Part================================================================
//===============================================================================

//===============================================================================
//=InitConnection================================================================
int InitConnection()
{
	if (State==NonConnected)
	{
		char buffer[6];
		memset(buffer,0,6);
		sockaddr_in ClAddr;

		bool Authorized=false;
		gSSock.BindToPort(GUIPort);
//		while (!Authorized)	{
		gSSock.WaitForCall((sockaddr*)&ClAddr);
		gSSock.PopUntil(buffer,5);
		if (!strcmp(buffer,"Hello"))
		{
			gSSock.SetClient(&gCSock);
			Authorized=true;
			PushHeader(10,0);
		} else
		{
			return EC_FAULT;
		}
//		}

		State = Idle;
	} else
	{
		return EC_FAULT; // Already connected
	}
	return EC_OK;
}
//===============================================================================
//===============================================================================

//===============================================================================
//=PushParam=====================================================================
void PushParam(int pushParam)
{
	if (State==Idle)
	{
		State=PushingParams;
		charBuffer.clear();
		intBuffer.clear();
		g_NumElements=0;
	}
	if (State==PushingParams)
	{
		intBuffer.push_back((u_long)pushParam);
		g_NumElements++;
	}
}
//=End of PushParam==============================================================
//===============================================================================

//===============================================================================
//=void PushMap(void)============================================================
extern "C" char* getPath(void);

void PushMap(void)
{
	char *MapPath = getPath();
	if (State == Idle)
	{
		union
		{
			char chBuf[4];
			u_long ulValue;
		} MaxX,MaxY;

		FILE *MapFile;
		char Buffer[gc_BufferSize];
		size_t tmpReaded = 0;
	
		MapFile = fopen(MapPath,"r");
		if (!MapFile)
		{
			fprintf(stderr,"Couldn't open file %s.",MapPath);
			return;
		}

		if (!feof(MapFile) && !ferror(MapFile))
		{
			tmpReaded = fread(MaxX.chBuf,sizeof(char),4,MapFile);
		}
		if (!feof(MapFile) && !ferror(MapFile))
		{
			tmpReaded = fread(MaxY.chBuf,sizeof(char),4,MapFile);
		}
		//Start pushing
		PushHeader(1,(2*4+MaxX.ulValue*MaxY.ulValue));
		gCSock.Push(&MaxX.ulValue,1);
		gCSock.Push(&MaxY.ulValue,1);
		while (!feof(MapFile))
		{
			tmpReaded = fread(Buffer,sizeof(char),100,MapFile);
			if (ferror(MapFile))
			{
				break;
			}

			gCSock.Push(Buffer,(int)tmpReaded);
		}
		fclose(MapFile);
	}
}
//=End of PushMap================================================================
//===============================================================================

//===============================================================================
//=PushFixedShipPart=============================================================
void PushFixedShipPart(
			  int ShipID,
			  int StrategyID,
			  int TypeShip,
			  int Speed,
			  int Heading,
			  int Resource,
			  int CoordinatesX,
			  int CoordinatesY,
			  int MissleAmount,
			  int TorpedoAmount,
			  int ShellAmount,
			  int RepairAmount
			  )
{
	if (State == Idle)
	{
		State = PushingFixedShipPart;
		charBuffer.clear();
		intBuffer.clear();

		shipBuffer.clear();
		g_NumElements=0;
		g_ByteSize=0;
	}
	if (State == PushingVariableShipPart)
	{
		State = PushingFixedShipPart;
	}
	if (State == PushingFixedShipPart)
	{
		g_NumElements++;
		g_ByteSize+=12*4;

		StructShip ss;	
		ss.fixPart[0]=(u_long)ShipID;
		ss.fixPart[1]=(u_long)StrategyID;
		ss.fixPart[2]=(u_long)TypeShip;
		ss.fixPart[3]=(u_long)Speed;
		ss.fixPart[4]=(u_long)Heading;
		ss.fixPart[5]=(u_long)Resource;
		ss.fixPart[6]=(u_long)CoordinatesX;
		ss.fixPart[7]=(u_long)CoordinatesY;
		ss.fixPart[8]=(u_long)MissleAmount;
		ss.fixPart[9]=(u_long)TorpedoAmount;
		ss.fixPart[10]=(u_long)ShellAmount;
		ss.fixPart[11]=(u_long)RepairAmount;		
		ss.varPart.clear();
		shipBuffer.push_back(ss);

		curShip=&shipBuffer.back();
	}
}
//=End of PushFixedShipPart======================================================
//===============================================================================

//===============================================================================
//=PushVariableShipPart==========================================================
void PushVariableShipPart(int Visible)
{
	if (State == PushingFixedShipPart)
	{
		State = PushingVariableShipPart;
	}
	if (State == PushingVariableShipPart)
	{
		(*curShip).varPart.push_back( char(Visible) );
		g_ByteSize++;
	}
}
//=End of PushVariableShipPart===================================================
//===============================================================================

//===============================================================================
//=PushShipsDummy================================================================
void PushShipsDummy(void)
{
	if (State == Idle)
	{
		PushHeader(2,4);
		u_long tmpULong = 0;
		gCSock.Push((u_long*)&tmpULong,1);
	}
}
//=End of PushShipsDummy=========================================================
//===============================================================================

//===============================================================================
//=PushShot======================================================================
void PushShot(
			  int TypeWeaponry,
			  int CurrentCoordinatesX,
			  int CurrentCoordinatesY,
			  int TargetCoordinatesX,
			  int TargetCoordinatesY
			  )
{
	if (State==Idle)
	{
		State = PushingShots;
		charBuffer.clear();
		intBuffer.clear();
		g_NumElements=0;
	}
	if (State == PushingShots)
	{
		intBuffer.push_back((u_long)TypeWeaponry);
		intBuffer.push_back((u_long)CurrentCoordinatesX);
		intBuffer.push_back((u_long)CurrentCoordinatesY);
		intBuffer.push_back((u_long)TargetCoordinatesX);
		intBuffer.push_back((u_long)TargetCoordinatesY);
		g_NumElements++;
	}
}
//=End of PushShot===============================================================
//===============================================================================

//===============================================================================
//=PushShotsDummy================================================================
void PushShotsDummy(void)
{
	if (State == Idle)
	{
		PushHeader(3,4);
		u_long tmpULong = 0;
		gCSock.Push((u_long*)&tmpULong,1);
	}
}
//=End of PushShotsDummy=========================================================
//===============================================================================

//===============================================================================
//=SendData======================================================================
int SendData(void)
{
	if (State == PushingParams)
	{
		PushHeader(0,(int)(4*intBuffer.size()));
		FlushIntBuffer();
		State=Idle;
		return EC_OK;
	}
	if (State == PushingShots)
	{
		PushHeader(3,(int)(4 * (intBuffer.size()+1)));
		gCSock.Push((u_long*)&g_NumElements,1);
		FlushIntBuffer();
		State=Idle;
		return EC_OK;
	}
	if (State == PushingFixedShipPart || State == PushingVariableShipPart)
	{
		PushHeader(2,(int)(4+g_ByteSize));
		gCSock.Push((u_long*)&g_NumElements,1);
		FlushShipBuffer();
		State=Idle;
		return EC_OK;
	}

	if (State != Idle)
	{
		return EC_FAULT;
	}
	return EC_OK;
}
//=End of SendData===============================================================
//===============================================================================

//===============================================================================
//=End of Interface Part=========================================================
//===============================================================================

//===============================================================================
//=Routine Part==================================================================
//===============================================================================

//===============================================================================
//=PushHeader====================================================================
void PushHeader(char ID, int LEN)
{
	gCSock.Push(&ID,1);
	gCSock.Push((u_long*)&LEN,1);
}
//=End of PushHeader=============================================================
//===============================================================================

//===============================================================================
//=FlushIntBuffer================================================================
void FlushIntBuffer(void)
{
	std::list<u_long>::const_iterator runIBuf;
	runIBuf=intBuffer.begin();
	u_long tmpUL;
	while (runIBuf!=intBuffer.end())
	{
		tmpUL=*runIBuf;
		gCSock.Push(&tmpUL,1);
		runIBuf++;
	}
	intBuffer.clear();
}
//=End of FlushIntBuffer=========================================================
//===============================================================================

//===============================================================================
//=FlushShipBuffer===============================================================
void FlushShipBuffer(void)
{
	std::list<StructShip>::const_iterator runShBuf;
	std::list<char>::const_iterator runChBuf;;

	runShBuf=shipBuffer.begin();

	u_long tmpUL;
	while (runShBuf!=shipBuffer.end())
	{
		int i;
		for (i=0; i<12; i++)
		{
			tmpUL=(*runShBuf).fixPart[i];
			gCSock.Push(&tmpUL,1);
		}

		runChBuf=(*runShBuf).varPart.begin();
		while (runChBuf!=(*runShBuf).varPart.end())
		{
			gCSock.Push((const char*)&(*runChBuf),1);
			runChBuf++;
		}
		runShBuf++;
	}	
	shipBuffer.clear();
}
//=End of FlushShipBuffer========================================================
//===============================================================================

//===============================================================================
//=End of Routine Part===========================================================
//===============================================================================
