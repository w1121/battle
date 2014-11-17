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
# "C Code Generation Template" configuration script
#
#########################################################################

namespace eval ::cgtc {
	variable title "SiC C code generation template"

	# Tool options
	variable OPT_NAME	0
	variable OPT_TITLE	1
	variable OPT_TYPE	2
	variable OPT_PREFIX	3
	variable OPT_AFFIL	4
		
	variable defaults [list \
		[list	"break"		"Generate break after first error detection"	"checkbox"	"-break"	"common"] \
		[list	"comm"		"Output signal's parameters comments"		"checkbox"	"-spcomm"	"common"] \
		[list	"inst"		"Generate instance check functionality"		"checkbox"	"-inst"		"common"] \
		[list	"log"		"Create log file of generation process"		"checkbox"	"-log"		"common"] \
		[list	"main"		"Don't generate 'main' function"		"checkbox"	"-nomain"	"common"] \
		[list	"qcheck"	"Check the queue for unprocessed messages"	"checkbox"	"-qcheck"	"common"] \
		[list	"time"		"Generate time processing functionality"	"checkbox"	"-time"		"common"] \
		[list	"wrapper"	"Generate wrapper prototype"			"checkbox"	"-wrap"		"common"] \
		[list	"sdl"		"SDL version (leave blank for none)"		"integer"	"-sdl "		"sdl"] \
		[list	"reset"		"Reset signal"					"entry"		"-reset "	"sdl"] \
		[list	"env"		"Default environment instance"			"entry"		"-env "		"sdl"] \
		[list	"model"		"Default model instance"			"entry"		"-model "	"sdl"] \
	]
				
	variable options

	# Tool inputs/outputs
	variable IO_NAME	0
	variable IO_TYPE	1
	variable IO_DIR		2
	variable IO_PREFIX	3

	variable ios [list \
		[list	"TCL_FILE"	"FILE"	"in"	"-ats "] \
		[list	"ANSIC_FILE"	"FILE"	"out"	"-o "] \
	]

	# Miscellaneous data
	variable data

	# Affiliation to a notebook pad
	variable AFFIL_NAME	0
	variable AFFIL_TITLE	1
			
	variable affils [list \
		[list	"common"	"Common options"] \
		[list	"sdl"		"SDL/UML options"] \
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
	foreach option $::cgtc::defaults {
		set ::cgtc::options([lindex $option $::cgtc::OPT_NAME]) {}
	}
	foreach attrnode [::XML::findElements $toolconfig ATTRIBUTE {}] {
		set ::cgtc::options([::dom::element getAttribute $attrnode comment]) [::dom::element getAttribute $attrnode value]
	}
	
	# packing notebook
	set nbook [NoteBook $frame.nbook]
	foreach affil $::cgtc::affils {
		set name	[lindex $affil $::cgtc::AFFIL_NAME]
		set title	[lindex $affil $::cgtc::AFFIL_TITLE]

		set pane [$nbook insert end $name -text $title]
		set titleframe [TitleFrame $pane.title -text $title]
		pack $titleframe -anchor n -side left -fill x -expand yes
		set wchild($name) [$titleframe getframe]
	}
	
	# main loop
	foreach option $::cgtc::defaults {
		set name	[lindex $option $::cgtc::OPT_NAME]
		set title	[lindex $option $::cgtc::OPT_TITLE]
		set type	[lindex $option $::cgtc::OPT_TYPE]
		set prefix	[lindex $option	$::cgtc::OPT_PREFIX]
		set affil	[lindex $option $::cgtc::OPT_AFFIL]

		set picked $wchild($affil)

		set titleframe [TitleFrame $picked.title_$name -text $title]
		set child [$titleframe getframe]
		pack $titleframe -expand yes -fill x -anchor w -side top
		
		# choosing and packing GUI element
		switch $type {
			scroll {
				set elem [ScrolledWindow $child.scroll -auto both]
				set ::cgtc::data(scroll.$name) [text $elem.text -bg white -wrap word -height 7 -width 40]
				$elem setwidget $::cgtc::data(scroll.$name)
				$::cgtc::data(scroll.$name) insert end $::cgtc::options($name)
				pack $elem -anchor w -side left -fill x -expand yes
			}
			fileopen {
				pack [entry $child.entry -textvariable ::cgtc::options($name)] -side left -fill x -expand yes
				pack [button $child.button -text "Browse..." -command "::Commands::browseOpen ::cgtc::options($name)"] -side right
			}
			filesave {
				pack [entry $child.entry -textvariable ::cgtc::options($name)] -side left -fill x -expand yes
				pack [button $child.button -text "Browse..." -command "::Commands::browseSave ::cgtc::options($name)"] -side right
			}
			directory {
				pack [entry $child.entry -textvariable ::cgtc::options($name)] -side left -fill x -expand yes
				pack [button $child.button -text "Browse..." -command "::Commands::browseDirectory ::cgtc::options($name)"] -side right
			}
			integer -
			entry {
				pack [entry $child.entry -textvariable ::cgtc::options($name)] -side left -fill x -expand yes
			}
			combo {
				pack [ComboBox $child.combo -textvariable ::cgtc::options($name) -values [::XML::getConfigList] -editable 0 -helptext $title] -side left -fill x -expand yes
			}
			checkbox
			{
				pack [checkbutton $child.checkbox -text $title -variable ::cgtc::options($name) -onvalue "?" -offvalue "!"] -side left
			}
		}
	}
	pack $nbook -fill x -expand yes
	$nbook raise [lindex [lindex $::cgtc::affils 0] $::cgtc::AFFIL_NAME]
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
	foreach option $::cgtc::defaults {
		set name	[lindex $option $::cgtc::OPT_NAME]
		set title	[lindex $option $::cgtc::OPT_TITLE]
		set type	[lindex $option $::cgtc::OPT_TYPE]
		set prefix	[lindex $option	$::cgtc::OPT_PREFIX]
		set affil	[lindex $option $::cgtc::OPT_AFFIL]
		switch $type {
			scroll {
				set optval [string trimright [$::cgtc::data(scroll.$name) get 1.0 end]]
			}
			default {				
				set optval $::cgtc::options($name)
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
	return $::cgtc::ios
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

	set cmdLine [subst "tclsh $path"]

	# processing inputs and outputs
	foreach toolsig $::cgtc::ios {
		set name [lindex $toolsig $::cgtc::IO_NAME]
		set type [lindex $toolsig $::cgtc::IO_TYPE]
		set dir [lindex $toolsig $::cgtc::IO_DIR]
		set prefix [lindex $toolsig $::cgtc::IO_PREFIX]
		foreach sig $io {
			set sig [subst $sig]
			if {[string match $name [lindex $sig 0]] && [string length [lindex $sig 1]]>0} {
				set workDir [::XML::getWorkDir]
				if {[string length $workDir]==0} {set workDir "."}
				# Note: CGTC does not allow the user to specify output file names, so use output directory name
				if {[string equal $name ANSIC_FILE]} {
					append cmdLine " ${prefix}[file join $workDir [file dirname [lindex $sig 1]]]"
				} else {
					append cmdLine " ${prefix}[file join $workDir [lindex $sig 1]]"
				}
			}
		}
	}

	# processing options
	foreach attrnode [::XML::findElements $toolconfig ATTRIBUTE {}] {
		set optName [::dom::element getAttribute $attrnode "comment"]
		set optVal [::dom::element getAttribute $attrnode "value"]
		set found 0	
		foreach option $::cgtc::defaults {
			set name	[lindex $option $::cgtc::OPT_NAME]
			set title	[lindex $option $::cgtc::OPT_TITLE]
			set type	[lindex $option $::cgtc::OPT_TYPE]
			set prefix	[lindex $option	$::cgtc::OPT_PREFIX]
			set affil	[lindex $option $::cgtc::OPT_AFFIL]
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
			return -code error "$::cgtc::title: unknown option \"$optName\" specified"
		}
	}

	#tk_messageBox -title "CGTC Command Line" -message $cmdLine

	# launching on the command prompt
	if {[catch {eval exec "$cmdLine"} err]} {
		return -code error "$::cgtc::title: \"$err\""
	}  else {
		return -code ok $err
	}
}