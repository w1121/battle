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

namespace eval ::cl {
	variable title "Microsoft C/C++ Compiler"

	# Tool options
	variable OPT_NAME	0
	variable OPT_TITLE	1
	variable OPT_TYPE	2
	variable OPT_PREFIX	3
		
	variable defaults [list \
		[list	"misc"	"Compiler options"	"scroll"	""] \
		[list	"link"	"Linker options"	"scroll"	"/link "] \
	]

	variable options

	# Tool inputs/outputs
	variable IO_NAME	0
	variable IO_TYPE	1
	variable IO_DIR		2
	variable IO_PREFIX	3

	variable ios [list \
		[list	"ANSIC_FILE"	"FILE"	"in"	""] \
		[list	"EXE_FILE"	"FILE"	"out"	"/Fe"] \
	]

	# Miscellaneous data
	variable data
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

proc Script_configTool { frame toolconfig } {

	# filling options array and extracting options from TOOLCONFIG
	foreach option $::cl::defaults {
		set ::cl::options([lindex $option $::cl::OPT_NAME]) {}
	}
	foreach attrnode [::XML::findElements $toolconfig ATTRIBUTE {}] {
		set ::cl::options([::dom::element getAttribute $attrnode comment]) [::dom::element getAttribute $attrnode value]
	}

	# main loop
	foreach option $::cl::defaults {
		set name	[lindex $option $::cl::OPT_NAME]
		set title	[lindex $option $::cl::OPT_TITLE]
		set type	[lindex $option $::cl::OPT_TYPE]
		set prefix	[lindex $option	$::cl::OPT_PREFIX]

		set titleframe [TitleFrame $frame.title_$name -text $title]
		set child [$titleframe getframe]
		pack $titleframe -expand yes -fill x -anchor w -side top

		# choosing and packing GUI element
		switch $type {
			scroll {
				set elem [ScrolledWindow $child.scroll -auto both]
				set ::cl::data(scroll.$name) [text $elem.text -bg white -wrap word -height 7 -width 40]
				$elem setwidget $::cl::data(scroll.$name)
				$::cl::data(scroll.$name) insert end $::cl::options($name)
				pack $elem -anchor w -side left -fill x -expand yes
			}
			fileopen {
				pack [entry $child.entry -textvariable ::cl::options($name)] -side left -fill x -expand yes
				pack [button $child.button -text "Browse..." -command "::Commands::browseOpen ::cl::options($name)"] -side right
			}
			filesave {
				pack [entry $child.entry -textvariable ::cl::options($name)] -side left -fill x -expand yes
				pack [button $child.button -text "Browse..." -command "::Commands::browseSave ::cl::options($name)"] -side right
			}
			directory {
				pack [entry $child.entry -textvariable ::cl::options($name)] -side left -fill x -expand yes
				pack [button $child.button -text "Browse..." -command "::Commands::browseDirectory ::cl::options($name)"] -side right
			}
			integer -
			entry {
				pack [entry $child.entry -textvariable ::cl::options($name)] -side left -fill x -expand yes
			}
			combo {
				pack [ComboBox $child.combo -textvariable ::cl::options($name) -values [::XML::getConfigList] -editable 0 -helptext $title] -side left -fill x -expand yes
			}
			checkbox
			{
				pack [checkbutton $child.checkbox -text $title -variable ::cl::options($name) -onvalue "?" -offvalue "!"] -side left
			}
		}
	}
	return -code ok
}

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
proc Script_getToolConfig {toolconfig} {

	# Deleting all TOOLCONFIG subnodes
	foreach attrnode [::XML::findElements $toolconfig ATTRIBUTE {}] {
		::XML::DeleteElement $attrnode
	}

	# main loop
	foreach option $::cl::defaults {
		set name	[lindex $option $::cl::OPT_NAME]
		set title	[lindex $option $::cl::OPT_TITLE]
		set type	[lindex $option $::cl::OPT_TYPE]
		set prefix	[lindex $option	$::cl::OPT_PREFIX]
		switch $type {
			scroll {
				set optval [string trimright [$::cl::data(scroll.$name) get 1.0 end]]
			}
			default {				
				set optval $::cl::options($name)
			}
		}
		if {$optval==""} continue
		::XML::AddElement $toolconfig ATTRIBUTE [list comment $name value $optval]
	}
	return -code ok
}


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
	return $::cl::ios
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

	set cmdLine [subst $path]

	# processing inputs and outputs
	foreach toolsig $::cl::ios {
		set name [lindex $toolsig $::cl::IO_NAME]
		set type [lindex $toolsig $::cl::IO_TYPE]
		set dir [lindex $toolsig $::cl::IO_DIR]
		set prefix [lindex $toolsig $::cl::IO_PREFIX]
		foreach sig $io {
			set sig [subst $sig]
			if {[string match $name [lindex $sig 0]] && [string length [lindex $sig 1]]>0} {
				set workDir [::XML::getWorkDir]
				if {[string length $workDir]==0} {set workDir "."}
				append cmdLine " ${prefix}[file join $workDir [lindex $sig 1]]"
			}
		}
	}

	# processing options
	foreach attrnode [::XML::findElements $toolconfig ATTRIBUTE {}] {
		set optName [::dom::element getAttribute $attrnode "comment"]
		set optVal [::dom::element getAttribute $attrnode "value"]
		set found 0	
		foreach option $::cl::defaults {
			set name	[lindex $option $::cl::OPT_NAME]
			set title	[lindex $option $::cl::OPT_TITLE]
			set type	[lindex $option $::cl::OPT_TYPE]
			set prefix	[lindex $option	$::cl::OPT_PREFIX]
			if {[string equal $optName $name]} {
				switch $optVal {
					"?" {append cmdLine " $prefix"}
					"!" {}
					default {append cmdLine " ${prefix}$optVal"}
				}
				set found 1
				break
			}
		}
		if {!$found} {
			return -code error "$::cl::title: unknown option \"$optName\" specified"
		}
	}

	tk_messageBox -title "CL Command Line" -message $cmdLine

	# launching on the command prompt
	if {[catch {eval exec "$cmdLine"} err]} {
		foreach f [glob -nocomplain *.obj] {file delete $f}
		return -code error "$::cl::title: \"$err\""
	}  else {
		foreach f [glob -nocomplain *.obj] {file delete $f}
		return -code ok $err
	}
}