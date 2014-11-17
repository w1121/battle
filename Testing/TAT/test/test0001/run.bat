@echo off
set testname=TAT_Test

..\..\bin\atsgen -m mpr/test.mpr -config config.xml -o work/%testname%.tcl -mf -tt -state -nia -asn1

tclsh83 ../../cgtc/cgtc.tcl -ats work/%testname%.tcl -log -inst -break -time -o test

set srcfiles=test\%testname%.c test\%testname%data.c test\wrapper.c ..\..\cgtc\mprlog.c ..\..\cgtc\txtlog.c ..\..\cgtc\testkern.c
cl /I "..\..\cgtc" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "TAT_TIME_SUPPORT" /D "TAT_INSTANCE_SUPPORT" /D "_MBCS" %srcfiles% /link /OUT:%testname%.exe

%testname%

del *.obj