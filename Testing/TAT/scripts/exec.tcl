#########################################################################
#
# COPYRIGHT 2001-2002 MOTOROLA, ALL RIGHTS RESERVED
#
# This code is the property of Motorola St.Petersburg Software Development
# and is Motorola Confidential Proprietary Information.
#
# The copyright notice above does not evidence any actual or intended
# publication of such source code.
#
# Description: Silver Cartridge
# Microsoft C/C++ Compiler configuration script
#
#########################################################################

namespace eval ::exec {
		variable title "Run Tool"

		# Tool signals
		variable ios [list [list "EXEC_FILE" "FILE" "in" ""] [list "VERDICT" "FILE" "out"]]
}

#########################################################################
# Procedure :  Script_configTool
#
# Description: Configures and packs tool configuration options
#
# Returns:	   Codes: ok, error with a string
#
# Notes:	   None
#   
########################################################################

proc Script_configTool { frame toolconfig } {}

#########################################################################
# Procedure :  Script_getToolConfig
#
# Description: Saves tool configuration options
#
# Returns:	   Codes: error, ok; an array of configured options
#
# Notes:	   None
#   
########################################################################
proc Script_getToolConfig {toolconfig} {}

############################################################################
# Procedure :  Script_getToolSignals
#
# Description: Fetches list of tool inputs and outputs for tool declaration
#
# Returns:	   List of signal declarations
#
# Notes:	   None
#
############################################################################
proc Script_getToolSignals {} {
	return $::exec::ios
}

#########################################################################
# Procedure : Script_runTool
#
# Description: Executes tool on the command prompt
#
# Returns: codes: ok, error with a string
#
# Notes:	nspc	- current namespace
#			path	- tool execution path
#			inputs	- tool inputs		 
#			ouputs	- tool outputs
#			opts	- tool command line options
#			dmode	- debug mode (1 or 0)
#			dinfo	- debug info returned
#
#########################################################################
proc Script_runTool { path io toolconfig } {
	global env
	set filename ""
	set verdictDir ""

	foreach chan $io {
		switch [lindex $chan 0] {
			"EXEC_FILE" {set filename [subst [lindex $chan 1]]}
			"VERDICT" {set verdictDir [file dirname [subst [lindex $chan 1]]]}
		}
	}
	
	if {$filename==""} return
	set workDir [::XML::getWorkDir]
	set cmdLine [file join $workDir $filename]

	if {$verdictDir!=""} {
		cd [file join $workDir $verdictDir]
	}

	#tk_messageBox -title "EXEC" -message "${cmdLine}\n${verdictDir}"

	catch {eval exec "$cmdLine"} res
	cd $workDir
	return -code ok $res
}