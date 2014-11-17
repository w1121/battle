###############################################################################
#
# COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
#
# Description: Abstract Test Suite 'TAT_Test'
#
###############################################################################
# command line: 'D:\Projects\TAT\bin\atsgen.exe -m mpr/test.mpr -config config.xml -o work/TAT_Test.tcl -mf -tt -state -nia -asn1 '

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
 "IUT" "model" [ list  ] [ list ] [ list ] [ list ] "" 0 100\
 "Test" "env" [ list  ] [ list ] [ list ] [ list ] "" 0 100\
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
"A"  "in"  [list ] [ list "CharString" "CharString" ] [ list 0 0 ] [ list Param1 Param2 ] "C" 0 100\
"B"  "out"  [list ] [ list "CharString" "CharString" ] [ list 0 0 ] [ list Param1 Param2 ] "C" 0 100\
]
set TestCaseNames [list "test"]
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
		"CharString" "char *" "" -1 [list ] [ list ] "" "" [list]\
]
set IncludeFiles [ list  ]
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

StartTestSuite "TAT_Test"


StartTestCase "test" 1 [ list ]\
[ list ]\
[ list ]  0 0 \
[ list \
"A(\"Hello\",\"World\")"\
"B(\"World\",\"Hello\")"\
] [ list  ]\
[ list  [list "D:\\Projects\\TAT\\test\\test0004\\mpr\\test.mpr" 1 5 8 ]\
]\
[ list \
 [list  1  0  ]\
 [list  0  1  ]\
]\
[ list \
[ list "send" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 0  PASS  [list [list 0 5 ] ] "" "" [list  0 [ list [ list "" "\"Hello\""  ] [ list "" "\"World\""  ]  ]  0 [ list REL "0" "0" ] ] ]\
[ list "recv" [list [ list REL "0" "INF" ] [ list REL "0" "INF" ] ] [list ] [list ] "" 1  PASS  [list [list 0 7 ] ] "" "" [list  1 [ list [ list "" "\"World\""  ] [ list "" "\"Hello\""  ]  ]  1 [ list REL "0" "0" ] ] ]\
] 1 0

State 0 0 0 0 1 0 [list \
[list 0  1 ]\
]

State 1 0 0 1 0 0 [list \
[list 1  -1 ]\
]

FinishTestCase "test"

EndTestSuite "TAT_Test"
