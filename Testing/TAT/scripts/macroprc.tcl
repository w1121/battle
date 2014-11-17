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
# "Macro Generated Code Executable" configuration script
#
#########################################################################

namespace eval ::macroprc {
	variable title "SiC Macro Processor"

	# Tool options
	variable OPT_NAME	0
	variable OPT_TITLE	1
	variable OPT_TYPE	2
	variable OPT_PREFIX	3
	variable OPT_AFFIL	4

	variable defaults [list \
		[list	"cname"		"XML configuration name"				"combo"		"-cname "	"input"] \
		[list	"ignorenf"	"ignore references to non-existing files"		"checkbox"	"-ignorenf"	"input"] \
		[list	"incl"		"set dynamic library files"				"scroll"	"-incl "	"input"] \
		[list	"count"		"count number of macro permutations"			"checkbox"	"-count"	"value"] \
		[list	"debug"		"generate set and print it"				"checkbox"	"-debug"	"value"] \
		[list	"lv"		"load values of variables from file"			"fileopen"	"-lv "		"value"] \
		[list	"n"		"max number of tests to generate"			"integer"	"-n "		"value"] \
		[list	"rnd"		"generate random permutations of macro"			"checkbox"	"-rnd"		"value"] \
		[list	"s"		"generate start test number"				"integer"	"-s "		"value"] \
		[list	"seed"		"set seed number for random generator"			"integer"	"-seed "	"value"] \
		[list	"c"		"ignore conditional field in reference_Events"		"checkbox"	"-c"		"output"] \
		[list	"il"		"inline loops with counter equal to 1 into plain code"	"checkbox"	"-il"		"output"] \
		[list	"ul"		"unroll loops with constant number of iterations"	"checkbox"	"-ul"		"output"] \
		[list	"f"		"force output synchronization condition"		"checkbox"	"-f"		"output"] \
		[list	"fr"		"do not substitute references started with '$'"		"checkbox"	"-fr"		"output"] \
		[list	"glob"		"macros are global for included MSC"			"checkbox"	"-glob"		"output"] \
		[list	"iar"		"substitute all references"				"checkbox"	"-iar"		"output"] \
		[list	"l"		"save loops with zero iterator"				"checkbox"	"-l"		"output"] \
		[list	"notime"	"exclude time specifications in generated files"	"checkbox"	"-notime"	"output"] \
		[list	"nrf"		"do not expand any references at all"			"checkbox"	"-nrf"		"output"] \
		[list	"plaindir"	"do not append directory to generated file names"	"checkbox"	"-plaindir"	"output"] \
		[list	"rf"		"frame substitued references in conditions"		"checkbox"	"-rf"		"output"] \
		[list	"use"		"set active macro names"				"entry"		"-use "		"output"] \
		[list	"excl"		"set excluded macro names"				"entry"		"-excl "	"output"] \
	]
	
	variable options

	# Tool inputs/outputs
	variable IO_NAME	0
	variable IO_TYPE	1
	variable IO_DIR		2
	variable IO_PREFIX	3

	variable ios [list \
		[list	"INPUT_FILE_LIST"	"FILE"	"in"	"-m @"] \
		[list	"OUTPUT_FILE_LIST"	"FILE"	"out"	"-list "] \
	]

	# Miscellaneous data
	variable data

	# Affiliation to a notebook pad
	variable AFFIL_NAME	0
	variable AFFIL_TITLE	1
			
	variable affils [list \
		[list	"input"		"Input options"] \
		[list	"value"		"Value generation options"] \
		[list	"output"	"Output options"] \
	]
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
#########################################################################

proc Script_configTool { frame toolconfig } {

	# filling options array and extracting options from TOOLCONFIG
	foreach option $::macroprc::defaults {
		set ::macroprc::options([lindex $option $::macroprc::OPT_NAME]) {}
	}
	foreach attrnode [::XML::findElements $toolconfig ATTRIBUTE {}] {
		set ::macroprc::options([::dom::element getAttribute $attrnode comment]) [::dom::element getAttribute $attrnode value]
	}
	
	# packing notebook
	set nbook [NoteBook $frame.nbook]
	foreach affil $::macroprc::affils {
		set name	[lindex $affil $::macroprc::AFFIL_NAME]
		set title	[lindex $affil $::macroprc::AFFIL_TITLE]

		set pane [$nbook insert end $name -text $title]
		set titleframe [TitleFrame $pane.title -text $title]
		pack $titleframe -anchor n -side left -fill x -expand yes
		set wchild($name) [$titleframe getframe]
	}
	
	# main loop
	foreach option $::macroprc::defaults {
		set name	[lindex $option $::macroprc::OPT_NAME]
		set title	[lindex $option $::macroprc::OPT_TITLE]
		set type	[lindex $option $::macroprc::OPT_TYPE]
		set prefix	[lindex $option	$::macroprc::OPT_PREFIX]
		set affil	[lindex $option $::macroprc::OPT_AFFIL]

		set picked $wchild($affil)

		set titleframe [TitleFrame $picked.title_$name -text $title]
		set child [$titleframe getframe]
		pack $titleframe -expand yes -fill x -anchor w -side top
		
		# choosing and packing GUI element
		switch $type {
			scroll {
				set elem [ScrolledWindow $child.scroll -auto both]
				set ::macroprc::data(scroll.$name) [text $elem.text -bg white -wrap word -height 7 -width 40]
				$elem setwidget $::macroprc::data(scroll.$name)
				$::macroprc::data(scroll.$name) insert end $::macroprc::options($name)
				pack $elem -anchor w -side left -fill x -expand yes
			}
			fileopen {
				pack [entry $child.entry -textvariable ::macroprc::options($name)] -side left -fill x -expand yes
				pack [button $child.button -text "Browse..." -command "::Commands::browseOpen ::macroprc::options($name)"] -side right
			}
			filesave {
				pack [entry $child.entry -textvariable ::macroprc::options($name)] -side left -fill x -expand yes
				pack [button $child.button -text "Browse..." -command "::Commands::browseSave ::macroprc::options($name)"] -side right
			}
			directory {
				pack [entry $child.entry -textvariable ::macroprc::options($name)] -side left -fill x -expand yes
				pack [button $child.button -text "Browse..." -command "::Commands::browseDirectory ::macroprc::options($name)"] -side right
			}
			integer -
			entry {
				pack [entry $child.entry -textvariable ::macroprc::options($name)] -side left -fill x -expand yes
			}
			combo {
				pack [ComboBox $child.combo -textvariable ::macroprc::options($name) -values [::XML::getConfigList] -editable 0 -helptext $title] -side left -fill x -expand yes
			}
			checkbox
			{
				pack [checkbutton $child.checkbox -text $title -variable ::macroprc::options($name) -onvalue "?" -offvalue "!"] -side left
			}
		}
	}
	pack $nbook -fill x -expand yes
	$nbook raise [lindex [lindex $::macroprc::affils 0] $::macroprc::AFFIL_NAME]
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
#########################################################################
proc Script_getToolConfig {toolconfig} {

	# Deleting all TOOLCONFIG subnodes
	foreach attrnode [::XML::findElements $toolconfig ATTRIBUTE {}] {
		::XML::DeleteElement $attrnode
	}

	# main loop
	foreach option $::macroprc::defaults {
		set name	[lindex $option $::macroprc::OPT_NAME]
		set title	[lindex $option $::macroprc::OPT_TITLE]
		set type	[lindex $option $::macroprc::OPT_TYPE]
		set prefix	[lindex $option	$::macroprc::OPT_PREFIX]
		set affil	[lindex $option $::macroprc::OPT_AFFIL]
		switch $type {
			scroll {
				set optval [string trimright [$::macroprc::data(scroll.$name) get 1.0 end]]
			}
			default {				
				set optval $::macroprc::options($name)
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
	return $::macroprc::ios
}

#########################################################################
# Procedure : run_Tool
#
# Description: Executes tool
#
# Returns: codes: ok, error with a string
#
#########################################################################
proc Script_runTool { path io toolconfig } {
	global env

	set cmdLine [subst $path]

	# processing inputs and outputs
	foreach toolsig $::macroprc::ios {
		set name [lindex $toolsig $::macroprc::IO_NAME]
		set type [lindex $toolsig $::macroprc::IO_TYPE]
		set dir [lindex $toolsig $::macroprc::IO_DIR]
		set prefix [lindex $toolsig $::macroprc::IO_PREFIX]
		foreach sig $io {
			set sig [subst $sig]
			if {[string match $name [lindex $sig 0]] && [string length [lindex $sig 1]]>0} {
				set workDir [::XML::getWorkDir]
				if {[string length $workDir]==0} {set workDir "."}
				append cmdLine " ${prefix}[file join $workDir [lindex $sig 1]]"
				# Note: output directory for macroprc is determined by output list file path, so add it
				if {[string equal $name OUTPUT_FILE_LIST]} {
					append cmdLine " -o [file join $workDir [file dirname [lindex $sig 1]]]"
				}
			}
		}
	}

	# processing options
	foreach attrnode [::XML::findElements $toolconfig ATTRIBUTE {}] {
		set optName [::dom::element getAttribute $attrnode "comment"]
		set optVal [::dom::element getAttribute $attrnode "value"]
		set found 0	
		foreach option $::macroprc::defaults {
			set name	[lindex $option $::macroprc::OPT_NAME]
			set title	[lindex $option $::macroprc::OPT_TITLE]
			set type	[lindex $option $::macroprc::OPT_TYPE]
			set prefix	[lindex $option	$::macroprc::OPT_PREFIX]
			set affil	[lindex $option $::macroprc::OPT_AFFIL]
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
			return -code error "$::macroprc::title: unknown option \"$optName\" specified"
		}
	}

	# fetching configuration filename
	set configFile [::XML::getConfigFile]
	if {![string length $configFile]} {return -code error "$::macroprc:::title: Cannot fetch the configuration file name"}
	append cmdLine " -config $configFile"

	#tk_messageBox -title "MACROPRC Command Line" -message $cmdLine

	# launching on the command prompt
	if {[catch {eval exec "$cmdLine"} err]} {
		return -code error "$::macroprc::title: \"$err\""
	}  else {
		return -code ok $err
	}
}
