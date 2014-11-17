###############################################################################
#
# COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
#
# Description: Abstract Test Suite 'battleshipstest'
#
###############################################################################
# command line: 'c:\Work\Testing\TAT\bin\atsgen.exe -m @C:/Work/Testing/BSTest/work/macroprc.lst -o C:/Work/Testing/BSTest/work/battleshipstest.tcl -incl types.h model.ifc -cname default -tt -asn1 -state -mf -nia -nic -config C:/Work/Testing/BSTest/config.sic '

set INSTANCE_NAME 0
set INSTANCE_TYPE 1
set INSTANCE_ATTR 2
set INSTANCE_PARAMS 3
set INSTANCE_STYPES 4
set INSTANCE_FIELDS 5
set INSTANCE_PARAMTYPE 6
set INSTANCE_VARARG 7
set INSTANCE_PRIORITY 8
set INSTANCE_RECORD_SIZE 9
set InstanceSet [ list \
 "Simulator" "env" [ list  ] [ list ] [ list ] [ list ] "" 0 100\
 "Strategy" "model" [ list  ] [ list ] [ list ] [ list ] "" 0 100\
 ]
set SIGNAL_NAME 0
set SIGNAL_TYPE 1
set SIGNAL_ATTR 2
set SIGNAL_PARAMS 3
set SIGNAL_STYPES 4
set SIGNAL_FIELDS 5
set SIGNAL_PARAMTYPE 6
set SIGNAL_VARARG 7
set SIGNAL_PRIORITY 8
set SIGNAL_RECORD_SIZE 9
set SignalSet [list \
"DestroyShip"  "in"  [list ] [ list "int" "int" "int" ] [ list -1 -1 -1 ] [ list Param1 Param2 Param3 ] "C" 0 100\
"FinishSimulation"  "out"  [list ] [ list "int" "int" ] [ list -1 -1 ] [ list Param1 Param2 ] "C" 0 100\
"InitDataComplete"  "in"  [list ] [ list ] [ list ] [ list ] "C" 0 100\
"OrderFireReject"  "in"  [list ] [ list "int" "int" ] [ list -1 -1 ] [ list Param1 Param2 ] "C" 0 100\
"OrderFireRequest"  "out"  [list ] [ list "int" "int" "int" "tStringShotsList" ] [ list -1 -1 -1 10 ] [ list Param1 Param2 Param3 Param4 ] "C" 0 100\
"OrderFireResponse"  "in"  [list ] [ list "int" "int" ] [ list -1 -1 ] [ list Param1 Param2 ] "C" 0 100\
"OrderSpeedReject"  "in"  [list ] [ list "int" "int" ] [ list -1 -1 ] [ list Param1 Param2 ] "C" 0 100\
"OrderSpeedRequest"  "out"  [list ] [ list "int" "int" "float" ] [ list -1 -1 -1 ] [ list Param1 Param2 Param3 ] "C" 0 100\
"OrderSpeedResponse"  "in"  [list ] [ list "int" "int" ] [ list -1 -1 ] [ list Param1 Param2 ] "C" 0 100\
"OrderTurnReject"  "in"  [list ] [ list "int" "int" ] [ list -1 -1 ] [ list Param1 Param2 ] "C" 0 100\
"OrderTurnRequest"  "out"  [list ] [ list "int" "int" "int" ] [ list -1 -1 -1 ] [ list Param1 Param2 Param3 ] "C" 0 100\
"OrderTurnResponse"  "in"  [list ] [ list "int" "int" ] [ list -1 -1 ] [ list Param1 Param2 ] "C" 0 100\
"ShipsCurrentState"  "in"  [list ] [ list "int" "tStringShipCurrentState" ] [ list -1 9 ] [ list Param1 Param2 ] "C" 0 100\
"StartSimulation"  "in"  [list ] [ list "int" ] [ list -1 ] [ list Param1 ] "C" 0 100\
"StrategyLose"  "in"  [list ] [ list "int" ] [ list -1 ] [ list Param1 ] "C" 0 100\
"StrategyVictory"  "in"  [list ] [ list "int" ] [ list -1 ] [ list Param1 ] "C" 0 100\
"VisibleShip"  "in"  [list ] [ list "int" "tStringEnemyShip" ] [ list -1 8 ] [ list Param1 Param2 ] "C" 0 100\
"messageError"  "out"  [list ] [ list "SDL_Charstring" ] [ list -1 ] [ list Param1 ] "C" 0 100\
"messageLog"  "out"  [list ] [ list "SDL_Charstring" ] [ list -1 ] [ list Param1 ] "C" 0 100\
"resetStrategy"  "in"  [list ] [ list ] [ list ] [ list ] "C" 0 100\
]
set TestCaseNames [list "Test0001N0" "Test0008N0" "Test0014N0" "Test0022N0" "Test0032N0" "Test0001N1" "Test0008N1" "Test0014N1" "Test0022N1" "Test0032N1" "Test0001N2" "Test0008N2" "Test0014N2" "Test0022N2" "Test0032N2"]
set DATATYPE_NAME 0
set DATATYPE_TYPE 1
set DATATYPE_DIMENSION 2
set DATATYPE_STYPE 3
set DATATYPE_ATTR 4
set DATATYPE_ENUM 5
set DATATYPE_PARAMTYPE 6
set DATATYPE_DATACLASS 7
set DATATYPE_MEMBERS 8
set DATATYPE_RECORD_SIZE 9
set DataTypeSet [list \
		"tArrayAmountAmmunition" "int" "4" -1 [list ] [ list ] "SDL" "" [list]\
		"tArrayAmountLaunchUnit" "int" "4" -1 [list ] [ list ] "SDL" "" [list]\
		"tArrayShipsCharacter" "tShipCharacter" "5" 6 [list ] [ list ] "SDL" "" [list]\
		"tArrayWeaponryCharacter" "tWeaponryCharacter" "4" 11 [list ] [ list ] "SDL" "" [list]\
		"tCoordinates" "struct" "" -1 [list ] [ list ] "SDL" "" [list [list "x" "int" "" -1] [list "y" "int" "" -1]]\
		"tEnemyShip" "struct" "" -1 [list ] [ list ] "SDL" "" [list [list "ShipId" "int" "" -1] [list "StrategyId" "int" "" -1] [list "TypeShip" "int" "" -1] [list "Speed" "int" "" -1] [list "Heading" "int" "" -1] [list "Coordinates" "tCoordinates" "" 4]]\
		"tShipCharacter" "struct" "" -1 [list ] [ list ] "SDL" "" [list [list "Range" "int" "" -1] [list "MaxSpeed" "int" "" -1] [list "MaxAcceleration" "int" "" -1] [list "MaxResource" "int" "" -1] [list "MaxAmmunition" "tArrayAmountAmmunition" "" 0] [list "MaxLaunchUnit" "tArrayAmountLaunchUnit" "" 1] [list "MaxDeep" "int" "" -1]]\
		"tShipCurrentState" "struct" "" -1 [list ] [ list ] "SDL" "" [list [list "ShipId" "int" "" -1] [list "StrategyId" "int" "" -1] [list "TypeShip" "int" "" -1] [list "Speed" "int" "" -1] [list "Heading" "int" "" -1] [list "Resource" "int" "" -1] [list "Coordinates" "tCoordinates" "" 4] [list "Ammunition" "tArrayAmountAmmunition" "" 0]]\
		"tStringEnemyShip" "tEnemyShip" "string" 5 [list ] [ list ] "SDL" "" [list]\
		"tStringShipCurrentState" "tShipCurrentState" "string" 7 [list ] [ list ] "SDL" "" [list]\
		"tStringShotsList" "tCoordinates" "string" 4 [list ] [ list ] "SDL" "" [list]\
		"tWeaponryCharacter" "struct" "" -1 [list ] [ list ] "SDL" "" [list [list "Power" "int" "" -1] [list "Range" "int" "" -1] [list "Speed" "int" "" -1]]\
]
set IncludeFiles [ list "types.h" "model.ifc"  ]
set CONFIGURATION ""
set ATS_EVENT_TYPE 0
set ATS_EVENT_TIME 1
set ATS_EVENT_OTIME 2
set ATS_EVENT_PTIME 3
set ATS_EVENT_COND 4
set ATS_EVENT_INST 5
set ATS_EVENT_VERD 6
set ATS_EVENT_SREF 7
set ATS_EVENT_COMMENT 8
set ATS_EVENT_LABEL 9
set ATS_EVENT_DATA 10

StartTestSuite "battleshipstest"


StartTestCase "Test0001N0" 1 [ list ]\
[ list ]\
[ list ]  0 0 \
[ list \
"InitDataComplete()"\
"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 5, SOUTH_EAST, 70, (. 13, 15 .), (: 15, 15, 3, 0 :) .) :))"\
"StartSimulation(0)"\
"OrderTurnRequest(1,1,LEFT)"\
"FinishSimulation(0,1)"\
] [ list  ]\
[ list  [list "C:\\Work\\Testing\\BSTest\\mpr\\Test0001.mpr" 1 5 14 ]\
[list "C:\\Work\\Testing\\BSTest\\work\\Test0001N0.mpr" 2 5 14 ]\
]\
[ list \
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  1  0  ]\
 [list  1  0  ]\
]\
[ list \
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 0  PASS  [list [list 0 5 ] [list 1 5 ] ] "" "" [list  2 [ list  ]  0 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 1  PASS  [list [list 0 7 ] [list 1 7 ] ] "" "" [list  12 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "1" "CRUISER" "5" "SOUTH_EAST" "70" [list STRUCT "13" "15"] [list ARRAY "15" "15" "3" "0"]]]  ]  ]  1 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 2  PASS  [list [list 0 9 ] [list 1 9 ] ] "" "" [list  13 [ list [ list "" "0"  ]  ]  2 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 3  PASS  [list [list 0 11 ] [list 1 11 ] ] "" "" [list  10 [ list [ list "" "1"  ] [ list "" "1"  ] [ list "" "LEFT"  ]  ]  3 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 4  PASS  [list [list 0 13 ] [list 1 13 ] ] "" "" [list  1 [ list [ list "" "0"  ] [ list "" "1"  ]  ]  4 [ list REL "0" "0" ] ] ]\
] 1 0

State 0 0 0 0 1 0 [list \
[list 0  1 ]\
]

State 1 0 0 0 1 0 [list \
[list 1  2 ]\
]

State 2 0 0 0 1 0 [list \
[list 2  3 ]\
]

State 3 0 0 1 0 0 [list \
[list 3  4 ]\
]

State 4 0 0 1 0 0 [list \
[list 4  -1 ]\
]

FinishTestCase "Test0001N0"


StartTestCase "Test0008N0" 1 [ list ]\
[ list ]\
[ list ]  0 0 \
[ list \
"InitDataComplete()"\
"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 8, EAST, 70, (. 1, 1 .), (: 15, 15, 3, 0 :) .) :))"\
"VisibleShip(1,(: (. 2, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))"\
"StartSimulation(0)"\
"OrderSpeedRequest(1,1,-1.0000)"\
"FinishSimulation(0,1)"\
"StrategyVictory(1)"\
] [ list  ]\
[ list  [list "C:\\Work\\Testing\\BSTest\\mpr\\Test0008.mpr" 1 5 18 ]\
[list "C:\\Work\\Testing\\BSTest\\work\\Test0008N0.mpr" 2 5 18 ]\
]\
[ list \
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  0  1  ]\
]\
[ list \
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 0  PASS  [list [list 0 5 ] [list 1 5 ] ] "" "" [list  2 [ list  ]  0 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 1  PASS  [list [list 0 7 ] [list 1 7 ] ] "" "" [list  12 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "1" "CRUISER" "8" "EAST" "70" [list STRUCT "1" "1"] [list ARRAY "15" "15" "3" "0"]]]  ]  ]  1 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 2  PASS  [list [list 0 9 ] [list 1 9 ] ] "" "" [list  16 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "2" "2" "REPAIR_BOAT" "0" "NORTH_WEST" [list STRUCT "25" "25"]]]  ]  ]  2 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 3  PASS  [list [list 0 11 ] [list 1 11 ] ] "" "" [list  13 [ list [ list "" "0"  ]  ]  3 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 4  PASS  [list [list 0 13 ] [list 1 13 ] ] "" "" [list  7 [ list [ list "" "1"  ] [ list "" "1"  ] [ list "" "-1.0000"  ]  ]  4 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 5  PASS  [list [list 0 15 ] [list 1 15 ] ] "" "" [list  1 [ list [ list "" "0"  ] [ list "" "1"  ]  ]  5 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 6  PASS  [list [list 0 17 ] [list 1 17 ] ] "" "" [list  15 [ list [ list "" "1"  ]  ]  6 [ list REL "0" "0" ] ] ]\
] 1 0

State 0 0 0 0 1 0 [list \
[list 0  1 ]\
]

State 1 0 0 0 1 0 [list \
[list 1  2 ]\
]

State 2 0 0 0 1 0 [list \
[list 2  3 ]\
]

State 3 0 0 0 1 0 [list \
[list 3  4 ]\
]

State 4 0 0 1 0 0 [list \
[list 4  5 ]\
]

State 5 0 0 1 0 0 [list \
[list 5  6 ]\
]

State 6 0 0 0 1 0 [list \
[list 6  -1 ]\
]

FinishTestCase "Test0008N0"


StartTestCase "Test0014N0" 1 [ list ]\
[ list ]\
[ list ]  0 0 \
[ list \
"InitDataComplete()"\
"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 0, SOUTH, 70, (. 10, 10 .), (: 0, 1, 0, 0 :) .) :))"\
"VisibleShip(1,(: (. 2, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))"\
"StartSimulation(0)"\
"OrderTurnRequest(1,1,LEFT)"\
"FinishSimulation(0,1)"\
"StrategyVictory(1)"\
] [ list  ]\
[ list  [list "C:\\Work\\Testing\\BSTest\\mpr\\Test0014.mpr" 1 5 18 ]\
[list "C:\\Work\\Testing\\BSTest\\work\\Test0014N0.mpr" 2 5 18 ]\
]\
[ list \
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  0  1  ]\
]\
[ list \
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 0  PASS  [list [list 0 5 ] [list 1 5 ] ] "" "" [list  2 [ list  ]  0 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 1  PASS  [list [list 0 7 ] [list 1 7 ] ] "" "" [list  12 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "1" "CRUISER" "0" "SOUTH" "70" [list STRUCT "10" "10"] [list ARRAY "0" "1" "0" "0"]]]  ]  ]  1 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 2  PASS  [list [list 0 9 ] [list 1 9 ] ] "" "" [list  16 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "2" "2" "REPAIR_BOAT" "0" "NORTH_WEST" [list STRUCT "25" "25"]]]  ]  ]  2 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 3  PASS  [list [list 0 11 ] [list 1 11 ] ] "" "" [list  13 [ list [ list "" "0"  ]  ]  3 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 4  PASS  [list [list 0 13 ] [list 1 13 ] ] "" "" [list  10 [ list [ list "" "1"  ] [ list "" "1"  ] [ list "" "LEFT"  ]  ]  4 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 5  PASS  [list [list 0 15 ] [list 1 15 ] ] "" "" [list  1 [ list [ list "" "0"  ] [ list "" "1"  ]  ]  5 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 6  PASS  [list [list 0 17 ] [list 1 17 ] ] "" "" [list  15 [ list [ list "" "1"  ]  ]  6 [ list REL "0" "0" ] ] ]\
] 1 0

State 0 0 0 0 1 0 [list \
[list 0  1 ]\
]

State 1 0 0 0 1 0 [list \
[list 1  2 ]\
]

State 2 0 0 0 1 0 [list \
[list 2  3 ]\
]

State 3 0 0 0 1 0 [list \
[list 3  4 ]\
]

State 4 0 0 1 0 0 [list \
[list 4  5 ]\
]

State 5 0 0 1 0 0 [list \
[list 5  6 ]\
]

State 6 0 0 0 1 0 [list \
[list 6  -1 ]\
]

FinishTestCase "Test0014N0"


StartTestCase "Test0022N0" 1 [ list ]\
[ list ]\
[ list ]  0 0 \
[ list \
"InitDataComplete()"\
"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 0, SOUTH, 20, (. 10, 10 .), (: 0, 0, 0, 0 :) .), (. 2, 1, REPAIR_BOAT, 0, SOUTH, 25, (. 12, 10 .), (: 0, 0, 0, 10 :) .), (. 3, 1, CRUISER, 0, SOUTH, 5, (. 15, 10 .), (: 0, 0, 0, 0 :) .) :))"\
"VisibleShip(1,(: (. 1, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))"\
"StartSimulation(0)"\
"OrderTurnRequest(1,1,LEFT)"\
"OrderFireRequest(2,1,REPAIR,(: (. 15, 10 .), (. 10, 10 .) :))"\
"OrderTurnRequest(3,1,LEFT)"\
"FinishSimulation(0,1)"\
"StrategyVictory(1)"\
] [ list  ]\
[ list  [list "C:\\Work\\Testing\\BSTest\\mpr\\Test0022.mpr" 1 5 22 ]\
[list "C:\\Work\\Testing\\BSTest\\work\\Test0022N0.mpr" 2 5 22 ]\
]\
[ list \
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  0  1  ]\
]\
[ list \
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 0  PASS  [list [list 0 5 ] [list 1 5 ] ] "" "" [list  2 [ list  ]  0 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 1  PASS  [list [list 0 7 ] [list 1 7 ] ] "" "" [list  12 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "1" "CRUISER" "0" "SOUTH" "20" [list STRUCT "10" "10"] [list ARRAY "0" "0" "0" "0"]] [list STRUCT "2" "1" "REPAIR_BOAT" "0" "SOUTH" "25" [list STRUCT "12" "10"] [list ARRAY "0" "0" "0" "10"]] [list STRUCT "3" "1" "CRUISER" "0" "SOUTH" "5" [list STRUCT "15" "10"] [list ARRAY "0" "0" "0" "0"]]]  ]  ]  1 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 2  PASS  [list [list 0 9 ] [list 1 9 ] ] "" "" [list  16 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "2" "REPAIR_BOAT" "0" "NORTH_WEST" [list STRUCT "25" "25"]]]  ]  ]  2 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 3  PASS  [list [list 0 11 ] [list 1 11 ] ] "" "" [list  13 [ list [ list "" "0"  ]  ]  3 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 4  PASS  [list [list 0 13 ] [list 1 13 ] ] "" "" [list  10 [ list [ list "" "1"  ] [ list "" "1"  ] [ list "" "LEFT"  ]  ]  4 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 5  PASS  [list [list 0 15 ] [list 1 15 ] ] "" "" [list  4 [ list [ list "" "2"  ] [ list "" "1"  ] [ list "" "REPAIR"  ] [ list "" [list ARRAY [list STRUCT "15" "10"] [list STRUCT "10" "10"]]  ]  ]  5 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 6  PASS  [list [list 0 17 ] [list 1 17 ] ] "" "" [list  10 [ list [ list "" "3"  ] [ list "" "1"  ] [ list "" "LEFT"  ]  ]  6 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 7  PASS  [list [list 0 19 ] [list 1 19 ] ] "" "" [list  1 [ list [ list "" "0"  ] [ list "" "1"  ]  ]  7 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 8  PASS  [list [list 0 21 ] [list 1 21 ] ] "" "" [list  15 [ list [ list "" "1"  ]  ]  8 [ list REL "0" "0" ] ] ]\
] 1 0

State 0 0 0 0 1 0 [list \
[list 0  1 ]\
]

State 1 0 0 0 1 0 [list \
[list 1  2 ]\
]

State 2 0 0 0 1 0 [list \
[list 2  3 ]\
]

State 3 0 0 0 1 0 [list \
[list 3  4 ]\
]

State 4 0 0 1 0 0 [list \
[list 4  5 ]\
]

State 5 0 0 1 0 0 [list \
[list 5  6 ]\
]

State 6 0 0 1 0 0 [list \
[list 6  7 ]\
]

State 7 0 0 1 0 0 [list \
[list 7  8 ]\
]

State 8 0 0 0 1 0 [list \
[list 8  -1 ]\
]

FinishTestCase "Test0022N0"


StartTestCase "Test0032N0" 1 [ list ]\
[ list ]\
[ list ]  0 0 \
[ list \
"InitDataComplete()"\
"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 8, EAST, 70, (. 40, 25 .), (: 15, 15, 3, 0 :) .) :))"\
"VisibleShip(1,(: (. 1, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 15, 15 .) .) :))"\
"StartSimulation(0)"\
"OrderSpeedRequest(1,1,-1.0000)"\
"FinishSimulation(0,1)"\
"StrategyVictory(1)"\
] [ list  ]\
[ list  [list "C:\\Work\\Testing\\BSTest\\mpr\\Test0032.mpr" 1 5 18 ]\
[list "C:\\Work\\Testing\\BSTest\\work\\Test0032N0.mpr" 2 5 18 ]\
]\
[ list \
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  0  1  ]\
]\
[ list \
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 0  PASS  [list [list 0 5 ] [list 1 5 ] ] "" "" [list  2 [ list  ]  0 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 1  PASS  [list [list 0 7 ] [list 1 7 ] ] "" "" [list  12 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "1" "CRUISER" "8" "EAST" "70" [list STRUCT "40" "25"] [list ARRAY "15" "15" "3" "0"]]]  ]  ]  1 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 2  PASS  [list [list 0 9 ] [list 1 9 ] ] "" "" [list  16 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "2" "REPAIR_BOAT" "0" "NORTH_WEST" [list STRUCT "15" "15"]]]  ]  ]  2 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 3  PASS  [list [list 0 11 ] [list 1 11 ] ] "" "" [list  13 [ list [ list "" "0"  ]  ]  3 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 4  PASS  [list [list 0 13 ] [list 1 13 ] ] "" "" [list  7 [ list [ list "" "1"  ] [ list "" "1"  ] [ list "" "-1.0000"  ]  ]  4 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 5  PASS  [list [list 0 15 ] [list 1 15 ] ] "" "" [list  1 [ list [ list "" "0"  ] [ list "" "1"  ]  ]  5 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 6  PASS  [list [list 0 17 ] [list 1 17 ] ] "" "" [list  15 [ list [ list "" "1"  ]  ]  6 [ list REL "0" "0" ] ] ]\
] 1 0

State 0 0 0 0 1 0 [list \
[list 0  1 ]\
]

State 1 0 0 0 1 0 [list \
[list 1  2 ]\
]

State 2 0 0 0 1 0 [list \
[list 2  3 ]\
]

State 3 0 0 0 1 0 [list \
[list 3  4 ]\
]

State 4 0 0 1 0 0 [list \
[list 4  5 ]\
]

State 5 0 0 1 0 0 [list \
[list 5  6 ]\
]

State 6 0 0 0 1 0 [list \
[list 6  -1 ]\
]

FinishTestCase "Test0032N0"


StartTestCase "Test0001N1" 1 [ list ]\
[ list ]\
[ list ]  0 0 \
[ list \
"InitDataComplete()"\
"ShipsCurrentState(1,(: (. 1, 1, DESTROYER, 10, SOUTH_EAST, 70, (. 13, 15 .), (: 15, 15, 3, 0 :) .) :))"\
"StartSimulation(0)"\
"OrderTurnRequest(1,1,LEFT)"\
"FinishSimulation(0,1)"\
] [ list  ]\
[ list  [list "C:\\Work\\Testing\\BSTest\\mpr\\Test0001.mpr" 1 5 14 ]\
[list "C:\\Work\\Testing\\BSTest\\work\\Test0001N1.mpr" 2 5 14 ]\
]\
[ list \
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  1  0  ]\
 [list  1  0  ]\
]\
[ list \
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 0  PASS  [list [list 0 5 ] [list 1 5 ] ] "" "" [list  2 [ list  ]  0 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 1  PASS  [list [list 0 7 ] [list 1 7 ] ] "" "" [list  12 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "1" "DESTROYER" "10" "SOUTH_EAST" "70" [list STRUCT "13" "15"] [list ARRAY "15" "15" "3" "0"]]]  ]  ]  1 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 2  PASS  [list [list 0 9 ] [list 1 9 ] ] "" "" [list  13 [ list [ list "" "0"  ]  ]  2 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 3  PASS  [list [list 0 11 ] [list 1 11 ] ] "" "" [list  10 [ list [ list "" "1"  ] [ list "" "1"  ] [ list "" "LEFT"  ]  ]  3 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 4  PASS  [list [list 0 13 ] [list 1 13 ] ] "" "" [list  1 [ list [ list "" "0"  ] [ list "" "1"  ]  ]  4 [ list REL "0" "0" ] ] ]\
] 1 0

State 0 0 0 0 1 0 [list \
[list 0  1 ]\
]

State 1 0 0 0 1 0 [list \
[list 1  2 ]\
]

State 2 0 0 0 1 0 [list \
[list 2  3 ]\
]

State 3 0 0 1 0 0 [list \
[list 3  4 ]\
]

State 4 0 0 1 0 0 [list \
[list 4  -1 ]\
]

FinishTestCase "Test0001N1"


StartTestCase "Test0008N1" 1 [ list ]\
[ list ]\
[ list ]  0 0 \
[ list \
"InitDataComplete()"\
"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 8, EAST, 70, (. 1, 1 .), (: 15, 15, 3, 0 :) .) :))"\
"VisibleShip(1,(: (. 2, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))"\
"StartSimulation(0)"\
"OrderSpeedRequest(1,1,-1.0000)"\
"FinishSimulation(0,1)"\
"StrategyVictory(1)"\
] [ list  ]\
[ list  [list "C:\\Work\\Testing\\BSTest\\mpr\\Test0008.mpr" 1 5 18 ]\
[list "C:\\Work\\Testing\\BSTest\\work\\Test0008N1.mpr" 2 5 18 ]\
]\
[ list \
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  0  1  ]\
]\
[ list \
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 0  PASS  [list [list 0 5 ] [list 1 5 ] ] "" "" [list  2 [ list  ]  0 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 1  PASS  [list [list 0 7 ] [list 1 7 ] ] "" "" [list  12 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "1" "CRUISER" "8" "EAST" "70" [list STRUCT "1" "1"] [list ARRAY "15" "15" "3" "0"]]]  ]  ]  1 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 2  PASS  [list [list 0 9 ] [list 1 9 ] ] "" "" [list  16 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "2" "2" "REPAIR_BOAT" "0" "NORTH_WEST" [list STRUCT "25" "25"]]]  ]  ]  2 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 3  PASS  [list [list 0 11 ] [list 1 11 ] ] "" "" [list  13 [ list [ list "" "0"  ]  ]  3 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 4  PASS  [list [list 0 13 ] [list 1 13 ] ] "" "" [list  7 [ list [ list "" "1"  ] [ list "" "1"  ] [ list "" "-1.0000"  ]  ]  4 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 5  PASS  [list [list 0 15 ] [list 1 15 ] ] "" "" [list  1 [ list [ list "" "0"  ] [ list "" "1"  ]  ]  5 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 6  PASS  [list [list 0 17 ] [list 1 17 ] ] "" "" [list  15 [ list [ list "" "1"  ]  ]  6 [ list REL "0" "0" ] ] ]\
] 1 0

State 0 0 0 0 1 0 [list \
[list 0  1 ]\
]

State 1 0 0 0 1 0 [list \
[list 1  2 ]\
]

State 2 0 0 0 1 0 [list \
[list 2  3 ]\
]

State 3 0 0 0 1 0 [list \
[list 3  4 ]\
]

State 4 0 0 1 0 0 [list \
[list 4  5 ]\
]

State 5 0 0 1 0 0 [list \
[list 5  6 ]\
]

State 6 0 0 0 1 0 [list \
[list 6  -1 ]\
]

FinishTestCase "Test0008N1"


StartTestCase "Test0014N1" 1 [ list ]\
[ list ]\
[ list ]  0 0 \
[ list \
"InitDataComplete()"\
"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 0, SOUTH, 70, (. 10, 10 .), (: 0, 1, 0, 0 :) .) :))"\
"VisibleShip(1,(: (. 2, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))"\
"StartSimulation(0)"\
"OrderTurnRequest(1,1,LEFT)"\
"FinishSimulation(0,1)"\
"StrategyVictory(1)"\
] [ list  ]\
[ list  [list "C:\\Work\\Testing\\BSTest\\mpr\\Test0014.mpr" 1 5 18 ]\
[list "C:\\Work\\Testing\\BSTest\\work\\Test0014N1.mpr" 2 5 18 ]\
]\
[ list \
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  0  1  ]\
]\
[ list \
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 0  PASS  [list [list 0 5 ] [list 1 5 ] ] "" "" [list  2 [ list  ]  0 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 1  PASS  [list [list 0 7 ] [list 1 7 ] ] "" "" [list  12 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "1" "CRUISER" "0" "SOUTH" "70" [list STRUCT "10" "10"] [list ARRAY "0" "1" "0" "0"]]]  ]  ]  1 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 2  PASS  [list [list 0 9 ] [list 1 9 ] ] "" "" [list  16 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "2" "2" "REPAIR_BOAT" "0" "NORTH_WEST" [list STRUCT "25" "25"]]]  ]  ]  2 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 3  PASS  [list [list 0 11 ] [list 1 11 ] ] "" "" [list  13 [ list [ list "" "0"  ]  ]  3 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 4  PASS  [list [list 0 13 ] [list 1 13 ] ] "" "" [list  10 [ list [ list "" "1"  ] [ list "" "1"  ] [ list "" "LEFT"  ]  ]  4 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 5  PASS  [list [list 0 15 ] [list 1 15 ] ] "" "" [list  1 [ list [ list "" "0"  ] [ list "" "1"  ]  ]  5 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 6  PASS  [list [list 0 17 ] [list 1 17 ] ] "" "" [list  15 [ list [ list "" "1"  ]  ]  6 [ list REL "0" "0" ] ] ]\
] 1 0

State 0 0 0 0 1 0 [list \
[list 0  1 ]\
]

State 1 0 0 0 1 0 [list \
[list 1  2 ]\
]

State 2 0 0 0 1 0 [list \
[list 2  3 ]\
]

State 3 0 0 0 1 0 [list \
[list 3  4 ]\
]

State 4 0 0 1 0 0 [list \
[list 4  5 ]\
]

State 5 0 0 1 0 0 [list \
[list 5  6 ]\
]

State 6 0 0 0 1 0 [list \
[list 6  -1 ]\
]

FinishTestCase "Test0014N1"


StartTestCase "Test0022N1" 1 [ list ]\
[ list ]\
[ list ]  0 0 \
[ list \
"InitDataComplete()"\
"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 0, SOUTH, 20, (. 10, 10 .), (: 0, 0, 0, 0 :) .), (. 2, 1, REPAIR_BOAT, 0, SOUTH, 25, (. 12, 10 .), (: 0, 0, 0, 10 :) .), (. 3, 1, CRUISER, 0, SOUTH, 5, (. 15, 10 .), (: 0, 0, 0, 0 :) .) :))"\
"VisibleShip(1,(: (. 1, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))"\
"StartSimulation(0)"\
"OrderTurnRequest(1,1,LEFT)"\
"OrderFireRequest(2,1,REPAIR,(: (. 15, 10 .), (. 10, 10 .) :))"\
"OrderTurnRequest(3,1,LEFT)"\
"FinishSimulation(0,1)"\
"StrategyVictory(1)"\
] [ list  ]\
[ list  [list "C:\\Work\\Testing\\BSTest\\mpr\\Test0022.mpr" 1 5 22 ]\
[list "C:\\Work\\Testing\\BSTest\\work\\Test0022N1.mpr" 2 5 22 ]\
]\
[ list \
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  0  1  ]\
]\
[ list \
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 0  PASS  [list [list 0 5 ] [list 1 5 ] ] "" "" [list  2 [ list  ]  0 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 1  PASS  [list [list 0 7 ] [list 1 7 ] ] "" "" [list  12 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "1" "CRUISER" "0" "SOUTH" "20" [list STRUCT "10" "10"] [list ARRAY "0" "0" "0" "0"]] [list STRUCT "2" "1" "REPAIR_BOAT" "0" "SOUTH" "25" [list STRUCT "12" "10"] [list ARRAY "0" "0" "0" "10"]] [list STRUCT "3" "1" "CRUISER" "0" "SOUTH" "5" [list STRUCT "15" "10"] [list ARRAY "0" "0" "0" "0"]]]  ]  ]  1 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 2  PASS  [list [list 0 9 ] [list 1 9 ] ] "" "" [list  16 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "2" "REPAIR_BOAT" "0" "NORTH_WEST" [list STRUCT "25" "25"]]]  ]  ]  2 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 3  PASS  [list [list 0 11 ] [list 1 11 ] ] "" "" [list  13 [ list [ list "" "0"  ]  ]  3 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 4  PASS  [list [list 0 13 ] [list 1 13 ] ] "" "" [list  10 [ list [ list "" "1"  ] [ list "" "1"  ] [ list "" "LEFT"  ]  ]  4 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 5  PASS  [list [list 0 15 ] [list 1 15 ] ] "" "" [list  4 [ list [ list "" "2"  ] [ list "" "1"  ] [ list "" "REPAIR"  ] [ list "" [list ARRAY [list STRUCT "15" "10"] [list STRUCT "10" "10"]]  ]  ]  5 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 6  PASS  [list [list 0 17 ] [list 1 17 ] ] "" "" [list  10 [ list [ list "" "3"  ] [ list "" "1"  ] [ list "" "LEFT"  ]  ]  6 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 7  PASS  [list [list 0 19 ] [list 1 19 ] ] "" "" [list  1 [ list [ list "" "0"  ] [ list "" "1"  ]  ]  7 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 8  PASS  [list [list 0 21 ] [list 1 21 ] ] "" "" [list  15 [ list [ list "" "1"  ]  ]  8 [ list REL "0" "0" ] ] ]\
] 1 0

State 0 0 0 0 1 0 [list \
[list 0  1 ]\
]

State 1 0 0 0 1 0 [list \
[list 1  2 ]\
]

State 2 0 0 0 1 0 [list \
[list 2  3 ]\
]

State 3 0 0 0 1 0 [list \
[list 3  4 ]\
]

State 4 0 0 1 0 0 [list \
[list 4  5 ]\
]

State 5 0 0 1 0 0 [list \
[list 5  6 ]\
]

State 6 0 0 1 0 0 [list \
[list 6  7 ]\
]

State 7 0 0 1 0 0 [list \
[list 7  8 ]\
]

State 8 0 0 0 1 0 [list \
[list 8  -1 ]\
]

FinishTestCase "Test0022N1"


StartTestCase "Test0032N1" 1 [ list ]\
[ list ]\
[ list ]  0 0 \
[ list \
"InitDataComplete()"\
"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 8, EAST, 70, (. 40, 25 .), (: 15, 15, 3, 0 :) .) :))"\
"VisibleShip(1,(: (. 1, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 15, 15 .) .) :))"\
"StartSimulation(0)"\
"OrderSpeedRequest(1,1,-1.0000)"\
"FinishSimulation(0,1)"\
"StrategyVictory(1)"\
] [ list  ]\
[ list  [list "C:\\Work\\Testing\\BSTest\\mpr\\Test0032.mpr" 1 5 18 ]\
[list "C:\\Work\\Testing\\BSTest\\work\\Test0032N1.mpr" 2 5 18 ]\
]\
[ list \
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  0  1  ]\
]\
[ list \
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 0  PASS  [list [list 0 5 ] [list 1 5 ] ] "" "" [list  2 [ list  ]  0 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 1  PASS  [list [list 0 7 ] [list 1 7 ] ] "" "" [list  12 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "1" "CRUISER" "8" "EAST" "70" [list STRUCT "40" "25"] [list ARRAY "15" "15" "3" "0"]]]  ]  ]  1 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 2  PASS  [list [list 0 9 ] [list 1 9 ] ] "" "" [list  16 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "2" "REPAIR_BOAT" "0" "NORTH_WEST" [list STRUCT "15" "15"]]]  ]  ]  2 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 3  PASS  [list [list 0 11 ] [list 1 11 ] ] "" "" [list  13 [ list [ list "" "0"  ]  ]  3 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 4  PASS  [list [list 0 13 ] [list 1 13 ] ] "" "" [list  7 [ list [ list "" "1"  ] [ list "" "1"  ] [ list "" "-1.0000"  ]  ]  4 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 5  PASS  [list [list 0 15 ] [list 1 15 ] ] "" "" [list  1 [ list [ list "" "0"  ] [ list "" "1"  ]  ]  5 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 6  PASS  [list [list 0 17 ] [list 1 17 ] ] "" "" [list  15 [ list [ list "" "1"  ]  ]  6 [ list REL "0" "0" ] ] ]\
] 1 0

State 0 0 0 0 1 0 [list \
[list 0  1 ]\
]

State 1 0 0 0 1 0 [list \
[list 1  2 ]\
]

State 2 0 0 0 1 0 [list \
[list 2  3 ]\
]

State 3 0 0 0 1 0 [list \
[list 3  4 ]\
]

State 4 0 0 1 0 0 [list \
[list 4  5 ]\
]

State 5 0 0 1 0 0 [list \
[list 5  6 ]\
]

State 6 0 0 0 1 0 [list \
[list 6  -1 ]\
]

FinishTestCase "Test0032N1"


StartTestCase "Test0001N2" 1 [ list ]\
[ list ]\
[ list ]  0 0 \
[ list \
"InitDataComplete()"\
"ShipsCurrentState(1,(: (. 1, 1, REPAIR_BOAT, 15, SOUTH_EAST, 70, (. 13, 15 .), (: 15, 15, 3, 0 :) .) :))"\
"StartSimulation(0)"\
"OrderTurnRequest(1,1,LEFT)"\
"FinishSimulation(0,1)"\
] [ list  ]\
[ list  [list "C:\\Work\\Testing\\BSTest\\mpr\\Test0001.mpr" 1 5 14 ]\
[list "C:\\Work\\Testing\\BSTest\\work\\Test0001N2.mpr" 2 5 14 ]\
]\
[ list \
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  1  0  ]\
 [list  1  0  ]\
]\
[ list \
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 0  PASS  [list [list 0 5 ] [list 1 5 ] ] "" "" [list  2 [ list  ]  0 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 1  PASS  [list [list 0 7 ] [list 1 7 ] ] "" "" [list  12 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "1" "REPAIR_BOAT" "15" "SOUTH_EAST" "70" [list STRUCT "13" "15"] [list ARRAY "15" "15" "3" "0"]]]  ]  ]  1 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 2  PASS  [list [list 0 9 ] [list 1 9 ] ] "" "" [list  13 [ list [ list "" "0"  ]  ]  2 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 3  PASS  [list [list 0 11 ] [list 1 11 ] ] "" "" [list  10 [ list [ list "" "1"  ] [ list "" "1"  ] [ list "" "LEFT"  ]  ]  3 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 4  PASS  [list [list 0 13 ] [list 1 13 ] ] "" "" [list  1 [ list [ list "" "0"  ] [ list "" "1"  ]  ]  4 [ list REL "0" "0" ] ] ]\
] 1 0

State 0 0 0 0 1 0 [list \
[list 0  1 ]\
]

State 1 0 0 0 1 0 [list \
[list 1  2 ]\
]

State 2 0 0 0 1 0 [list \
[list 2  3 ]\
]

State 3 0 0 1 0 0 [list \
[list 3  4 ]\
]

State 4 0 0 1 0 0 [list \
[list 4  -1 ]\
]

FinishTestCase "Test0001N2"


StartTestCase "Test0008N2" 1 [ list ]\
[ list ]\
[ list ]  0 0 \
[ list \
"InitDataComplete()"\
"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 8, EAST, 70, (. 1, 1 .), (: 15, 15, 3, 0 :) .) :))"\
"VisibleShip(1,(: (. 2, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))"\
"StartSimulation(0)"\
"OrderSpeedRequest(1,1,-1.0000)"\
"FinishSimulation(0,1)"\
"StrategyVictory(1)"\
] [ list  ]\
[ list  [list "C:\\Work\\Testing\\BSTest\\mpr\\Test0008.mpr" 1 5 18 ]\
[list "C:\\Work\\Testing\\BSTest\\work\\Test0008N2.mpr" 2 5 18 ]\
]\
[ list \
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  0  1  ]\
]\
[ list \
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 0  PASS  [list [list 0 5 ] [list 1 5 ] ] "" "" [list  2 [ list  ]  0 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 1  PASS  [list [list 0 7 ] [list 1 7 ] ] "" "" [list  12 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "1" "CRUISER" "8" "EAST" "70" [list STRUCT "1" "1"] [list ARRAY "15" "15" "3" "0"]]]  ]  ]  1 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 2  PASS  [list [list 0 9 ] [list 1 9 ] ] "" "" [list  16 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "2" "2" "REPAIR_BOAT" "0" "NORTH_WEST" [list STRUCT "25" "25"]]]  ]  ]  2 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 3  PASS  [list [list 0 11 ] [list 1 11 ] ] "" "" [list  13 [ list [ list "" "0"  ]  ]  3 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 4  PASS  [list [list 0 13 ] [list 1 13 ] ] "" "" [list  7 [ list [ list "" "1"  ] [ list "" "1"  ] [ list "" "-1.0000"  ]  ]  4 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 5  PASS  [list [list 0 15 ] [list 1 15 ] ] "" "" [list  1 [ list [ list "" "0"  ] [ list "" "1"  ]  ]  5 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 6  PASS  [list [list 0 17 ] [list 1 17 ] ] "" "" [list  15 [ list [ list "" "1"  ]  ]  6 [ list REL "0" "0" ] ] ]\
] 1 0

State 0 0 0 0 1 0 [list \
[list 0  1 ]\
]

State 1 0 0 0 1 0 [list \
[list 1  2 ]\
]

State 2 0 0 0 1 0 [list \
[list 2  3 ]\
]

State 3 0 0 0 1 0 [list \
[list 3  4 ]\
]

State 4 0 0 1 0 0 [list \
[list 4  5 ]\
]

State 5 0 0 1 0 0 [list \
[list 5  6 ]\
]

State 6 0 0 0 1 0 [list \
[list 6  -1 ]\
]

FinishTestCase "Test0008N2"


StartTestCase "Test0014N2" 1 [ list ]\
[ list ]\
[ list ]  0 0 \
[ list \
"InitDataComplete()"\
"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 0, SOUTH, 70, (. 10, 10 .), (: 0, 1, 0, 0 :) .) :))"\
"VisibleShip(1,(: (. 2, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))"\
"StartSimulation(0)"\
"OrderTurnRequest(1,1,LEFT)"\
"FinishSimulation(0,1)"\
"StrategyVictory(1)"\
] [ list  ]\
[ list  [list "C:\\Work\\Testing\\BSTest\\mpr\\Test0014.mpr" 1 5 18 ]\
[list "C:\\Work\\Testing\\BSTest\\work\\Test0014N2.mpr" 2 5 18 ]\
]\
[ list \
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  0  1  ]\
]\
[ list \
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 0  PASS  [list [list 0 5 ] [list 1 5 ] ] "" "" [list  2 [ list  ]  0 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 1  PASS  [list [list 0 7 ] [list 1 7 ] ] "" "" [list  12 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "1" "CRUISER" "0" "SOUTH" "70" [list STRUCT "10" "10"] [list ARRAY "0" "1" "0" "0"]]]  ]  ]  1 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 2  PASS  [list [list 0 9 ] [list 1 9 ] ] "" "" [list  16 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "2" "2" "REPAIR_BOAT" "0" "NORTH_WEST" [list STRUCT "25" "25"]]]  ]  ]  2 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 3  PASS  [list [list 0 11 ] [list 1 11 ] ] "" "" [list  13 [ list [ list "" "0"  ]  ]  3 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 4  PASS  [list [list 0 13 ] [list 1 13 ] ] "" "" [list  10 [ list [ list "" "1"  ] [ list "" "1"  ] [ list "" "LEFT"  ]  ]  4 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 5  PASS  [list [list 0 15 ] [list 1 15 ] ] "" "" [list  1 [ list [ list "" "0"  ] [ list "" "1"  ]  ]  5 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 6  PASS  [list [list 0 17 ] [list 1 17 ] ] "" "" [list  15 [ list [ list "" "1"  ]  ]  6 [ list REL "0" "0" ] ] ]\
] 1 0

State 0 0 0 0 1 0 [list \
[list 0  1 ]\
]

State 1 0 0 0 1 0 [list \
[list 1  2 ]\
]

State 2 0 0 0 1 0 [list \
[list 2  3 ]\
]

State 3 0 0 0 1 0 [list \
[list 3  4 ]\
]

State 4 0 0 1 0 0 [list \
[list 4  5 ]\
]

State 5 0 0 1 0 0 [list \
[list 5  6 ]\
]

State 6 0 0 0 1 0 [list \
[list 6  -1 ]\
]

FinishTestCase "Test0014N2"


StartTestCase "Test0022N2" 1 [ list ]\
[ list ]\
[ list ]  0 0 \
[ list \
"InitDataComplete()"\
"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 0, SOUTH, 20, (. 10, 10 .), (: 0, 0, 0, 0 :) .), (. 2, 1, REPAIR_BOAT, 0, SOUTH, 25, (. 12, 10 .), (: 0, 0, 0, 10 :) .), (. 3, 1, CRUISER, 0, SOUTH, 5, (. 15, 10 .), (: 0, 0, 0, 0 :) .) :))"\
"VisibleShip(1,(: (. 1, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 25, 25 .) .) :))"\
"StartSimulation(0)"\
"OrderTurnRequest(1,1,LEFT)"\
"OrderFireRequest(2,1,REPAIR,(: (. 15, 10 .), (. 10, 10 .) :))"\
"OrderTurnRequest(3,1,LEFT)"\
"FinishSimulation(0,1)"\
"StrategyVictory(1)"\
] [ list  ]\
[ list  [list "C:\\Work\\Testing\\BSTest\\mpr\\Test0022.mpr" 1 5 22 ]\
[list "C:\\Work\\Testing\\BSTest\\work\\Test0022N2.mpr" 2 5 22 ]\
]\
[ list \
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  0  1  ]\
]\
[ list \
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 0  PASS  [list [list 0 5 ] [list 1 5 ] ] "" "" [list  2 [ list  ]  0 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 1  PASS  [list [list 0 7 ] [list 1 7 ] ] "" "" [list  12 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "1" "CRUISER" "0" "SOUTH" "20" [list STRUCT "10" "10"] [list ARRAY "0" "0" "0" "0"]] [list STRUCT "2" "1" "REPAIR_BOAT" "0" "SOUTH" "25" [list STRUCT "12" "10"] [list ARRAY "0" "0" "0" "10"]] [list STRUCT "3" "1" "CRUISER" "0" "SOUTH" "5" [list STRUCT "15" "10"] [list ARRAY "0" "0" "0" "0"]]]  ]  ]  1 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 2  PASS  [list [list 0 9 ] [list 1 9 ] ] "" "" [list  16 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "2" "REPAIR_BOAT" "0" "NORTH_WEST" [list STRUCT "25" "25"]]]  ]  ]  2 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 3  PASS  [list [list 0 11 ] [list 1 11 ] ] "" "" [list  13 [ list [ list "" "0"  ]  ]  3 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 4  PASS  [list [list 0 13 ] [list 1 13 ] ] "" "" [list  10 [ list [ list "" "1"  ] [ list "" "1"  ] [ list "" "LEFT"  ]  ]  4 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 5  PASS  [list [list 0 15 ] [list 1 15 ] ] "" "" [list  4 [ list [ list "" "2"  ] [ list "" "1"  ] [ list "" "REPAIR"  ] [ list "" [list ARRAY [list STRUCT "15" "10"] [list STRUCT "10" "10"]]  ]  ]  5 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 6  PASS  [list [list 0 17 ] [list 1 17 ] ] "" "" [list  10 [ list [ list "" "3"  ] [ list "" "1"  ] [ list "" "LEFT"  ]  ]  6 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 7  PASS  [list [list 0 19 ] [list 1 19 ] ] "" "" [list  1 [ list [ list "" "0"  ] [ list "" "1"  ]  ]  7 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 8  PASS  [list [list 0 21 ] [list 1 21 ] ] "" "" [list  15 [ list [ list "" "1"  ]  ]  8 [ list REL "0" "0" ] ] ]\
] 1 0

State 0 0 0 0 1 0 [list \
[list 0  1 ]\
]

State 1 0 0 0 1 0 [list \
[list 1  2 ]\
]

State 2 0 0 0 1 0 [list \
[list 2  3 ]\
]

State 3 0 0 0 1 0 [list \
[list 3  4 ]\
]

State 4 0 0 1 0 0 [list \
[list 4  5 ]\
]

State 5 0 0 1 0 0 [list \
[list 5  6 ]\
]

State 6 0 0 1 0 0 [list \
[list 6  7 ]\
]

State 7 0 0 1 0 0 [list \
[list 7  8 ]\
]

State 8 0 0 0 1 0 [list \
[list 8  -1 ]\
]

FinishTestCase "Test0022N2"


StartTestCase "Test0032N2" 1 [ list ]\
[ list ]\
[ list ]  0 0 \
[ list \
"InitDataComplete()"\
"ShipsCurrentState(1,(: (. 1, 1, CRUISER, 8, EAST, 70, (. 40, 25 .), (: 15, 15, 3, 0 :) .) :))"\
"VisibleShip(1,(: (. 1, 2, REPAIR_BOAT, 0, NORTH_WEST, (. 15, 15 .) .) :))"\
"StartSimulation(0)"\
"OrderSpeedRequest(1,1,-1.0000)"\
"FinishSimulation(0,1)"\
"StrategyVictory(1)"\
] [ list  ]\
[ list  [list "C:\\Work\\Testing\\BSTest\\mpr\\Test0032.mpr" 1 5 18 ]\
[list "C:\\Work\\Testing\\BSTest\\work\\Test0032N2.mpr" 2 5 18 ]\
]\
[ list \
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  0  1  ]\
 [list  1  0  ]\
 [list  1  0  ]\
 [list  0  1  ]\
]\
[ list \
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 0  PASS  [list [list 0 5 ] [list 1 5 ] ] "" "" [list  2 [ list  ]  0 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 1  PASS  [list [list 0 7 ] [list 1 7 ] ] "" "" [list  12 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "1" "CRUISER" "8" "EAST" "70" [list STRUCT "40" "25"] [list ARRAY "15" "15" "3" "0"]]]  ]  ]  1 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 2  PASS  [list [list 0 9 ] [list 1 9 ] ] "" "" [list  16 [ list [ list "" "1"  ] [ list "" [list ARRAY [list STRUCT "1" "2" "REPAIR_BOAT" "0" "NORTH_WEST" [list STRUCT "15" "15"]]]  ]  ]  2 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 3  PASS  [list [list 0 11 ] [list 1 11 ] ] "" "" [list  13 [ list [ list "" "0"  ]  ]  3 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 4  PASS  [list [list 0 13 ] [list 1 13 ] ] "" "" [list  7 [ list [ list "" "1"  ] [ list "" "1"  ] [ list "" "-1.0000"  ]  ]  4 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 5  PASS  [list [list 0 15 ] [list 1 15 ] ] "" "" [list  1 [ list [ list "" "0"  ] [ list "" "1"  ]  ]  5 [ list REL "0" "0" ] ] ]\
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 6  PASS  [list [list 0 17 ] [list 1 17 ] ] "" "" [list  15 [ list [ list "" "1"  ]  ]  6 [ list REL "0" "0" ] ] ]\
] 1 0

State 0 0 0 0 1 0 [list \
[list 0  1 ]\
]

State 1 0 0 0 1 0 [list \
[list 1  2 ]\
]

State 2 0 0 0 1 0 [list \
[list 2  3 ]\
]

State 3 0 0 0 1 0 [list \
[list 3  4 ]\
]

State 4 0 0 1 0 0 [list \
[list 4  5 ]\
]

State 5 0 0 1 0 0 [list \
[list 5  6 ]\
]

State 6 0 0 0 1 0 [list \
[list 6  -1 ]\
]

FinishTestCase "Test0032N2"

EndTestSuite "battleshipstest"
