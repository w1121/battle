@echo off
FOR /D %%d IN (*) do (
	cd %%d
	tclsh run.tcl
	cd ..
	echo %%d
)