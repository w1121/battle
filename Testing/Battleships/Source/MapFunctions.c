#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define  MAX_FLEETS  8

#define FleetPath "d:/Battleships/Maps/Fleets.dat"

typedef struct tagMap
{
	int MaxX;
	int MaxY;
	char * MapVal;
} stMap;

static stMap * Map;
static const char * MapPath    = "d:/Battleships/Maps/map1.map";
static int FleetCoords [MAX_FLEETS][2];
static int MapReaded = 0;
static int CoordsCalculated = 0;

char * getPath(void);
int readMap();

char * getPath()
{
	int StrSize;
	char * PathStr;
	
	StrSize = strlen(MapPath);
	PathStr = (char *)malloc((StrSize + 1) * sizeof(char));
	return strcpy(PathStr,MapPath);
}

int readMap()
{
	FILE * MapFile = NULL;
	int MapSize[2];
	
	Map = (stMap *)malloc(sizeof(stMap));
	if (Map == NULL)
		return 0;
	Map->MapVal = NULL;
	Map->MaxX = 0;
	Map->MaxY = 0;
	
	MapFile = fopen(MapPath,"r");
	if (MapFile == NULL)
		return 0;
	
	if (fread(MapSize,sizeof(int),2,MapFile) < 2)
		return 0;
	
	Map->MaxX = MapSize[0];
	Map->MaxY = MapSize[1];
	
	Map->MapVal = (char *)malloc(Map->MaxX * Map->MaxY * sizeof(char));
	if (fread(Map->MapVal,Map->MaxX * Map->MaxY * sizeof(char),1,MapFile) < 1)
		return 0;
	
	fclose(MapFile);
	MapReaded = 1;
	return 1;
}

int checkMap(void)
{
	if (!MapReaded)
		return readMap();
	return 1;
}

int getDeepXY(int x, int y)
{
	if (!MapReaded)
		if (!readMap()) 
			return -1;

	if ((x > Map->MaxX)||(y > Map->MaxY)||(x <= 0)||(y <= 0))
		return -1;
	return Map->MapVal[Map->MaxX * (y - 1) + x - 1];
}

int generatePoints(int Radius, int Deep, int NumOfStrat)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int x = 0;
	int y = 0;
	int NumGeneratedFleets = 0;
	int Generated = 0;
	int BankFound = 0;
	int IncorrectCoords = 0;
	int Count = 0;
	float XCoef;
	float YCoef;

	FILE * FleetFile=fopen(FleetPath,"r");
	
	if (!MapReaded)
		if (!readMap()) 
			return 0;
	
	if ((Deep > 4)||(Deep < 1)||(Radius < 0)||(NumOfStrat < 2))
		return 0;
	
	CoordsCalculated = 1;	
	XCoef = (float)Map->MaxX/RAND_MAX;
	YCoef = (float)Map->MaxY/RAND_MAX;

	for (NumGeneratedFleets = 0; NumGeneratedFleets < NumOfStrat; NumGeneratedFleets++ )
	{
		Generated = 0;
		Count = 0;

		if(FleetFile && !feof(FleetFile))
		{
			fscanf(FleetFile,"%d %d\n",FleetCoords[NumGeneratedFleets],FleetCoords[NumGeneratedFleets]+1);
			continue;
		}

		srand((unsigned)time(NULL));
		while (!Generated)
		{
			if (Count > 1000000)
				return 0;
			else
				Count++;

			BankFound = 0;
			IncorrectCoords = 0;
			
			x = (int)((float)XCoef * rand());
			y = (int)((float)YCoef * rand());
			
			for (k = 0;k < NumGeneratedFleets;k++)
				if((abs(x - FleetCoords[k][0]) <= (MAX_FLEETS - NumOfStrat + 2) * Radius )&&
				   (abs(y - FleetCoords[k][1]) <= (MAX_FLEETS - NumOfStrat + 2) * Radius ))
				{
					IncorrectCoords = 1;
					break;
				}
				
			if (!IncorrectCoords)
			{
				if ((x - Radius >= 1)&&(x + Radius < Map->MaxX - 1)&&
				    (y - Radius >= 1)&&(y + Radius < Map->MaxY - 1))
				{
					for (j = y - Radius;j <= y + Radius;j++)
					{
						for (i = x - Radius;i <= x + Radius;i++)
							if (Map->MapVal[(j - 1) * Map->MaxX + i] < Deep)
							{
								BankFound = 1;
								break;
							}
						if (BankFound)
							break;
					}
					if (!BankFound)
					{
						FleetCoords[NumGeneratedFleets][0] = x;
						FleetCoords[NumGeneratedFleets][1] = y;
						Generated  = 1;
					}
				}
			}
		}
	}
	if(FleetFile) fclose(FleetFile);
	return 1;
}

int getFleetPointX(int StratNum)
{
	if ((StratNum < 0 )||(StratNum > MAX_FLEETS))
		return -1;
	if (!CoordsCalculated)
		return -1;
	return FleetCoords[StratNum - 1][0];
}

int getFleetPointY(int StratNum)
{
	if ((StratNum < 0 )||(StratNum > MAX_FLEETS))
		return -1;
	if (!CoordsCalculated)
		return -1;
	return FleetCoords[StratNum - 1][1];
}

void FreeAll()
{
	if (Map != NULL)
	{
		if (Map ->MapVal != NULL)
			free(Map->MapVal);
		free(Map);
	}
}
