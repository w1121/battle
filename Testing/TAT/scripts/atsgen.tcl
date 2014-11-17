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
# SiC Abstract Test Generator configuration script
#
#########################################################################

namespace eval ::atsgen {

	variable title "SiC Abstract Test Generator"

	# Tool options
	variable OPT_NAME	0
	variable OPT_TITLE	1
	variable OPT_TYPE	2
	variable OPT_PREFIX	3
	variable OPT_AFFIL	4

	variable defaults [list \
		[list	"i"		"set input directory"						"directory"	"-i "		"input"] \
		[list	"incl"		"set included files"						"scroll"	"-incl "	"input"] \
		[list	"cname"		"set configuration name"					"combo"		"-cname "	"input"] \
		[list	"tt"		"check variables via target types too"				"checkbox"	"-tt"		"input"] \
		[list	"sfg"		"split events in FlowGen files"					"checkbox"	"-sfg"		"input"] \
		[list	"asn1"		"enable ASN.1 parsing in MSC"					"checkbox"	"-asn1"		"input"] \
		[list	"conf"		"set configuration tag specification"				"entry"		"-conf "	"output"] \
		[list	"envself"	"save events from env to env" 					"checkbox"	"-envself"	"output"] \
		[list	"list"		"specify file for list of generated files"			"filesave"	"-list "	"output"] \
		[list	"prefix"	"prefix for test case names in ATS"				"entry"		"-prefix "	"output"] \
		[list	"optexc"	"optimize 'exc' inline expressions"				"checkbox"	"-optexc"	"output"] \
		[list	"state"		"generate ATS in TTS representation"				"checkbox"	"-state"	"output"] \
		[list	"flat"		"generate direct ATS"						"checkbox"	"-flat"		"output"] \
		[list	"cm"		"use max possible number of paths for testcase"			"checkbox"	"-cm"		"output"] \
		[list	"sro"		"output only send/receive/labeled events"			"checkbox"	"-sro"		"output"] \
		[list	"rmvt"		"exclude timers without labels from output"			"checkbox"	"-rmvt"		"output"] \
		[list	"rmvc"		"exclude conditions without labels from output" 		"checkbox"	"-rmvc"		"output"] \
		[list	"smt"		"single message table for test cases"				"checkbox"	"-smt"		"output"] \
		[list	"tf"		"consider time while generating test paths"			"checkbox"	"-tf"		"time"] \
		[list	"ct"		"calculate time"						"checkbox"	"-ct"		"time"] \
		[list	"nosync"	"increase intervals instead of labels"				"checkbox"	"-nosync"	"time"] \
		[list	"interv"	"compare time intervals (default - min time)"			"checkbox"	"-interv"	"time"] \
		[list	"ndefst"	"start time is {0..INF}, otherwise {0..0}"			"checkbox"	"-ndefst"	"time"] \
		[list	"duration"	"use duration field for time calculation"			"checkbox"	"-duration"	"time"] \
		[list	"floatt"	"support floating point time in output"				"checkbox"	"-floatt"	"time"] \
		[list	"ul"		"unroll loops with const number of iterations"			"checkbox"	"-ul"		"loop"] \
		[list	"mf"		"association of in/out events into send/receive signals"	"checkbox"	"-mf"		"synchro"] \
		[list	"is"		"filter react on consuming messages from environment"		"checkbox"	"-is"		"synchro"] \
		[list	"ie"		"filter react on consuming messages from model"			"checkbox"	"-ie"		"synchro"] \
		[list	"ec"		"make all condition events global"				"checkbox"	"-ec"		"synchro"] \
		[list	"sa"		"restrict max active branches"					"integer"	"-sa "		"trace"] \
		[list	"sp"		"restrict max passive branches"					"integer"	"-sp "		"trace"] \
		[list	"sf"		"active events in start"					"checkbox"	"-sf"		"trace"] \
		[list	"rf"		"passive events in start"					"checkbox"	"-rf"		"trace"] \
		[list	"nia"		"do not interleave actions"					"checkbox"	"-nia"		"trace"] \
		[list	"nic"		"do not interleave conditions"					"checkbox"	"-nic"		"trace"] \
		[list	"g"		"generate debug info for MSC"					"checkbox"	"-g"		"trace"] \
	]

	variable options

	# Tool inputs/outputs
	variable IO_NAME	0
	variable IO_TYPE	1
	variable IO_DIR		2
	variable IO_PREFIX	3

	variable ios [list \
		[list	"MPR_FILE_LIST"	"FILE"	"in"	"-m @"] \
		[list	"TCL_FILE"	"FILE"	"out"	"-o "] \
	]

	# Miscellaneous data
	variable data

	# Affiliation to a notebook pad
	variable AFFIL_NAME	0
	variable AFFIL_TITLE	1

	variable affils [list \
		[list	"input"		"Input file options"] \
		[list	"output"	"Output file options"] \
		[list	"time"		"Time processing"] \
		[list	"loop"		"Loop processing"] \
		[list	"synchro"	"Synchronization options"] \
		[list	"trace"		"Trace generation options"] \
	]
}

#########################################################################
# Procedure:	Script_configTool
#
# Description:	Configures and packs tool configuration options
#
# Returns:	Codes: ok, error with a string
#
# Notes:	None
#   
#########################################################################

proc Script_configTool { frame toolconfig } {

	# filling options array and extracting options from TOOLCONFIG
	foreach option $::atsgen::defaults {
		set ::atsgen::options([lindex $option $::atsgen::OPT_NAME]) {}
	}
	foreach attrnode [::XML::findElements $toolconfig ATTRIBUTE {}] {
		set ::atsgen::options([::dom::element getAttribute $attrnode comment]) [::dom::element getAttribute $attrnode value]
	}
	
	# packing notebook
	set nbook [NoteBook $frame.nbook]
	foreach affil $::atsgen::affils {
		set name	[lindex $affil $::atsgen::AFFIL_NAME]
		set title	[lindex $affil $::atsgen::AFFIL_TITLE]

		set pane [$nbook insert end $name -text $title]
		set titleframe [TitleFrame $pane.title -text $title]
		pack $titleframe -anchor n -side left -fill x -expand yes
		set wchild($name) [$titleframe getframe]
	}
	
	# main loop
	foreach option $::atsgen::defaults {
		set name	[lindex $option $::atsgen::OPT_NAME]
		set title	[lindex $option $::atsgen::OPT_TITLE]
		set type	[lindex $option $::atsgen::OPT_TYPE]
		set prefix	[lindex $option	$::atsgen::OPT_PREFIX]
		set affil	[lindex $option $::atsgen::OPT_AFFIL]

		set picked $wchild($affil)

		set titleframe [TitleFrame $picked.title_$name -text $title]
		set child [$titleframe getframe]
		pack $titleframe -expand yes -fill x -anchor w -side top
		
		# choosing and packing GUI element
		switch $type {
			scroll {
				set elem [ScrolledWindow $child.scroll -auto both]
				set ::atsgen::data(scroll.$name) [text $elem.text -bg white -wrap word -height 7 -width 40]
				$elem setwidget $::atsgen::data(scroll.$name)
				$::atsgen::data(scroll.$name) insert end $::atsgen::options($name)
				pack $elem -anchor w -side left -fill x -expand yes
			}
			fileopen {
				pack [entry $child.entry -textvariable ::atsgen::options($name)] -side left -fill x -expand yes
				pack [button $child.button -text "Browse..." -command "::Commands::browseOpen ::atsgen::options($name)"] -side right
			}
			filesave {
				pack [entry $child.entry -textvariable ::atsgen::options($name)] -side left -fill x -expand yes
				pack [button $child.button -text "Browse..." -command "::Commands::browseSave ::atsgen::options($name)"] -side right
			}
			directory {
				pack [entry $child.entry -textvariable ::atsgen::options($name)] -side left -fill x -expand yes
				pack [button $child.button -text "Browse..." -command "::Commands::browseDirectory ::atsgen::options($name)"] -side right
			}
			integer -
			entry {
				pack [entry $child.entry -textvariable ::atsgen::options($name)] -side left -fill x -expand yes
			}
			combo {
				pack [ComboBox $child.combo -textvariable ::atsgen::options($name) -values [::XML::getConfigList] -editable 0 -helptext $title] -side left -fill x -expand yes
			}
			checkbox
			{
				pack [checkbutton $child.checkbox -text $title -variable ::atsgen::options($name) -onvalue "?" -offvalue "!"] -side left
			}
		}
	}
	pack $nbook -fill x -expand yes
	$nbook raise [lindex [lindex $::atsgen::affils 0] $::atsgen::AFFIL_NAME]
	return -code ok
}

#########################################################################
# Procedure:	Script_getToolConfig
#
# Description:	Saves tool configuration options
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
	foreach option $::atsgen::defaults {
		set name	[lindex $option $::atsgen::OPT_NAME]
		set title	[lindex $option $::atsgen::OPT_TITLE]
		set type	[lindex $option $::atsgen::OPT_TYPE]
		set prefix	[lindex $option	$::atsgen::OPT_PREFIX]
		set affil	[lindex $option $::atsgen::OPT_AFFIL]
		switch $type {
			scroll {
				set optval [string trimright [$::atsgen::data(scroll.$name) get 1.0 end]]
			}
			default {				
				set optval $::atsgen::options($name)
			}
		}
		if {$optval==""} continue
		::XML::AddElement $toolconfig ATTRIBUTE [list comment $name value $optval]
	}
	return -code ok
}

#########################################################################
# Procedure:	Script_getToolSignals
#
# Description:	Fetches list of tool inputs and outputs for tool declaration
#
# Returns:	List of signal declarations
#
# Notes:	None
#
#########################################################################
proc Script_getToolSignals {} {
	return $::atsgen::ios
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
	foreach toolsig $::atsgen::ios {
		set name [lindex $toolsig $::atsgen::IO_NAME]
		set type [lindex $toolsig $::atsgen::IO_TYPE]
		set dir [lindex $toolsig $::atsgen::IO_DIR]
		set prefix [lindex $toolsig $::atsgen::IO_PREFIX]
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
		foreach option $::atsgen::defaults {
			set name	[lindex $option $::atsgen::OPT_NAME]
			set title	[lindex $option $::atsgen::OPT_TITLE]
			set type	[lindex $option $::atsgen::OPT_TYPE]
			set prefix	[lindex $option	$::atsgen::OPT_PREFIX]
			set affil	[lindex $option $::atsgen::OPT_AFFIL]
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
			return -code error "$::atsgen::title: unknown option \"$optName\" specified"
		}
	}

	# fetching configuration filename
	set configFile [::XML::getConfigFile]
	if {![string length $configFile]} {return -code error "$::atsgen:::title: Cannot fetch the configuration file name"}
	append cmdLine " -config $configFile"

	#tk_messageBox -title "ATSGEN Command Line" -message $cmdLine

	# launching on the command prompt
	if {[catch {eval exec "$cmdLine"} err]} {
		return -code error "$::atsgen::title: \"$err\""
	}  else {
		return -code ok $err
	}
}
