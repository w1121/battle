#ifndef _ZSWRAP_H_
#define _ZSWRAP_H_

#include <stdio.h>

const size_t gc_BufferSize = 100;
const unsigned short int GUIPort = 22881;

const int EC_OK = 0;
const int EC_FAULT = -1;

extern "C" int InitConnection(void);
extern "C" void PushParam(int pushParam);
extern "C" void PushMap(void);
extern "C" void PushFixedShipPart(
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
			  );
extern "C" void PushVariableShipPart(
			  int Visible);
extern "C" void PushShot(
			  int TypeWeaponry,
			  int CurrentCoordinatesX,
			  int CurrentCoordinatesY,
			  int TargetCoordinatesX,
			  int TargetCoordinatesY
			  );
extern "C" int SendData(void);
extern "C" void PushShipsDummy(void);
extern "C" void PushShotsDummy(void);


#endif