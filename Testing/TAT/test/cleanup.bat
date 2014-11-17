@echo off
FOR /D %%d IN (*) do (
	del %%d\test\TAT_Test*.* 2>NUL
	del %%d\test\*.log 2>NUL
	del %%d\work\*.tcl 2>NUL
	del %%d\*.txt 2>NUL
	del %%d\*.mpr 2>NUL
	del %%d\*.lst 2>NUL
	del %%d\*.exe 2>NUL
	del %%d\*.obj 2>NUL
)