#########################################################################
#
# COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED
#
# This code is the property of Motorola St.Petersburg Software Development	
# and is Motorola Confidential Proprietary Information.	
#
# The copyright notice above does not evidence any actual or intended
# publication of such source code.
#
# Author: rva, akk033c, vas, akk302c, adp033c
#
# History: Use the ClearCase command "History"
#          to display revision history information.     
#
# Description: Code Generation Template in C (CGTC) from ATS.
#
# Notes: None 
#
#########################################################################

#######################################################################
# Procedure: genTypeDef
# Generate a single data type definition and associated declarations
# Parameters:
# typeid - type ID
# channel - output channel
#######################################################################
proc genTypeDef {typeid channel} {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_TYPE
	global DATATYPE_DIMENSION
	global DATATYPE_STYPE
	global DATATYPE_PARAMTYPE
	global DATATYPE_MEMBERS
	global DATATYPE_RECORD_SIZE

	set name [lindex $DataTypeSet [expr $typeid*$DATATYPE_RECORD_SIZE+$DATATYPE_NAME]]
	set type [lindex $DataTypeSet [expr $typeid*$DATATYPE_RECORD_SIZE+$DATATYPE_TYPE]]
	set dim [lindex $DataTypeSet [expr $typeid*$DATATYPE_RECORD_SIZE+$DATATYPE_DIMENSION]]
	set stype [lindex $DataTypeSet [expr $typeid*$DATATYPE_RECORD_SIZE+$DATATYPE_STYPE]]
	set paramtype [lindex $DataTypeSet [expr $typeid*$DATATYPE_RECORD_SIZE+$DATATYPE_PARAMTYPE]]
	set members [lindex $DataTypeSet [expr $typeid*$DATATYPE_RECORD_SIZE+$DATATYPE_MEMBERS]]

	# Predefined data type
	if {$paramtype=="PREDEF"} return

	# SDL support
	if {$paramtype=="SDL"} {
		if {!$TConfVars::SDL} {TCommProc::genAbort "Use -sdl command line switch to enable SDL support"}
		if {$dim=="string"} {puts $channel "extern ${name}_yrec * TAT_${name}_Cur;"}
		return
	}
		
	# Process structure
	if {[llength $members]>0} {
		set structure "{\n"
		foreach member $members {
			set mname [lindex $member $DATATYPE_NAME]
			set mtype [lindex $member $DATATYPE_TYPE]
			set mdim [lindex $member $DATATYPE_DIMENSION]
			set mstype [lindex $member $DATATYPE_STYPE]
			if {$mdim==""} {
				append structure "\t$mtype $mname;\n"
				continue
			}
			if {[string is digit $mdim]} {
				append structure "\t$mtype $mname\[$mdim\];\n"
				continue
			}
			if {$mdim=="var"} {
				append structure "\tstruct \{\n\t\tint counter;\n\t\t$mtype * data;\n\t\} $mname;\n"
				continue
			}
			TCommProc::genAbort "${name}.${mname}: wrong member dimension."
		}
		append structure "}"
	} else {set structure ""}
	
	if {[string tolower $type] =="struct" } { set type "struct" }

	# Process dimension
	# Scalar
	if {$dim==""} {
		puts $channel "typedef $type $structure $name;"
		return
	}
	# Array
	if {[string is digit -strict $dim]} {
		if {$structure!=""} {
			puts $channel "typedef $type $structure _$name;\n"
			puts $channel "typedef struct \{\n\t_$name data\[$dim\];\n\} $name;"
		} else {
			puts $channel "typedef struct \{\n\t$type data\[$dim\];\n\} $name;"
		}
		return
	}
	# Variable size array
	if {$dim=="var"} {
		if {$structure!=""} {
			puts $channel "typedef $type $structure _$name;\n"
			puts $channel "typedef struct \{\n\t_$name * data;\n\tint counter;\n\} $name;"
		} else {
			puts $channel "typedef struct \{\n\t$type * data;\n\tint counter;\n\} $name;"
		}
		return
	}
	# List
	if {$dim=="list"} {
		puts $channel "typedef struct _$name * $name;\n"
		puts $channel "struct _$name \{\n$type $structure data;\n$name next;\n\};\n"
		puts $channel "extern $name TAT_${name}_Cur;"
		return
	}
	TCommProc::genAbort "${name}: wrong dimension"
}

##########################################################
# Procedure: genTypeDefs
# Generate all data type definitions
# Parameters:
# channel - output channel
##########################################################
proc genTypeDefs {channel} {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_TYPE
	global DATATYPE_DIMENSION
	global DATATYPE_STYPE
	global DATATYPE_MEMBERS
	global DATATYPE_RECORD_SIZE	

	set dataTypeSetLen [expr [llength $DataTypeSet] / $DATATYPE_RECORD_SIZE] 
	set output [list]

	# Generate until all datatypes are output
	while {[llength $output] < $dataTypeSetLen} {
		set nores 1
		#Iterate through all datatype definitions
		for {set i 0} {$i<$dataTypeSetLen} {incr i} {
			#If datatype already output then skip it
			if {[lsearch $output $i]>=0} continue
			set gen 1
			#Don't generate if depends on a datatype not yet generated
			if {([lindex $DataTypeSet [expr ${i}*${DATATYPE_RECORD_SIZE}+${DATATYPE_STYPE}]]>0) && ([lsearch $output [lindex $DataTypeSet [expr ${i}*${DATATYPE_RECORD_SIZE}+${DATATYPE_STYPE}]]]<0)} {
				set gen 0
			}
			foreach member [lindex $DataTypeSet [expr ${i}*${DATATYPE_RECORD_SIZE}+${DATATYPE_MEMBERS}]] {
				if {([lindex $member $DATATYPE_STYPE]>0) && ([lsearch $output [lindex $member $DATATYPE_STYPE]]<0)} {
					set gen 0
				}
			}
			if $gen {
				genTypeDef $i $channel
				puts $channel ""
				lappend output $i
				set nores 0
			}
		}
		#If no datatypes generated in a loop, halt generation (recursive data type definitions)
		if $nores {
			TCommProc::genAbort "Unable to resolve dependencies in data type definitions"
			return
		}
	}
}

##########################################################
# Procedure: genListCursors
# Generate pointers for iterating linked lists
# Parameters:
# cfile - output channel
##########################################################
proc genListCursors {cfile} {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_DIMENSION
	global DATATYPE_RECORD_SIZE
	
	set len [expr [llength $DataTypeSet] / $DATATYPE_RECORD_SIZE]

	for {set i 0} {$i<$len} {incr i} {
		set name [lindex $DataTypeSet [expr $i * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
		set dim [lindex $DataTypeSet [expr $i * $DATATYPE_RECORD_SIZE + $DATATYPE_DIMENSION]]
		if {$dim=="list"} {puts $cfile "$name TAT_${name}_Cur;"}
		if {$dim=="string"} {puts $cfile "${name}_yrec * TAT_${name}_Cur;"}
	}
	puts $cfile ""
}

#############################################################
# Procedure: genAssData
# Generate assignment code for specified data type
# Parameters:
# name - variable name
# typestr - variable type
# typeid - variable type ID / -1
# value - value to assign
# channel - output channel
#############################################################
proc genAssData {name typestr typeid value channel} {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_TYPE
	global DATATYPE_DIMENSION
	global DATATYPE_STYPE
	global DATATYPE_PARAMTYPE
	global DATATYPE_MEMBERS
	global DATATYPE_RECORD_SIZE

	# Basic data type
	if {$typeid<0} {
		switch $typestr {
			"SDL_Charstring" -
			"SDL_charstring" -
			"char*" -
			"char *" {puts $channel "\t$name=(char*)malloc(strlen([TCommProc::changeExpr $value])+1);"
				puts $channel "\tstrcpy($name,[TCommProc::changeExpr $value]);"}
			default {puts $channel "\t$name=[TCommProc::changeExpr $value];"}
		}
		return
	}
	
	set typestr [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
	set type [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_TYPE]]
	set dim [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_DIMENSION]]
	set stype [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_STYPE]]
	set paramtype [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_PARAMTYPE]]
	set members [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_MEMBERS]]
	
	# Scalar data
	if {$dim==""} {
		genAssDataSingle $name $typeid $value $channel
		return
	}
	
	# Array
	if {[string is digit -strict $dim]} {
		if {[string compare [string tolower [lindex $value 0]] array]} {TCommProc::genAbort "Must be array."}
		set valuelist [lrange $value 1 end]
		if {[llength $valuelist]>$dim} {TCommProc::genAbort "Array dimension mismatch."}
		if {$paramtype=="SDL"} {
			for {set i 0} {$i<[llength $valuelist]} {incr i} {
				genAssDataSingle $name.A\[$i\] $typeid [lindex $valuelist $i] $channel
			}
		} else {
			for {set i 0} {$i<[llength $valuelist]} {incr i} {
				genAssDataSingle $name.data\[$i\] $typeid [lindex $valuelist $i] $channel
			}
		}
		return
	}

	# Variable size array
	if {$dim=="var"} {
		if {[string compare [string tolower [lindex $value 0]] array]} {TCommProc::genAbort "Must be array."}
		if {$paramtype=="SDL"} {TCommProc::genAbort "Variable size arrays not supported in SDL."}
		set valuelist [lrange $value 1 end]

		if {[llength $members]>0} {set ptype "_$typestr"} else {set ptype $type}

		if {[llength $valuelist]>0} {
			puts $channel "\t$name.data=($ptype*)malloc([llength $valuelist]*sizeof($ptype));"
		} else {
			puts $channel "\t$name.data=0;"
		}
		puts $channel "\t$name.counter=[llength $valuelist];"
		for {set i 0} {$i<[llength $valuelist]} {incr i} {
			genAssDataSingle $name.data\[$i\] $typeid [lindex $valuelist $i] $channel
		}
		return
	}

	# List
	if {$dim=="list"} {
		if {[string compare [string tolower [lindex $value 0]] array]} {TCommProc::genAbort "Must be array."}
		if {$paramtype=="SDL"} {TCommProc::genAbort "Lists not supported in SDL."}
		set valuelist [lrange $value 1 end]

		set curptr "TAT_${typestr}_Cur"

		if {[llength $valuelist]<1} {
			puts $channel "\t$name=0;"
		} else {
			puts $channel "\t$name=($typestr)malloc(sizeof(struct _$typestr));"
			puts $channel "\t$curptr=$name;"
			puts $channel "\t$curptr->next=0;"
			genAssDataSingle $curptr->data $typeid [lindex $valuelist 0] $channel
			for {set i 1} {$i<[llength $valuelist]} {incr i} {
				puts $channel "\t$curptr->next=($typestr)malloc(sizeof(struct _$typestr));"
				puts $channel "\t$curptr=$curptr->next;"
				puts $channel "\t$curptr->next=0;"
				genAssDataSingle $curptr->data $typeid [lindex $valuelist $i] $channel
			}
		}
		return
	}

	# String (SDL only)
	if {$dim=="string"} {
		if {[string compare [string tolower [lindex $value 0]] array]} {TCommProc::genAbort "Must be array."}
		if {$paramtype!="SDL"} {TCommProc::genAbort "Strings supported in SDL only."}
		set valuelist [lrange $value 1 end]
		
		set curptr "TAT_${typestr}_Cur"

		if {[llength $valuelist]<1} {
			puts $channel "\t$name.First=NULL;"
			puts $channel "\t$name.Last=NULL;"
			puts $channel "\t$name.Length=0;"
			puts $channel "\t$name.IsAssigned=1;"
		} else {
			puts $channel "\t$name.First=(${typestr}_yrec*)malloc(sizeof(${typestr}_yrec));"
			puts $channel "\t$name.Last=$name.First;"
			puts $channel "\t$name.Length=1;"
			puts $channel "\t$name.IsAssigned=1;"
			puts $channel "\t$curptr=$name.First;"
			puts $channel "\t$curptr->Suc=NULL;"
			genAssDataSingle $curptr->Data $typeid [lindex $valuelist 0] $channel
			for {set i 1} {$i<[llength $valuelist]} {incr i} {
				puts $channel "\t$curptr->Suc=(${typestr}_yrec*)malloc(sizeof(${typestr}_yrec));"
				puts $channel "\t$curptr=$curptr->Suc;"
				puts $channel "\t$curptr->Suc=0;"
				puts $channel "\t$name.Last=$curptr;"
				puts $channel "\t$name.Length++;"
				genAssDataSingle $curptr->Data $typeid [lindex $valuelist $i] $channel
			}
		}
		return
	}

	# Otherwise
	TCommProc::genAbort "Wrong dimension"
}

#############################################################
# Procedure: genAssDataSingle
# Generate assignment code for a single element of specified data type
# Parameters:
# name - variable name
# typeid - variable type ID / -1
# value - value to assign
# channel - output channel
#############################################################
proc genAssDataSingle {name typeid value channel} {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_TYPE
	global DATATYPE_DIMENSION
	global DATATYPE_STYPE
	global DATATYPE_MEMBERS
	global DATATYPE_RECORD_SIZE

	set type [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_TYPE]]
	set stype [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_STYPE]]
	set members [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_MEMBERS]]
	
	if {![string compare [string tolower $type] struct]} {
		if {[string compare [string tolower [lindex $value 0]] struct]} {TCommProc::genAbort "Must be structure."}
		set valuelist [lrange $value 1 end]
		if {[llength $valuelist]!=[llength $members]} {TCommProc::genAbort "Structure mismatch."}
		foreach member $members val $valuelist {
			set mname [lindex $member $DATATYPE_NAME]
			set mtype [lindex $member $DATATYPE_TYPE]
			set mdim [lindex $member $DATATYPE_DIMENSION]
			set mstype [lindex $member $DATATYPE_STYPE]
			genAssDataMember $name.$mname $mtype $mdim $mstype $val $channel
		}
	} else {
		genAssData $name $type $stype $value $channel
	}
}

#############################################################
# Procedure: genAssDataMember
# Generate assignment code for a structure member
# Parameters:
# name - variable name
# type - variable type
# dim - dimension
# typeid - variable type ID / -1
# value - value to assign
# channel - output channel
#############################################################
proc genAssDataMember {name type dim typeid value channel} {
	if {$dim==""} {
		genAssData $name $type $typeid $value $channel
		return
	}
	
	if {[string is digit -strict $dim]} {
		if {[string compare [string tolower [lindex $value 0]] array]} {TCommProc::genAbort "Must be array."}
		set valuelist [lrange $value 1 end]
		if {[llength $valuelist]>$dim} {TCommProc::genAbort "Array dimension mismatch."}
		for {set i 0} {$i<[llength $valuelist]} {incr i} {
			genAssData $name\[$i\] $type $typeid [lindex $valuelist $i] $channel
		}
		return
	}

	if {$dim=="var"} {
		if {[string compare [string tolower [lindex $value 0]] array]} {TCommProc::genAbort "Must be array."}
		set valuelist [lrange $value 1 end]
		
		if {[llength $valuelist]>0} {
			puts $channel "\t$name.data=($type*)malloc([llength $valuelist]*sizeof($type));"
		} else {
			puts $channel "\t$name.data=0;"
		}
		puts $channel "\t$name.counter=[llength $valuelist];"
		for {set i 0} {$i<[llength $valuelist]} {incr i} {
			genAssData $name.data\[$i\] $type $typeid [lindex $valuelist $i] $channel
		}
		return
	}

	TCommProc::genAbort "Wrong member dimension."
}

#############################################################
# Procedure: genAssFunc
# Generate assignment function
# Parameters:
# name - function name
# typestr - data type
# typeid - datatype ID / -1
# value - value to assign
# channel - output channel
#############################################################
proc genAssFunc {name type typeid value channel} {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_RECORD_SIZE
	
	if {$typeid!=-1} {set type [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]}
	
	puts $channel "$type $name (TAT_TEST_INSTANCE *id) \{"
	puts $channel "\t$type x;"
	genAssData x $type $typeid $value $channel
	puts $channel "\treturn x;"
	puts $channel "\}"
}

#############################################################
# Procedure: genCmpData
# Generate compare code for specified data type
# Parameters:
# name - variable name
# typestr - variable type
# typeid - variable type ID / -1
# value - value to compare
# channel - output channel
#############################################################
proc genCmpData {name typestr typeid value channel} {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_TYPE
	global DATATYPE_DIMENSION
	global DATATYPE_STYPE
	global DATATYPE_PARAMTYPE
	global DATATYPE_MEMBERS
	global DATATYPE_RECORD_SIZE

	# Basic data type
	if {$typeid<0} {
		switch $typestr {
			"SDL_Charstring" -
			"SDL_charstring" -
			"char*" -
			"char *" {puts $channel "\tif(strcmp($name,[TCommProc::changeExpr $value])) return 1;"}
			default {puts $channel "\tif($name!=[TCommProc::changeExpr $value]) return 1;"}
		}
		return
	}
	
	set typestr [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
	set type [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_TYPE]]
	set dim [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_DIMENSION]]
	set stype [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_STYPE]]
	set paramtype [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_PARAMTYPE]]
	set members [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_MEMBERS]]
	
	# Scalar data
	if {$dim==""} {
		genCmpDataSingle $name $typeid $value $channel
		return
	}
	
	# Array
	if {[string is digit -strict $dim]} {
		if {[string compare [string tolower [lindex $value 0]] array]} {TCommProc::genAbort "Must be array."}
		set valuelist [lrange $value 1 end]
		if {[llength $valuelist]>$dim} {TCommProc::genAbort "Array dimension mismatch."}
		if {$paramtype=="SDL"} {
			for {set i 0} {$i<[llength $valuelist]} {incr i} {
				genCmpDataSingle $name.A\[$i\] $typeid [lindex $valuelist $i] $channel
			}
		} else {
			for {set i 0} {$i<[llength $valuelist]} {incr i} {
				genCmpDataSingle $name.data\[$i\] $typeid [lindex $valuelist $i] $channel
			}
		}
		return
	}

	# Variable size array
	if {$dim=="var"} {
		if {[string compare [string tolower [lindex $value 0]] array]} {TCommProc::genAbort "Must be array."}
		if {$paramtype=="SDL"} {TCommProc::genAbort "Variable size arrays not supported in SDL."}
		set valuelist [lrange $value 1 end]
		puts $channel "\tif($name.counter!=[llength $valuelist]) return 1;"
		for {set i 0} {$i<[llength $valuelist]} {incr i} {
			genCmpDataSingle $name.data\[$i\] $typeid [lindex $valuelist $i] $channel
		}
		return
	}
	
	#List
	if {$dim=="list"} {
		if {[string compare [string tolower [lindex $value 0]] array]} {TCommProc::genAbort "Must be array."}
		if {$paramtype=="SDL"} {TCommProc::genAbort "Lists not supported in SDL."}
		set valuelist [lrange $value 1 end]

		set curptr "TAT_${typestr}_Cur"

		if {[llength $valuelist]<1} {
			puts $channel "\tif($name!=0) return 1;"
		} else {
			puts $channel "\t$curptr=$name;"
			foreach val $valuelist {
				puts $channel "\tif($curptr==0) return 1;"
				genCmpDataSingle $curptr->data $typeid $val $channel
				puts $channel "\t$curptr=$curptr->next;"
			}
			puts $channel "\tif($curptr!=0) return 1;"
		}
		return
	}

	# String (SDL only)
	if {$dim=="string"} {
		if {[string compare [string tolower [lindex $value 0]] array]} {TCommProc::genAbort "Must be array."}
		if {$paramtype!="SDL"} {TCommProc::genAbort "Strings supported in SDL only."}
		set valuelist [lrange $value 1 end]

		set curptr "TAT_${typestr}_Cur"

		if {[llength $valuelist]<1} {
			puts $channel "\tif($name.Length>0) return 1;"
		} else {
			puts $channel "\t$curptr=$name.First;"
			foreach val $valuelist {
				puts $channel "\tif($curptr==NULL) return 1;"
				genCmpDataSingle $curptr->Data $typeid $val $channel
				puts $channel "\t$curptr=$curptr->Suc;"
			}
			puts $channel "\tif($curptr!=NULL) return 1;"
		}
		return
	}
	
	#Otherwise
	TCommProc::genAbort "Wrong dimension"
}

#############################################################
# Procedure: genCmpDataSingle
# Generate compare code for a single element of specified data type
# Parameters:
# name - variable name
# typeid - variable type ID / -1
# value - value to compare
# channel - output channel
#############################################################
proc genCmpDataSingle {name typeid value channel} {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_TYPE
	global DATATYPE_DIMENSION
	global DATATYPE_STYPE
	global DATATYPE_MEMBERS
	global DATATYPE_RECORD_SIZE

	set type [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_TYPE]]
	set stype [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_STYPE]]
	set members [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_MEMBERS]]
	
	if {![string compare [string tolower $type] struct]} {
		if {[string compare [string tolower [lindex $value 0]] struct]} {TCommProc::genAbort "Must be structure."}
		set valuelist [lrange $value 1 end]
		if {[llength $valuelist]!=[llength $members]} {TCommProc::genAbort "Structure mismatch."}
		foreach member $members val $valuelist {
			set mname [lindex $member $DATATYPE_NAME]
			set mtype [lindex $member $DATATYPE_TYPE]
			set mdim [lindex $member $DATATYPE_DIMENSION]
			set mstype [lindex $member $DATATYPE_STYPE]
			genCmpDataMember $name.$mname $mtype $mdim $mstype $val $channel
		}
	} else {
		genCmpData $name $type $stype $value $channel
	}
}

#############################################################
# Procedure: genCmpDataMember
# Generate compare code for a structure member
# Parameters:
# name - variable name
# type - variable type
# dim - dimension
# typeid - variable type ID / -1
# value - value to compare
# channel - output channel
#############################################################
proc genCmpDataMember {name type dim typeid value channel} {
	if {$dim==""} {
		genCmpData $name $type $typeid $value $channel
		return
	}
	
	if {[string is digit -strict $dim]} {
		if {[string compare [string tolower [lindex $value 0]] array]} {TCommProc::genAbort "Must be array."}
		set valuelist [lrange $value 1 end]
		if {[llength $valuelist]>$dim} {TCommProc::genAbort "Array dimension mismatch."}
		for {set i 0} {$i<[llength $valuelist]} {incr i} {
			genCmpData $name\[$i\] $type $typeid [lindex $valuelist $i] $channel
		}
		return
	}

	if {$dim=="var"} {
		if {[string compare [string tolower [lindex $value 0]] array]} {TCommProc::genAbort "Must be array."}
		set valuelist [lrange $value 1 end]
		puts $channel "\tif($name.counter!=[llength $valuelist]) return 1;"
		for {set i 0} {$i<[llength $valuelist]} {incr i} {
			genCmpData $name.data\[$i\] $type $typeid [lindex $valuelist $i] $channel
		}
		return
	}

	TCommProc::genAbort "Wrong member dimension."
}

#############################################################
# Procedure: genCmpFunc
# Generate compare function
# Parameters:
# name - function name
# typestr - data type
# typeid - datatype ID / -1
# value - value to compare
# channel - output channel
#############################################################
proc genCmpFunc {name type typeid value channel} {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_RECORD_SIZE
	
	if {$typeid!=-1} {set type [lindex $DataTypeSet [expr $typeid * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]}
	
	puts $channel "int $name ($type x, TAT_TEST_INSTANCE *id) \{"
	genCmpData x $type $typeid $value $channel
	puts $channel "\treturn 0;"
	puts $channel "\}"
}


##########################################################################
proc embedLine {str} {
	set out "\""
	set len [string length $str]
	for {set i 0} {$i<$len} {incr i} {
		set ch [string index $str $i]
		switch -- $ch {
			"\n" {  append out "\\n" }
			"\"" {  append out "\\\"" }
			"\\" {  append out "\\\\" }
			"\t" {  append out "\\t" }
			default { append out $ch }
		}
	}
	append out "\""
	return $out
}

#############################################################
# Procedure: isASN1
# Check if argument is an ASN.1 string processed by ATSGEN
# Parameters:
# str - string to check
# Returns: 0/1
#############################################################
proc isASN1 {str} {
	if {[llength $str]<1} {
		return 0
	} elseif {([lindex $str 0]!="ARRAY") && ([lindex $str 0]!="STRUCT")} {
		return 0
	} else {
		return 1
	}
}

namespace eval TCommProc {

    #template generation log file
    variable tLogFile ""

    #test suite common header file
    variable hCommFile ""
    
    #test suite common source file
    variable sCommFile ""
        
    #test case (suite) source file
    variable tSourceFile ""
	
    #generated code indentation
    variable inden ""

    #generated ATS mode 0 -BHT, 1-TTS, 2-flat (not supported)
    variable ATSmode -1

    #name of current test suite
    variable testSuite ""

    #name of current test case
    variable testCase ""

    #number of states in current test case
    variable stateCount ""

    #list of time labels
    variable labelList ""

    #list of variables in test case
    variable varList ""
    
    #list of functions in test case
    variable stateFunc ""

    #list of test cases in test suite
    variable testCount ""
    
    #list of events in test case
    variable evList ""
    
    #number of using loop 
    variable loopID ""

    #max number of loops
    variable loopMax ""

    #list for nested loops support
    variable loopIDLst {}

    #index of nesting level
    variable loopNest ""

    #########################################################################
    # Procedure : createParamsCommentsLst
    # 
    # Description: create string of comments for each signal`s parameter
    # 
    # Returns: created string
    # 
    # Notes: None
    # 
    #########################################################################
	proc createParamsCommentsLst { paramsLst fieldLst} {
		set counter 0
		set tmpList {}
		if { $paramsLst == "" || $fieldLst == "" } {
			return "NULL"
		}
		foreach paramStr $paramsLst {
			set commStr ""
			set str [string trim [lindex $paramStr 1]]

			while { 1 } {
				set startInd [string first "/*" $paramStr]
				if { $startInd == -1 } {
					break
				}
				set finishInd [string first "*/" $paramStr]
				append commStr [string range $paramStr $startInd [expr $finishInd + 1]]
				set paramStr [string replace $paramStr $startInd [expr $finishInd + 1] ""]
			}

			if { $commStr != "" } {
				lappend tmpList "\"$commStr\""
			} else {
				lappend tmpList "\"/*[lindex $fieldLst $counter]*/\""
			}
			incr counter
		}
		return "\{[join $tmpList ", "]\}"
	}
    #########################################################################
    # Procedure : changeExpr
    # 
    # Description: change variable and time label references to actual storage
    # 
    # Returns: updated expression
    # 
    # Notes: None
    # 
    #########################################################################
    proc changeExpr { str } {
        variable labelList
        variable varList
        variable testCase
        set out ""
        set len [string length $str]
        set i 0
        while {$i<$len} {
        set ch [string index $str $i]
        switch -- $ch {
            "@" {
                    #extract time label name
                    incr i
                    set ch [string index $str $i]
                    set varname ""
                    if {[string is alpha $ch]==1} {
                        while {$i<$len && ([string is alnum $ch] || $ch=="_")} {
                            append varname $ch
                            incr i
                            set ch [string index $str $i]
                        }
                        set ind [lsearch -exact $labelList $varname]
                        if {$ind != -1} then {
                            append out "id->timeLV\[${ind}\]"
                        } else {
                            append out $varname
                        }
                        continue
                    }
                }
            "\"" {  append out $ch
                    incr i
                    set ch [string index $str $i]
                    while {$i<$len && $ch!="\""} {
                        append out $ch
                        incr i
                        set ch [string index $str $i]
                    }
                    #append last quote
                    append out $ch
                }
            "." {
			#adp033c: added to avoid changing member names
			append out $ch
			incr i
			set ch [string index $str $i]
			set varname ""
			while {$i<$len && ([string is alnum $ch] || $ch=="_")} {
				append varname $ch
				incr i
				set ch [string index $str $i]
			}
			incr i -1
			append out $varname
		}		
            default {
                    if {[string is alpha $ch]==1} then {
                        set varname ""
                        while {$i<$len && ([string is alnum $ch] || $ch=="_") } {
                            append varname $ch
                            incr i
                            set ch [string index $str $i]
                        }
                        incr i -1
                        set ind [lsearch -exact $varList $varname]
                        if {$ind != -1} then {
                            #output symbolic name of variable
                            append out "var_${testCase}_${varname}"
                        } else {
                            append out $varname
                        }
                    } else {
                        append out $ch
                    }
                }
        }
        incr i
        }
       
        return $out
    }
    
    #########################################################################
    # Procedure : genAbort
    # 
    # Description: Break test suite generation in error case
    # 
    # Returns: 1
    # 
    # Notes: Internal service procedure
    # 
    #########################################################################
    proc genAbort { errMsg } {
        variable tLogFile
                        
        #close all opened files
        if { ($TConfVars::templLog == 1) &&  ($tLogFile != "") } then {
                puts $tLogFile $errMsg
                puts $tLogFile "Test suite generation aborted"
            close $tLogFile
        } else {
                puts $errMsg
                puts "Test suite generation aborted"
        }
        if { $TCommProc::tSourceFile != "" } then {
            close $TCommProc::tSourceFile
        }
        exit 1
    }

    #########################################################################
    # Procedure : dataOutFunc
    # 
    # Description: generates name of function for dumping specific datatype
    # first parameter is data type in text, second - source data type ID/-1
    # 
    # Returns: function name
    # 
    # Notes: Internal service procedure
    # 
    #########################################################################
    proc dataOutFunc { sgParam dindex } {
        global DataTypeSet DATATYPE_RECORD_SIZE DATATYPE_ATTR
        set fname ""
        if {$dindex !=-1} then {
            set fname [lindex [lindex $DataTypeSet [expr {$dindex * $DATATYPE_RECORD_SIZE + $DATATYPE_ATTR} ]] 0]
	    if {$fname==""} {set fname TAT_Dump_$sgParam}
        }
        if { ($fname=="") && $TConfVars::SDL} {
	    switch -exact $sgParam {
		"SDL_Octet" -
		"SDL_octet" -
		"SDL_Bit" -
		"SDL_bit" {set fname "TAT_Dump_Uchar" }
		"SDL_Boolean" -
		"SDL_boolean" -
		"SDL_Natural" -
		"SDL_natural" -
                "SDL_Integer" -
		"SDL_integer" { set fname "TAT_Dump_int" }
		"SDL_Real" -
		"SDL_real" { set fname "TAT_Dump_double" }
		"SDL_Character" -
		"SDL_character" { set fname "TAT_Dump_char" }
		"SDL_Charstring" -
		"SDL_charstring" { set fname "TAT_Dump_String" }
            }
        }
        if { $fname == "" } then {
            # try to define dumping function from name of type
            switch -exact $sgParam {
                "double" -
                "float"  -
                "int" -
                "char" { set fname "TAT_Dump_${sgParam}" }
                "unsigned int" { set fname "TAT_Dump_Uint" }
                "unsigned char" { set fname "TAT_Dump_Uchar" }
		"char *" -
                "char*" { set fname "TAT_Dump_String" }
                default {
                    puts "WARNING: ats: no function is defined for dumping type '$sgParam'"
                    set fname "TAT_Dump_int"
                }
            }
        }
        return $fname 
    }
    
    #########################################################################
    # Procedure : genCommonCode
    # 
    # Description: Generates common code for entire test suite
    # 
    # Returns: None
    # 
    # Notes: None 
    # 
    #########################################################################
    proc genCommonCode { testSuiteName } {
        global IncludeFiles InstanceSet SignalSet DataTypeSet TestCaseNames
        global INSTANCE_RECORD_SIZE SIGNAL_RECORD_SIZE SIGNAL_NAME INSTANCE_ATTR DATATYPE_ATTR DATATYPE_RECORD_SIZE

        variable hCommFile
        variable sCommFile
        
        #create common header file
        if { [catch { set hCommFile [open [file join $TConfVars::outputDir ${testSuiteName}.h] w ] } res ] } then {
            TCommProc::genAbort "ERROR: file: '[file join $TConfVars::outputDir ${testSuiteName}.h]': can not be open for writing\n$res"
        }
        
        # creating a test common code file
        if { [catch {set sCommFile [open [file join $TConfVars::outputDir ${testSuiteName}data.c] w ] } res ] } then {
            close $hCommFile
            TCommProc::genAbort "ERROR: file: '[file join $TConfVars::outputDir ${testSuiteName}data.c]': can not be open for writing\n$res"
        }
        fconfigure $hCommFile -buffersize 524288
        fconfigure $sCommFile -buffersize 524288
	
        puts $hCommFile "/* TAT TEST SUITE HEADER FILE */"
        puts $hCommFile "#ifndef TAT_${testSuiteName}_H"
        puts $hCommFile "#define TAT_${testSuiteName}_H"
        
        # generate guards to avoid improper compilation
        if {$TConfVars::instCheck==1} then {
            puts $hCommFile "#ifndef TAT_INSTANCE_SUPPORT"
            puts $hCommFile "#error TAT_INSTANCE_SUPPORT define not set for compilation"
            puts $hCommFile "#endif"
        }
        if {$TConfVars::timeCheck==1} then {
            puts $hCommFile "#ifndef TAT_TIME_SUPPORT"
            puts $hCommFile "#error TAT_TIME_SUPPORT define not set for compilation"
            puts $hCommFile "#endif"
        }
        if { $TConfVars::genDebugger == 1 } then {
            puts $hCommFile "#ifndef TAT_DEBUG_SUPPORT"
            puts $hCommFile "#error TAT_DEBUG_SUPPORT define not set for compilation"
            puts $hCommFile "#endif"
        }

        puts $hCommFile "#include <testkern.h>"
	if {$TConfVars::SDL} {puts $hCommFile "#include <scttypes.h>"}
       
        #user specified include files
        foreach header $IncludeFiles {
            puts $hCommFile "#include \"$header\""
        }
        puts $hCommFile ""
        
        #generate include files
        #puts $sCommFile "#include <stdlib.h>"
        #puts $sCommFile "#include <string.h>"
        #puts $sCommFile "#include <stdio.h>"
        #puts $sCommFile "#include <testkern.h>"
        puts $sCommFile "#include \"${testSuiteName}.h\"\n"
        
     
        # process instances
        set object_id ""
        set counter 0
        set count 0
        set pcount [expr {[llength $InstanceSet] / $INSTANCE_RECORD_SIZE}]
        
        puts $sCommFile "static TAT_INSTANCE_DEF tat_InstanceDefinition\[\]=\{"
        foreach { iname itype iattr iparams istype ifield iparamtype ivararg iprior}  $InstanceSet {
            incr counter
        #generate instance ID
            set instID [lindex $iattr 0]
            if { $instID != "" } {
                #if defined by user, use it if not duplicated
                if {[lsearch -exact $object_id $instID] != -1 } then {
                    puts "WARNING: ats: file '$TConfVars::ATSFile': duplicated instance ID $instID for $iname"
                }
                puts $hCommFile "#define TAT_I_${iname}_ID $instID"
                lappend object_id $instID
            } else {
                #generate unique number
                set instID $counter
                while {[lsearch -exact $object_id $instID] != -1} {
                    incr counter
                    set instID $counter
                }
                puts $hCommFile "#define TAT_I_${iname}_ID $instID"
                lappend object_id $instID
            }
            puts -nonewline $sCommFile "\{TAT_I_${iname}_ID, \"${iname}\", ${iprior}\}"
            incr count
            if {$count==$pcount} then {
                puts $sCommFile ""
            } else {
                puts $sCommFile ","
            }
        }
        puts $sCommFile "\};\n\n"
        
        puts $hCommFile ""
        
	# Generate type definitions, service functions, etc.
	genTypeDefs $hCommFile
	genListCursors $sCommFile
	genDumpFunctions $sCommFile $hCommFile
	genFreeFunctions $sCommFile $hCommFile
	genCopyFunctions $sCommFile $hCommFile
	genCompareFunctions $sCommFile $hCommFile

	set dataTypeSetLen [expr [llength $DataTypeSet] / $DATATYPE_RECORD_SIZE]
	
	# generate prototypes for user's dumping functions
        #akk302c: здесь было сделано без привязки к константам ATS, исправлено
        global DATATYPE_ATTR
        for {set dataTypeCounter 0} { $dataTypeCounter < $dataTypeSetLen } { incr dataTypeCounter } {
            set iattr [lindex $DataTypeSet [expr $dataTypeCounter * $DATATYPE_RECORD_SIZE + $DATATYPE_ATTR]]
            set object_id [lindex $iattr 0]
            if {$object_id != ""} then {
                puts $hCommFile "extern int ${object_id} (void *, char *);"
            }
        }
        puts $hCommFile ""
        
        # process instances
        set object_id ""
        
        #counter for signal ID
        set counter 0
        
        #counter for parameter's ID
        set paramID 0
        
        foreach { iname itype iattr iparams istype ifield iparamtype ivararg iprior} $SignalSet {
            incr counter
            #generate signal ID and store it in the test header file
            set instID [lindex $iattr 0]
            if { $instID != "" } {
                #if defined by user, use it if not duplicated
                if {[lsearch -exact $object_id $instID] != -1 } then {
                    puts "WARNING: ats: file '$TConfVars::ATSFile': duplicated signal ID $instID for $iname"
                }
                puts $hCommFile "#define TAT_S_${iname}_ID $instID"
                lappend object_id $instID
            } else {
                #generate unique number
                set instID $counter
                while {[lsearch -exact $object_id $instID] != -1} {
                    incr counter
                    set instID $counter
                }
                puts $hCommFile "#define TAT_S_${iname}_ID $instID"
                lappend object_id $instID
            }
            
            #generate signal structure
            #begin###############modified by akk302c
            global DATATYPE_TYPE
            if {$iparams != ""} then {
                set count 0
                puts $hCommFile "typedef struct \{"
                puts $sCommFile "static TAT_SIG_PARAM_DEF tat_SIG_PARAM_${iname}\[\]=\{"
                set pcount [llength $iparams]
                foreach sgParam $iparams sgSType $istype {
                	set paramName [lindex $ifield $count]
                	puts $hCommFile "\t$sgParam $paramName;"
			set dumpFname [dataOutFunc $sgParam [lindex $istype $count]]
			puts -nonewline $sCommFile "\t\{$paramID,\"$paramName\",sizeof($sgParam), offsetof(TAT_SIG_${iname}, ${paramName}), $dumpFname\}"
			#end###############modified by akk302c
                    incr count
                    if {$count==$pcount} then {
                        puts $sCommFile ""
                    } else {
                        puts $sCommFile ","
                    }
                    incr paramID
                }
                puts $hCommFile "\} TAT_SIG_${iname};\n"
                puts $sCommFile "\};" 
            }
            #output pack message function
            if {$itype=="in" || $itype=="inout"} then {
                puts -nonewline $hCommFile "extern TAT_TEST_RESULT TAT_Send_${iname} ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc"
                puts -nonewline $sCommFile "TAT_TEST_RESULT TAT_Send_${iname} ( TAT_TEST_INSTANCE * id, TAT_EVENT_INSTANCE *ev, TAT_EVENT_INSTANCE *evSrc"
                set count 0
                foreach sgParam $iparams sgSType $istype {
                    set paramName [lindex $ifield $count]
                    	#begin###############modified by akk302c
                	puts -nonewline $hCommFile ", $sgParam $paramName"
			puts -nonewline $sCommFile ", $sgParam $paramName"
			#end###############modified by akk302c
                    incr count
                }
                puts $hCommFile ");"
                puts $sCommFile ")\n\{"
                set count 0
                foreach sgParam $iparams {
                    set paramName [lindex $ifield $count]
                    puts $sCommFile "\t((TAT_SIG_${iname} *)ev->eventData)->$paramName=$paramName;"
                    incr count
                }
                puts $sCommFile "\tev->eventID=TAT_S_${iname}_ID;"
                puts $sCommFile "\tif(id->dumpEvent(id,ev,evSrc)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;"
                puts $sCommFile "\treturn (id->testResult=id->sendMessage(id, ev));"
                puts $sCommFile "\}"
            }
            puts $hCommFile ""
        }
        unset object_id
        
        #output signal definition table
        puts $sCommFile "static TAT_SIGNAL_DEF tat_SignalDefinition\[\]=\{"
        set pcount [expr {[llength $SignalSet] / $SIGNAL_RECORD_SIZE}]
        set count 0
        foreach { iname itype iattr iparams istype ifield iparamtype ivararg iprior}  $SignalSet {
            set dumpFname "NULL"
            set sizeF "0"
            if {$iparams != ""} then {
                set dumpFname "(TAT_SIG_PARAM_DEF *)\&tat_SIG_PARAM_${iname}"
                set sizeF "sizeof(TAT_SIG_${iname})"
            }
            puts -nonewline $sCommFile [format "\t\{%24s,\t\"%s\", %s, %d, %s, %d\}" TAT_S_${iname}_ID $iname $sizeF [llength $iparams] $dumpFname $iprior]
            incr count
            if {$count==$pcount} then {
                puts $sCommFile ""
            } else {
                puts $sCommFile ","
            }
        }
        puts $sCommFile "\};\n\n"
        
        puts $hCommFile "extern TAT_TEST_RESULT TAT_Init_${testSuiteName} (TAT_TEST_INSTANCE * id);"
        puts $hCommFile "extern TAT_TEST_TABLE TAT_${testSuiteName}_TestTable\[\];"
        
        puts $sCommFile "static int _ssort(const void *x1, const void *x2)"
        puts $sCommFile "\{"
        puts $sCommFile "\treturn (( TAT_SIGNAL_DEF * ) x1 )->signalID - (( TAT_SIGNAL_DEF * ) x2 )->signalID;"
        puts $sCommFile "\}"
        puts $sCommFile "static int _isort(const void *x1, const void *x2)"
        puts $sCommFile "\{"
        puts $sCommFile "\treturn (( TAT_INSTANCE_DEF * ) x1 )->instanceID - (( TAT_INSTANCE_DEF * ) x2 )->instanceID;"
        puts $sCommFile "\}"
        
        puts $sCommFile "TAT_TEST_RESULT TAT_Init_${testSuiteName} (TAT_TEST_INSTANCE * id)"
        puts $sCommFile "\{"
        puts $sCommFile "\tif (!id) return TAT_TEST_CRITICAL;"
        puts $sCommFile "\tid->signalCount = [expr {[llength $SignalSet] / $SIGNAL_RECORD_SIZE}];"
        puts $sCommFile "\tid->signalDef = (TAT_SIGNAL_DEF *)\&tat_SignalDefinition;"
        
        puts $sCommFile "\tid->instanceCount = [expr {[llength $InstanceSet] / $INSTANCE_RECORD_SIZE}];"
        puts $sCommFile "\tid->instanceDef = (TAT_INSTANCE_DEF *)\&tat_InstanceDefinition;"
        #sort signal declarations by SIGNAL ID
        puts $sCommFile "\tqsort(id->signalDef, id->signalCount, sizeof(TAT_SIGNAL_DEF), _ssort);"
        
        #sort instance declarations by INSTANCE ID
        puts $sCommFile "\tqsort(id->instanceDef, id->instanceCount, sizeof(TAT_INSTANCE_DEF), _isort);"
        
        #initialize specific structures
        puts $sCommFile "\tid->currSuiteName = \"${testSuiteName}\";"

        puts $sCommFile "\tid->testCount = [llength $TestCaseNames];"
        puts $sCommFile "\tid->testTable = (TAT_TEST_TABLE *)\&TAT_${testSuiteName}_TestTable;"
        puts $sCommFile "\treturn TAT_TEST_CONTINUE;"
        puts $sCommFile "\}"

        unset dumpFname
        unset sizeF
    }
    
    #########################################################################
    # Procedure : stTestSuite
    # 
    # Description: Begin a test suite
    # 
    # Returns: None
    # 
    # Notes: None
    # 
    #########################################################################
    proc stTestSuite { testSuiteName } {
        global TEST_CASES
        variable tSourceFile
        
        # creating a test suite file
        if { $TConfVars::genMultiTC == 0 } then {
            if { [catch { set tSourceFile [open [file join $TConfVars::outputDir ${testSuiteName}.c] w ] } res ] } then {
                TCommProc::genAbort "ERROR: file: '[file join $TConfVars::outputDir ${testSuiteName}.c]\': can not open for writing\n$res"
            }
            fconfigure $tSourceFile -buffersize 524288
            #puts $tSourceFile "#include <stdlib.h>"
            #puts $tSourceFile "#include <testkern.h>"
            puts $tSourceFile "#include \"${testSuiteName}.h\"\n"
        }
        puts $TCommProc::tLogFile "REMARK: Test suite \"$testSuiteName\" generation started"
    }

    #########################################################################
    # Procedure : endTestSuite
    # 
    # Description: Complete a test suite
    # 
    # Returns: ok
    # 
    # Notes: None
    # 
    #########################################################################
    proc endTestSuite { testSuiteName } {
        
        #close test suite file
        if { $TConfVars::genMultiTC == 0 } then {
                close $TCommProc::tSourceFile
        }
    puts $TCommProc::tLogFile "REMARK: Test suite \"$testSuiteName\" generation finished"
    close $TCommProc::tLogFile
    
    #close common files
    TCommProc::closeCommFiles
   
    return -code ok
    }

    #########################################################################
    # Procedure : closeCommFiles
    # 
    # Description: closes test suite common files
    # 
    # Returns: None
    # 
    # Notes: None
    # 
    #########################################################################
    proc closeCommFiles { } {
        variable hCommFile
        variable sCommFile
        puts $hCommFile "#endif"
	close $hCommFile
    	close $sCommFile
    }
    
}

namespace eval TConfVars {

    #create and initialize configuration vars as default settings
    # ats file name
    variable ATSFile ""

    #instance support
    variable instCheck 0
    
    #time support
    variable timeCheck 0

    #gen process log support
    variable templLog 0

    #don't gen main function
    variable noMain 0

    #set output dir
    variable outputDir [pwd]
    
    #gen exit from test after first error
    variable testBreak 0

    #check queue befor each send
    variable queueCheck 0

    #discard unexpected signals
    variable discardUnexp 0

    #output MSC comments to target code
    variable genComment 0

    #gen own file for each test case
    variable genMultiTC 0

    #output signal`s parameters comments
    variable sgParamsComm 0

    #gen wrapper prototype
    variable genWrapper ""

    #included files for wrapper prototype
    variable genInclWrapper {}

    #SDL support
    variable SDL 0

    #Reset signal
    variable Reset ""

    #Default environment instance
    variable DefaultEnv ""

    #Default model instance
    variable DefaultModel ""

    #gen functionality for debugger support
    variable genDebugger 0

    #check for Single Message Table
    variable singleMT 0
    
    #########################################################################
    # Procedure : printHelp
    # 
    # Description: Printing help information - key usage
    # 
    # Returns: None
    # 
    # Notes: Internal service procedure 
    # 
    #########################################################################
    proc printHelp {} {
        puts "Mandatory switches:"
        puts "\t-ats <file name>         set source ATS file"
        puts "\nOptional switches:"
        puts "\t-break                   generate break after first error detection"
        puts "\t-inst                    generate instance check functionality"
        puts "\t-log                     create log file of generation process"
        puts "\t-multi                   generate each test case in a file"
        puts "\t-nomain                  don`t generate 'main' function"
        puts "\t-o <path>                set output directory for generated files"
        puts "\t-qcheck                  check the queue for unprocessed messages"
        puts "\t-discard                 discard unprocessed messages"
        puts "\t-time                    generate time processing functionality"
        puts "\t-spcomm                  output signal`s parameters comments"
        puts "\t-wrap \[<file name>\]      generate wrapper prototype"
        puts "\t-iwrap \[<files name>\]    included files to wrapper prototype"
        puts "\t-sdl \[<TAU version>\]     SDL support"
	puts "\t-reset <signal name>     specify signal to reset SDL system"
	puts "\t-env <instance name>     specify default environment instance"
	puts "\t-model <instance name>   specify default model instance"
    }

    #########################################################################
    # Procedure : processCommandLine
    # 
    # Description: processing command line
    # 
    # Returns: None
    # 
    # Notes: Internal service procedure 
    # 
    #########################################################################
    proc processCommandLine { argNumb argLine } {
        #for mandatory switch control
        set ATSFlag 0

        if { $argNumb == 0 } then {
            printHelp
            exit 0
        } else {
            for {set i 0} {$i < $argNumb} {incr i} {
                switch -regexp -- [set arg [lindex $argLine $i]] {
                    "^\[-/\]ats$" {
                        #Option for source ATS script file name
                        incr i
                        if { ($i < $argNumb) && ([string index [lindex $argLine $i] 0] != "-") } then {
                            set TConfVars::ATSFile [lindex $argLine $i]
                            set ATSFlag 1
                        } else {
                            puts "ERROR: command-line: Data expected for switch '-ats'"
                            exit 1
                        }
                    }           
                    "^\[-/\]inst$" {
                        #option for chesk instance
                        set TConfVars::instCheck 1
                    }           
                    "^\[-/\]log$" {
                        #Option for logging generation process
                        set TConfVars::templLog 1
                    }           
                    "^\[-/\]nomain$" {
                        #Option for not gen main func to code
                        set TConfVars::noMain 1
                    }           
                    "^\[-/\]o$" {
                        #Option for set output dir
                        incr i
                        if { ($i < $argNumb) && ([string index [lindex $argLine $i] 0] != "-") } then {
                            set TConfVars::outputDir [lindex $argLine $i]
                        } else {
                            puts "ERROR: command-line: data expected for switch '-o'"
                            exit 1
                        }
                    }           
                    "^\[-/\]time$" {
                        #Option for gen time processing
                        set TConfVars::timeCheck 1
                    }           
                    "^\[-/\]break$" {
                        #Option for break test execution after first error detection
                        set TConfVars::testBreak 1
                    }           
                    "^\[-/\]qcheck$" {
                        #Option for gen check queue before sending a message
                        set TConfVars::queueCheck 1
                    }
                    "^\[-/\]discard$" {
                        #Option for discarding unprocessed messages
                        set TConfVars::discardUnexp 1
                    }
                    "^\[-/\]multi$" {
                        #Option for gen file for each test case
                        set TConfVars::genMultiTC 1
                    }           
                    "^\[-/\]spcomm$" {
                        #Option for gen file for each test case
                        set TConfVars::sgParamsComm 1
                    }           
                    "^\[-/\]wrap$" {
                        #Option for generate wrapper prototype for with stub functions
                        incr i
                        if { ($i < $argNumb) && ([string index [lindex $argLine $i] 0] != "-") } then {
                            set TConfVars::genWrapper [lindex $argLine $i]
                        } else {
                            set TConfVars::genWrapper "wrapper"
                            incr i -1
                        }
                    }           
                    "^\[-/\]iwrap$" {
                        #Option to set included files to generating wrapper prototype
                        incr i
                        while { ($i < $argNumb) && ([string index [lindex $argLine $i] 0] != "-") } {
                            lappend TConfVars::genInclWrapper [lindex $argLine $i]
	                        incr i
                        }
						incr i -1
						if { [llength $TConfVars::genInclWrapper] == 0 } {
                            puts "ERROR: command-line: switch '-iwrap' should contains included files for wrapper"                            
                            exit 1
                        }
                    }           
                    "^\[-/\]timeout$" {
                        #Option for set timeout value for recv signals
                        incr i
                        if { ($i < $argNumb) && ([string index [lindex $argLine $i] 0] != "-") } then {
                            if { ([string is integer -strict [lindex $argLine $i]]) && ([expr [lindex $argLine $i] >= 0]) } then {
                                set TConfVars::setTimeout [lindex $argLine $i]
                            } else {
                                puts "ERROR: command-line: invalid value for switch '-timeout'"                            
                                exit 1
                            }
                        } else {
                            puts "ERROR: command-line: data expected for switch '-timeout'"
                            exit 1
                        }
                    }
                    "^\[-/\]sdl$" {
			incr i
			if { ($i < $argNumb) && ([string index [lindex $argLine $i] 0] != "-") } then {
				if { ([string is integer -strict [lindex $argLine $i]]) && ([expr [lindex $argLine $i] >= 0]) } then {
                        		set TConfVars::SDL [lindex $argLine $i]
				} else {
					puts "ERROR: command-line: invalid value for switch '-sdl'"
                                	exit 1
				}
			} else {
				set TConfVars::SDL 1
				incr i -1
			}
                    }
		    "^\[-/\]reset$" {
                        incr i
			if { ($i < $argNumb) && ([string index [lindex $argLine $i] 0] != "-") } then {
                            set TConfVars::Reset [lindex $argLine $i]
                        } else {
                            puts "ERROR: command-line: data expected for switch '-reset'"
                            exit 1
                        }
		    }
                    "^\[-/\]env$" {
                        incr i
			if { ($i < $argNumb) && ([string index [lindex $argLine $i] 0] != "-") } then {
                            set TConfVars::DefaultEnv [lindex $argLine $i]
                        } else {
                            puts "ERROR: command-line: data expected for switch '-env'"
                            exit 1
                        }
                    }
                    "^\[-/\]model$" {
                        incr i
			if { ($i < $argNumb) && ([string index [lindex $argLine $i] 0] != "-") } then {
                            set TConfVars::DefaultModel [lindex $argLine $i]
                        } else {
                            puts "ERROR: command-line: data expected for switch '-model'"
                            exit 1
                        }
                    }           
                    default {
                        puts "ERROR: command-line: unknown parameter: '$arg'"
                        printHelp
                        exit 1
                    }
                }
            }
        }
        if { ([llength $TConfVars::genInclWrapper] != 0) && ($TConfVars::genWrapper == "") } then {
            puts "ERROR: command-line: switch '-iwrap' shouldn`t be used without switch '-wrap'"
            exit 1
        }
        if { $ATSFlag == 0 } then {
            puts "ERROR: command-line: invalid value for switch '-ats'"
            exit 1
        }
        unset ATSFlag
    }
}

#########################################################################
# Procedure : StartTestSuite
#
# Description: Generates the beginning of a test suite
# 
# Returns: None
# 
# Notes: Opens test suite files
# 
#########################################################################
proc StartTestSuite { testSuiteName args } {

    #create working directory
    if { [catch { file mkdir $TConfVars::outputDir } res ] } then {
        puts "ERROR: file: '$TConfVars::outputDir': can't be created\n$res"
        exit 1
    }

	#create generation log file
	if { $TConfVars::templLog == 1 } then {
	    if { [catch {set TCommProc::tLogFile [open [file join $TConfVars::outputDir gen.log] w] } res ] } then {
	        puts "ERROR: file: '[file join $TConfVars::outputDir gen.log]': can't be open for writing\n$res"
	        exit 1
	    }
	} else {
	    set TCommProc::tLogFile stdout
	}
	#save current test suite name
        set TCommProc::testSuite $testSuiteName

	#generate common code
	TCommProc::genCommonCode $testSuiteName
	
	#start test suite
        TCommProc::stTestSuite $testSuiteName
}

#########################################################################
# Procedure : GenDumpFunction
# Generate dumping function for a user-defined data type
# Parameters:
# index - data type ID
# sCommFile - output channel
#########################################################################
proc GenDumpFunction { index sCommFile } {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_TYPE
	global DATATYPE_DIMENSION
	global DATATYPE_STYPE
	global DATATYPE_ATTR
	global DATATYPE_ENUM
	global DATATYPE_PARAMTYPE
	global DATATYPE_DATACLASS
	global DATATYPE_MEMBERS
	global DATATYPE_RECORD_SIZE
	
	set name [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
	set attr [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_ATTR]]
	set type [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_TYPE]]
	set dim  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_DIMENSION]]
	set stype  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_STYPE]]
	set paramtype  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_PARAMTYPE]]
	set members  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_MEMBERS]]
	
	#if function is defined by user, return
	if {[lindex $attr 0] != ""} { return }

	# Added to provide SDL support
	if {$paramtype=="SDL"} {
		GenSDLDumpFunction $index $sCommFile
		return
	}

	puts $sCommFile "int TAT_Dump_${name}(void *data, char *dest) \{"
	puts $sCommFile "\tint i,j;"

	if {$dim=="var"} {
		puts $sCommFile "\tstrcat(dest,\"(: \");"
		puts $sCommFile "\tfor(i=0;i<((${name} *)data)->counter;i++) \{"
		if {![string compare [string tolower $type] "struct"]} {
			puts $sCommFile "\t\tstrcat(dest,\"(. \");"
			set mcount 0
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mdim [lindex $member $DATATYPE_DIMENSION]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mdim=="var"} {
					puts $sCommFile "\t\tstrcat(dest,\"(: \");"
					puts $sCommFile "\t\tfor(j=0;j<(($name *)data)->data\[i\].$mname.counter;j++) \{"
					puts $sCommFile "\t\t\t[TCommProc::dataOutFunc $mtype $mstype](&((($name *)data)->data\[i\].${mname}.data\[j\]),dest);"
					puts $sCommFile "\t\t\tif(j<(($name *)data)->data\[i\].${mname}.counter-1) strcat(dest,\", \");"
					puts $sCommFile "\t\t\}"
					puts $sCommFile "\t\tstrcat(dest,\" :)\");"
				} elseif {[string is digit -strict $mdim]} {
					puts $sCommFile "\t\tstrcat(dest,\"(: \");"
					puts $sCommFile "\t\tfor(j=0;j<${mdim};j++) \{"
					puts $sCommFile "\t\t\t[TCommProc::dataOutFunc $mtype $mstype](&((($name *)data)->data\[i\].${mname}\[j\]),dest);"
					puts $sCommFile "\t\t\tif(j<[expr $mdim - 1]) strcat(dest,\", \");"
					puts $sCommFile "\t\t\}"
					puts $sCommFile "\t\tstrcat(dest,\" :)\");"
				} elseif {$mdim==""} {
					puts $sCommFile "\t\t[TCommProc::dataOutFunc $mtype $mstype](&((($name *)data)->data\[i\].${mname}),dest);"
				} else {TCommProc::genAbort "${name}.${mname}: wrong member dimension"}
				incr mcount
				if {$mcount<[llength $members]} {puts $sCommFile "\t\tstrcat(dest,\", \");"}
			}
			puts $sCommFile "\t\tstrcat(dest,\" .)\");"
		} else {
			puts $sCommFile "\t\t[TCommProc::dataOutFunc $type $stype](&((($name *)data)->data\[i\]),dest);"
		}
		puts $sCommFile "\t\tif(i<(($name *)data)->counter-1) strcat(dest,\", \");"
		puts $sCommFile "\t\}"
		puts $sCommFile "\tstrcat(dest,\" :)\");"
	} elseif {[string is digit -strict $dim]} {
		puts $sCommFile "\tstrcat(dest,\"(: \");"
		puts $sCommFile "\tfor(i=0;i<${dim};i++) \{"
		if {![string compare [string tolower $type] struct]} {
			puts $sCommFile "\t\tstrcat(dest,\"(. \");"
			set mcount 0
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mdim [lindex $member $DATATYPE_DIMENSION]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mdim=="var"} {
					puts $sCommFile "\t\tstrcat(dest,\"(: \");"
					puts $sCommFile "\t\tfor(j=0;j<(($name *)data)->data\[i\].${mname}.counter;j++) \{"
					puts $sCommFile "\t\t\t[TCommProc::dataOutFunc $mtype $mstype](&((($name *)data)->data\[i\].${mname}.data\[j\]),dest);"
					puts $sCommFile "\t\t\tif(j<(($name *)data)->data\[i\].${mname}.counter-1) strcat(dest,\", \");"
					puts $sCommFile "\t\t\}"
					puts $sCommFile "\t\tstrcat(dest,\" :)\");"
				} elseif {[string is digit -strict $mdim]} {
					puts $sCommFile "\t\tstrcat(dest,\"(: \");"
					puts $sCommFile "\t\tfor(j=0;j<${mdim};j++) \{"
					puts $sCommFile "\t\t\t[TCommProc::dataOutFunc $mtype $mstype](&((($name *)data)->data\[i\].${mname}\[j\]),dest);"
					puts $sCommFile "\t\t\tif(j<[expr $mdim - 1]) strcat(dest,\", \");"
					puts $sCommFile "\t\t\}"
					puts $sCommFile "\t\tstrcat(dest,\" :)\");"
				} elseif {$mdim==""} {
					puts $sCommFile "\t\t[TCommProc::dataOutFunc $mtype $mstype](&((($name *)data)->data\[i\].${mname}),dest);"
				} else {TCommProc::genAbort "${name}.${mname}: wrong member dimension"}
				incr mcount
				if {$mcount<[llength $members]} {puts $sCommFile "\t\tstrcat(dest,\", \");"}
			}
			puts $sCommFile "\t\tstrcat(dest,\" .)\");"
		} else {
			puts $sCommFile "\t\t[TCommProc::dataOutFunc $type $stype](&((($name *)data)->data\[i\]),dest);"
		}
		puts $sCommFile "\t\tif(i<[expr $dim - 1]) strcat(dest,\", \");"
		puts $sCommFile "\t\}"
		puts $sCommFile "\tstrcat(dest,\" :)\");"
	} elseif {$dim==""} {
		if {![string compare [string tolower $type] struct]} {
			puts $sCommFile "\tstrcat(dest,\"(. \");"
			set mcount 0
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mdim [lindex $member $DATATYPE_DIMENSION]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mdim=="var"} {
					puts $sCommFile "\tstrcat(dest,\"(: \");"
					puts $sCommFile "\tfor(j=0;j<(($name *)data)->${mname}.counter;j++) \{"
					puts $sCommFile "\t\t[TCommProc::dataOutFunc $mtype $mstype](&((($name *)data)->${mname}.data\[j\]),dest);"
					puts $sCommFile "\t\tif(j<(($name *)data)->${mname}.counter-1) strcat(dest,\", \");"
					puts $sCommFile "\t\}"
					puts $sCommFile "\tstrcat(dest,\" :)\");"
				} elseif {[string is digit -strict $mdim]} {
					puts $sCommFile "\tstrcat(dest,\"(: \");"
					puts $sCommFile "\tfor(j=0;j<${mdim};j++) \{"
					puts $sCommFile "\t\t[TCommProc::dataOutFunc $mtype $mstype](&((($name *)data)->${mname}\[j\]),dest);"
					puts $sCommFile "\t\tif(j<[expr $mdim - 1]) strcat(dest,\", \");"
					puts $sCommFile "\t\}"
					puts $sCommFile "\tstrcat(dest,\" :)\");"
				} elseif {$mdim==""} {
					puts $sCommFile "\t[TCommProc::dataOutFunc $mtype $mstype](&((($name *)data)->${mname}),dest);"
				} else {TCommProc::genAbort "${name}.${mname}: wrong member dimension"}
				incr mcount
				if {$mcount<[llength $members]} {puts $sCommFile "\tstrcat(dest,\", \");"}
			}
			puts $sCommFile "\tstrcat(dest,\" .)\");"
		} else {
			puts $sCommFile "\t[TCommProc::dataOutFunc $type $stype](data,dest);"
		}
	} elseif {$dim=="list"} {
		puts $sCommFile "\t$name cur=*($name *)data;"
		puts $sCommFile "\tstrcat(dest,\"(: \");"
		puts $sCommFile "\twhile(cur) \{"
		if {![string compare [string tolower $type] "struct"]} {
			puts $sCommFile "\t\tstrcat(dest,\"(. \");"
			set mcount 0
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mdim [lindex $member $DATATYPE_DIMENSION]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mdim=="var"} {
					puts $sCommFile "\t\tstrcat(dest,\"(: \");"
					puts $sCommFile "\t\tfor(j=0;j<cur->data.$mname.counter;j++) \{"
					puts $sCommFile "\t\t\t[TCommProc::dataOutFunc $mtype $mstype](&(cur->data.${mname}.data\[j\]),dest);"
					puts $sCommFile "\t\t\tif(j<cur->data.${mname}.counter-1) strcat(dest,\", \");"
					puts $sCommFile "\t\t\}"
					puts $sCommFile "\t\tstrcat(dest,\" :)\");"
				} elseif {[string is digit -strict $mdim]} {
					puts $sCommFile "\t\tstrcat(dest,\"(: \");"
					puts $sCommFile "\t\tfor(j=0;j<${mdim};j++) \{"
					puts $sCommFile "\t\t\t[TCommProc::dataOutFunc $mtype $mstype](&(cur->data.${mname}\[j\]),dest);"
					puts $sCommFile "\t\t\tif(j<[expr $mdim - 1]) strcat(dest,\", \");"
					puts $sCommFile "\t\t\}"
					puts $sCommFile "\t\tstrcat(dest,\" :)\");"
				} elseif {$mdim==""} {
					puts $sCommFile "\t\t[TCommProc::dataOutFunc $mtype $mstype](&(cur->data.${mname}),dest);"
				} else {TCommProc::genAbort "${name}.${mname}: wrong member dimension"}
				incr mcount
				if {$mcount<[llength $members]} {puts $sCommFile "\t\tstrcat(dest,\", \");"}
			}
			puts $sCommFile "\t\tstrcat(dest,\" .)\");"
		} else {
			puts $sCommFile "\t\t[TCommProc::dataOutFunc $type $stype](&(cur->data),dest);"
		}
		puts $sCommFile "\t\tcur=cur->next;"
		puts $sCommFile "\t\tif(cur) strcat(dest,\", \");"
		puts $sCommFile "\t\}"
		puts $sCommFile "\tstrcat(dest,\" :)\");"
	} else {TCommProc::genAbort "${name}: wrong dimension"}
	puts $sCommFile "\treturn 1;"
	puts $sCommFile "\}"
}

#########################################################################
# Procedure : GenSDLDumpFunction
# Generate dumping function for SDL data type
# Parameters:
# index - data type ID
# sCommFile - output channel
#########################################################################
proc GenSDLDumpFunction { index sCommFile } {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_TYPE
	global DATATYPE_DIMENSION
	global DATATYPE_STYPE
	global DATATYPE_ATTR
	global DATATYPE_ENUM
	global DATATYPE_PARAMTYPE
	global DATATYPE_DATACLASS
	global DATATYPE_MEMBERS
	global DATATYPE_RECORD_SIZE
	
	set name [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
	set attr [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_ATTR]]
	set type [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_TYPE]]
	set dim  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_DIMENSION]]
	set stype  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_STYPE]]
	set paramtype  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_PARAMTYPE]]
	set members  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_MEMBERS]]

	puts $sCommFile "int TAT_Dump_${name}(void *data, char *dest) \{"
	
	if {[string is digit -strict $dim]} {
		puts $sCommFile "\tint i;"
		puts $sCommFile "\tstrcat(dest,\"(: \");"
		puts $sCommFile "\tfor(i=0;i<${dim};i++) \{"
		puts $sCommFile "\t\t[TCommProc::dataOutFunc $type $stype](&((($name *)data)->A\[i\]),dest);"
		puts $sCommFile "\t\tif(i<[expr $dim - 1]) strcat(dest,\", \");"
		puts $sCommFile "\t\}"
		puts $sCommFile "\tstrcat(dest,\" :)\");"
	} elseif {$dim=="string"} {
		puts $sCommFile "\t${name}_yrec * cur=((${name} *)data)->First;"
		puts $sCommFile "\tstrcat(dest,\"(: \");"
		puts $sCommFile "\twhile(cur) \{"
		puts $sCommFile "\t\t[TCommProc::dataOutFunc $type $stype](&(cur->Data),dest);"
		puts $sCommFile "\t\tcur=cur->Suc;"
		puts $sCommFile "\t\tif(cur) strcat(dest,\", \");"
		puts $sCommFile "\t\}"
		puts $sCommFile "\tstrcat(dest,\" :)\");"
	} elseif {$dim==""} {
		if {[string tolower $type]=="struct"} {
			puts $sCommFile "\tstrcat(dest,\"(. \");"
			set mcount 0
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mstype [lindex $member $DATATYPE_STYPE]
				puts $sCommFile "\t[TCommProc::dataOutFunc $mtype $mstype](&((($name *)data)->${mname}),dest);"
				incr mcount
				if {$mcount<[llength $members]} {puts $sCommFile "\tstrcat(dest,\", \");"}
			}
			puts $sCommFile "\tstrcat(dest,\" .)\");"
		} else {
			puts $sCommFile "\t[TCommProc::dataOutFunc $type $stype](data,dest);"
		}
	} else {
		TCommProc::genAbort "${name}: wrong SDL data type definition"
	}
	puts $sCommFile "\treturn 1;"
	puts $sCommFile "\}"
	return
}

#########################################################################
# Procedure : genDumpFunctions
# Generate dumping functions for all user-defined data types
# Parameters:
# cfile - output channel (source file)
# hfile - output channel (header file)
#########################################################################
proc genDumpFunctions {cfile hfile} {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_ATTR
	global DATATYPE_RECORD_SIZE

	set datacount [expr [llength $DataTypeSet] / $DATATYPE_RECORD_SIZE]

	#Generate prototypes in header file
	for {set i 0} {$i<$datacount} {incr i} {
		set name [lindex $DataTypeSet [expr $i * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
		set attr [lindex $DataTypeSet [expr $i * $DATATYPE_RECORD_SIZE + $DATATYPE_ATTR]]
		if {[lindex $attr 0]==""} {puts $hfile "int TAT_Dump_${name}(void *data, char *dest);"}
	}

	puts $hfile ""

	#Generate dumping functions in source file
	for {set i 0} {$i<$datacount} {incr i} {
		GenDumpFunction $i $cfile
		puts $cfile ""
	}
}	

#########################################################################
# Procedure : GenFreeFunction
# Generate free function for a user-defined data type
# Parameters:
# index - data type ID
# sCommFile - output channel
#########################################################################
proc GenFreeFunction { index sCommFile } {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_TYPE
	global DATATYPE_DIMENSION
	global DATATYPE_STYPE
	global DATATYPE_ATTR
	global DATATYPE_ENUM
	global DATATYPE_PARAMTYPE
	global DATATYPE_DATACLASS
	global DATATYPE_MEMBERS
	global DATATYPE_RECORD_SIZE
	
	set name [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
	set attr [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_ATTR]]
	set type [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_TYPE]]
	set dim  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_DIMENSION]]
	set stype  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_STYPE]]
	set paramtype  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_PARAMTYPE]]
	set members  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_MEMBERS]]
	
	# Added to provide SDL support
	if {$paramtype=="SDL"} {
		GenSDLFreeFunction $index $sCommFile
		return
	}
	
	puts $sCommFile "int TAT_Free_${name}($name * x) \{"
	puts $sCommFile "\tint i,j;"

	if {$dim=="var"} {
		puts $sCommFile "\tfor(i=0;i<x->counter;i++) \{"
		if {![string compare [string tolower $type] struct]} {
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mdim [lindex $member $DATATYPE_DIMENSION]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mdim=="var"} {
					if {$mstype!=-1} {
						puts $sCommFile "\t\tfor(j=0;j<x->data\[i\].$mname.counter;j++) \{"
						puts $sCommFile "\t\t\tTAT_Free_${mtype}(&(x->data\[i\].${mname}.data\[j\]));"
						puts $sCommFile "\t\t\}"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tfor(j=0;j<x->data\[i\].$mname.counter;j++) \{"
						puts $sCommFile "\t\t\tfree(x->data\[i\].${mname}.data\[j\]);"
						puts $sCommFile "\t\t\}"
					}
					puts $sCommFile "\t\tfree(x->data\[i\].$mname.data);"
				} elseif {[string is digit -strict $mdim]} {
					if {$mstype!=-1} {
						puts $sCommFile "\t\tfor(j=0;j<${mdim};j++) \{"
						puts $sCommFile "\t\t\tTAT_Free_${mtype}(&(x->data\[i\].${mname}\[j\]));"
						puts $sCommFile "\t\t\}"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tfor(j=0;j<${mdim};j++) \{"
						puts $sCommFile "\t\t\tfree(x->data\[i\].${mname}\[j\]);"
						puts $sCommFile "\t\t\}"
					}
				} elseif {$mdim==""} {
					if {$mstype!=-1} {
						puts $sCommFile "\t\tTAT_Free_${mtype}(&(x->data\[i\].${mname}));"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tfree(x->data\[i\].${mname});"
					}
				} else {TCommProc::genAbort "${name}.${mname}: wrong member dimension"}
			}
		} elseif {$stype!=-1} {
			puts $sCommFile "\t\tTAT_Free_${type}(&(x->data\[i\]));"
		} elseif {[regexp "char( |\t)*\\*" $type]} {
			puts $sCommFile "\t\tfree(x->data\[i\]);"
		}
		puts $sCommFile "\t\}"
		puts $sCommFile "\tfree(x->data);"
	} elseif {[string is digit -strict $dim]} {
		puts $sCommFile "\tfor(i=0;i<${dim};i++) \{"
		if {![string compare [string tolower $type] struct]} {
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mdim [lindex $member $DATATYPE_DIMENSION]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mdim=="var"} {
					if {$mstype!=-1} {
						puts $sCommFile "\t\tfor(j=0;j<x->data\[i\].$mname.counter;j++) \{"
						puts $sCommFile "\t\t\tTAT_Free_${mtype}(&(x->data\[i\].${mname}.data\[j\]));"
						puts $sCommFile "\t\t\}"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tfor(j=0;j<x->data\[i\].$mname.counter;j++) \{"
						puts $sCommFile "\t\t\tfree(x->data\[i\].${mname}.data\[j\]);"
						puts $sCommFile "\t\t\}"
					}
					puts $sCommFile "\t\tfree(x->data\[i\].$mname.data);"
				} elseif {[string is digit -strict $mdim]} {
					if {$mstype!=-1} {
						puts $sCommFile "\t\tfor(j=0;j<${mdim};j++) \{"
						puts $sCommFile "\t\t\tTAT_Free_${mtype}(&(x->data\[i\].${mname}\[j\]));"
						puts $sCommFile "\t\t\}"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tfor(j=0;j<${mdim};j++) \{"
						puts $sCommFile "\t\t\tfree(x->data\[i\].${mname}\[j\]);"
						puts $sCommFile "\t\t\}"
					}
				} elseif {$mdim==""} {
					if {$mstype!=-1} {
						puts $sCommFile "\t\tTAT_Free_${mtype}(&(x->data\[i\].${mname}));"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tfree(x->data\[i\].${mname});"
					}
				} else {TCommProc::genAbort "${name}.${mname}: wrong member dimension"}
			}
		} elseif {$stype!=-1} {
			puts $sCommFile "\t\tTAT_Free_${type}(&(x->data\[i\]));"
		} elseif {[regexp "char( |\t)*\\*" $type]} {
			puts $sCommFile "\t\tfree(x->data\[i\]);"
		}
		puts $sCommFile "\t\}"
	} elseif {$dim==""} {
		if {![string compare [string tolower $type] struct]} {
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mdim [lindex $member $DATATYPE_DIMENSION]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mdim=="var"} {
					if {$mstype!=-1} {
						puts $sCommFile "\tfor(j=0;j<x->$mname.counter;j++) \{"
						puts $sCommFile "\t\tTAT_Free_${mtype}(&(x->${mname}.data\[j\]));"
						puts $sCommFile "\t\}"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\tfor(j=0;j<x->$mname.counter;j++) \{"
						puts $sCommFile "\t\tfree(x->${mname}.data\[j\]);"
						puts $sCommFile "\t\}"
					}
					puts $sCommFile "\tfree(x->$mname.data);"
				} elseif {[string is digit -strict $mdim]} {
					if {$mstype!=-1} {
						puts $sCommFile "\tfor(j=0;j<${mdim};j++) \{"
						puts $sCommFile "\t\tTAT_Free_${mtype}(&(x->${mname}\[j\]));"
						puts $sCommFile "\t\}"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\tfor(j=0;j<${mdim};j++) \{"
						puts $sCommFile "\t\tfree(x->${mname}\[j\]);"
						puts $sCommFile "\t\}"
					}
				} elseif {$mdim==""} {
					if {$mstype!=-1} {
						puts $sCommFile "\tTAT_Free_${mtype}(&(x->${mname}));"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\tfree(x->${mname});"
					}
				} else {TCommProc::genAbort "${name}.${mname}: wrong member dimension"}
			}
		} elseif {$stype!=-1} {
			puts $sCommFile "\tTAT_Free_${type}(x);"
		} elseif {[regexp "char( |\t)*\\*" $type]} {
			puts $sCommFile "\tfree(*x);"
		}
	} elseif {$dim=="list"} {
		puts $sCommFile "\t$name cur=*x;"
		puts $sCommFile "\t$name prev;"
		puts $sCommFile "\twhile(cur) \{"
		if {![string compare [string tolower $type] struct]} {
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mdim [lindex $member $DATATYPE_DIMENSION]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mdim=="var"} {
					if {$mstype!=-1} {
						puts $sCommFile "\t\tfor(j=0;j<cur->data.$mname.counter;j++) \{"
						puts $sCommFile "\t\t\tTAT_Free_${mtype}(&(cur->data.${mname}.data\[j\]));"
						puts $sCommFile "\t\t\}"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tfor(j=0;j<cur->data.$mname.counter;j++) \{"
						puts $sCommFile "\t\t\tfree(cur->data.${mname}.data\[j\]);"
						puts $sCommFile "\t\t\}"
					}
					puts $sCommFile "\t\tfree(cur->data.$mname.data);"
				} elseif {[string is digit -strict $mdim]} {
					if {$mstype!=-1} {
						puts $sCommFile "\t\tfor(j=0;j<${mdim};j++) \{"
						puts $sCommFile "\t\t\tTAT_Free_${mtype}(&(cur->data.${mname}\[j\]));"
						puts $sCommFile "\t\t\}"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tfor(j=0;j<${mdim};j++) \{"
						puts $sCommFile "\t\t\tfree(cur->data.${mname}\[j\]);"
						puts $sCommFile "\t\t\}"
					}
				} elseif {$mdim==""} {
					if {$mstype!=-1} {
						puts $sCommFile "\t\tTAT_Free_${mtype}(&(cur->data.${mname}));"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tfree(cur->data.${mname});"
					}
				} else {TCommProc::genAbort "${name}.${mname}: wrong member dimension"}
			}
		} elseif {$stype!=-1} {
			puts $sCommFile "\t\tTAT_Free_${type}(&(cur->data));"
		} elseif {[regexp "char( |\t)*\\*" $type]} {
			puts $sCommFile "\t\tfree(cur->data);"
		}
		puts $sCommFile "\t\tprev=cur;"
		puts $sCommFile "\t\tcur=cur->next;"
		puts $sCommFile "\t\tfree(prev);"
		puts $sCommFile "\t\}"
	} else {TCommProc::genAbort "${name}: wrong dimension"}
	puts $sCommFile "\tmemset(x,0,sizeof(${name}));"
	puts $sCommFile "\treturn 1;"
	puts $sCommFile "\}"
}

#########################################################################
# Procedure : GenSDLFreeFunction
# Generate free function for SDL data type
# Parameters:
# index - data type ID
# sCommFile - output channel
#########################################################################
proc GenSDLFreeFunction { index sCommFile } {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_TYPE
	global DATATYPE_DIMENSION
	global DATATYPE_STYPE
	global DATATYPE_ATTR
	global DATATYPE_ENUM
	global DATATYPE_PARAMTYPE
	global DATATYPE_DATACLASS
	global DATATYPE_MEMBERS
	global DATATYPE_RECORD_SIZE
	
	set name [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
	set attr [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_ATTR]]
	set type [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_TYPE]]
	set dim  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_DIMENSION]]
	set stype  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_STYPE]]
	set paramtype  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_PARAMTYPE]]
	set members  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_MEMBERS]]

	puts $sCommFile "int TAT_Free_${name}($name * x) \{"
	
	if {[string is digit -strict $dim]} {
		puts $sCommFile "\tint i;"
		puts $sCommFile "\tfor(i=0;i<${dim};i++) \{"
		if {$stype!=-1} {
			puts $sCommFile "\t\tTAT_Free_${type}(&(x->A\[i\]));"
		} elseif {$type=="char*" || $type=="char *" || $type=="SDL_Charstring" || $type=="SDL_charstring"} {
			puts $sCommFile "\t\tfree(x->A\[i\]);"
		}
		puts $sCommFile "\t\}"
	} elseif {$dim=="string"} {
		puts $sCommFile "\t${name}_yrec * cur=x->First;"
		puts $sCommFile "\t${name}_yrec * prev;"
		puts $sCommFile "\twhile(cur) \{"
		if {$stype!=-1} {
			puts $sCommFile "\t\tTAT_Free_${type}(&(cur->Data));"
		} elseif {$type=="char*" || $type=="char *" || $type=="SDL_Charstring" || $type=="SDL_charstring"} {
			puts $sCommFile "\t\tfree(cur->Data);"
		}
		puts $sCommFile "\t\tprev=cur;"
		puts $sCommFile "\t\tcur=cur->Suc;"
		puts $sCommFile "\t\tfree(prev);"
		puts $sCommFile "\t\}"
	} elseif {$dim==""} {
		if {[string tolower $type]=="struct"} {
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mstype!=-1} {
					puts $sCommFile "\tTAT_Free_${mtype}(&(x->${mname}));"
				} elseif {$mtype=="char*" || $mtype=="char *" || $mtype=="SDL_Charstring" || $mtype=="SDL_charstring"} {
					puts $sCommFile "\tfree(x->${mname});"
				}
			}
		} else {
			if {$stype!=-1} {
				puts $sCommFile "\tTAT_Free_${type}(x);"
			} elseif {$type=="char*" || $type=="char *" || $type=="SDL_Charstring" || $type=="SDL_charstring"} {
				puts $sCommFile "\tfree(*x);"
			}
		}
	} else {
		TCommProc::genAbort "${name}: wrong SDL data type definition"
	}
	puts $sCommFile "\tmemset(x,0,sizeof(${name}));"
	puts $sCommFile "\treturn 1;"
	puts $sCommFile "\}"
	return
}

#########################################################################
# Procedure : genFreeFunctions
# Generate free functions for all user-defined data types
# Parameters:
# cfile - output channel (source file)
# hfile - output channel (header file)
#########################################################################
proc genFreeFunctions {cfile hfile} {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_RECORD_SIZE

	set datacount [expr [llength $DataTypeSet] / $DATATYPE_RECORD_SIZE]

	#Generate prototypes in header file
	for {set i 0} {$i<$datacount} {incr i} {
		set name [lindex $DataTypeSet [expr $i * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
		puts $hfile "int TAT_Free_${name}($name * x);"
	}

	puts $hfile ""

	#Generate free functions in source file
	for {set i 0} {$i<$datacount} {incr i} {
		GenFreeFunction $i $cfile
		puts $cfile ""
	}
}

#########################################################################
# Procedure : GenCopyFunction
# Generate copy function for a user-defined data type
# Parameters:
# index - data type ID
# sCommFile - output channel
#########################################################################
proc GenCopyFunction { index sCommFile } {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_TYPE
	global DATATYPE_DIMENSION
	global DATATYPE_STYPE
	global DATATYPE_ATTR
	global DATATYPE_ENUM
	global DATATYPE_PARAMTYPE
	global DATATYPE_DATACLASS
	global DATATYPE_MEMBERS
	global DATATYPE_RECORD_SIZE
	
	set name [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
	set attr [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_ATTR]]
	set type [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_TYPE]]
	set dim  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_DIMENSION]]
	set stype  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_STYPE]]
	set paramtype  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_PARAMTYPE]]
	set members  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_MEMBERS]]
	
	# Added to provide SDL support
	if {$paramtype=="SDL"} {
		GenSDLCopyFunction $index $sCommFile
		return
	}
	
	puts $sCommFile "$name TAT_Copy_${name}($name src) \{"
	puts $sCommFile "\tint i,j;"
	puts $sCommFile "\t$name x;"

	if {$dim=="var"} {
		puts $sCommFile "\tx.counter=src.counter;"
		if {[llength $members]>0} {
			set ptype "_$name"
		} else {
			set ptype $type
		}
		puts $sCommFile "\tif(x.counter>0)"
		puts $sCommFile "\t\tx.data=(${ptype}*)malloc(x.counter*sizeof(${ptype}));"
		puts $sCommFile "\telse"
		puts $sCommFile "\t\tx.data=0;"
		puts $sCommFile "\tfor(i=0;i<x.counter;i++) \{"
		if {![string compare [string tolower $type] struct]} {
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mdim [lindex $member $DATATYPE_DIMENSION]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mdim=="var"} {
					puts $sCommFile "\t\tx.data\[i\].$mname.counter=src.data\[i\].$mname.counter;"
					puts $sCommFile "\t\tif(x.data\[i\].$mname.counter>0)"
					puts $sCommFile "\t\t\tx.data\[i\].$mname.data=(${mtype}*)malloc(x.data\[i\].$mname.counter*sizeof(${mtype}));"
					puts $sCommFile "\t\telse"
					puts $sCommFile "\t\t\tx.data\[i\].$mname.data=0;"
					puts $sCommFile "\t\tfor(j=0;j<x.data\[i\].$mname.counter;j++) \{"
					if {$mstype!=-1} {
						puts $sCommFile "\t\t\tx.data\[i\].$mname.data\[j\]=TAT_Copy_${mtype}(src.data\[i\].${mname}.data\[j\]);"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\t\tx.data\[i\].$mname.data\[j\]=(char*)malloc(strlen(src.data\[i\].$mname.data\[j\])+1);"
						puts $sCommFile "\t\t\tstrcpy(x.data\[i\].$mname.data\[j\],src.data\[i\].$mname.data\[j\]);"
					} else {
						puts $sCommFile "\t\t\tx.data\[i\].$mname.data\[j\]=src.data\[i\].$mname.data\[j\];"
					}
					puts $sCommFile "\t\t\}"

				} elseif {[string is digit -strict $mdim]} {
					puts $sCommFile "\t\tfor(j=0;j<${mdim};j++) \{"
					if {$mstype!=-1} {
						puts $sCommFile "\t\t\tx.data\[i\].$mname\[j\]=TAT_Copy_${mtype}(src.data\[i\].${mname}\[j\]);"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\t\tx.data\[i\].$mname\[j\]=(char*)malloc(strlen(src.data\[i\].$mname\[j\])+1);"
						puts $sCommFile "\t\t\tstrcpy(x.data\[i\].$mname\[j\],src.data\[i\].$mname\[j\]);"
					} else {
						puts $sCommFile "\t\t\tx.data\[i\].$mname\[j\]=src.data\[i\].$mname\[j\];"
					}
					puts $sCommFile "\t\t\}"
				} elseif {$mdim==""} {
					if {$mstype!=-1} {
						puts $sCommFile "\t\tx.data\[i\].$mname=TAT_Copy_${mtype}(src.data\[i\].${mname});"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tx.data\[i\].${mname}=(char*)malloc(strlen(src.data\[i\].$mname)+1);"
						puts $sCommFile "\t\tstrcpy(x.data\[i\].$mname,src.data\[i\].$mname);"
					} else {
						puts $sCommFile "\t\tx.data\[i\].$mname=src.data\[i\].$mname;"
					}
				} else {TCommProc::genAbort "${name}.${mname}: wrong member dimension"}
			}
		} elseif {$stype!=-1} {
			puts $sCommFile "\t\tx.data\[i\]=TAT_Copy_${type}(src.data\[i\]);"
		} elseif {[regexp "char( |\t)*\\*" $type]} {
			puts $sCommFile "\t\tx.data\[i\]=(char*)malloc(strlen(src.data\[i\])+1);"
			puts $sCommFile "\t\tstrcpy(x.data\[i\],src.data\[i\]);"
		} else {
			puts $sCommFile "\t\tx.data\[i\]=src.data\[i\];"
		}
		puts $sCommFile "\t\}"
	} elseif {[string is digit -strict $dim]} {
		puts $sCommFile "\tfor(i=0;i<${dim};i++) \{"
		if {![string compare [string tolower $type] struct]} {
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mdim [lindex $member $DATATYPE_DIMENSION]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mdim=="var"} {
					puts $sCommFile "\t\tx.data\[i\].$mname.counter=src.data\[i\].$mname.counter;"
					puts $sCommFile "\t\tif(x.data\[i\].$mname.counter>0)"
					puts $sCommFile "\t\t\tx.data\[i\].$mname.data=(${mtype}*)malloc(x.data\[i\].$mname.counter*sizeof(${mtype}));"
					puts $sCommFile "\t\telse"
					puts $sCommFile "\t\t\tx.data\[i\].$mname.data=0;"
					puts $sCommFile "\t\tfor(j=0;j<x.data\[i\].$mname.counter;j++) \{"
					if {$mstype!=-1} {
						puts $sCommFile "\t\t\tx.data\[i\].$mname.data\[j\]=TAT_Copy_${mtype}(src.data\[i\].${mname}.data\[j\]);"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\t\tx.data\[i\].$mname.data\[j\]=(char*)malloc(strlen(src.data\[i\].$mname.data\[j\])+1);"
						puts $sCommFile "\t\t\tstrcpy(x.data\[i\].$mname.data\[j\],src.data\[i\].$mname.data\[j\]);"
					} else {
						puts $sCommFile "\t\t\tx.data\[i\].$mname.data\[j\]=src.data\[i\].$mname.data\[j\];"
					}
					puts $sCommFile "\t\t\}"

				} elseif {[string is digit -strict $mdim]} {
					puts $sCommFile "\t\tfor(j=0;j<${mdim};j++) \{"
					if {$mstype!=-1} {
						puts $sCommFile "\t\t\tx.data\[i\].$mname\[j\]=TAT_Copy_${mtype}(src.data\[i\].${mname}\[j\]);"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\t\tx.data\[i\].$mname\[j\]=(char*)malloc(strlen(src.data\[i\].$mname\[j\])+1);"
						puts $sCommFile "\t\t\tstrcpy(x.data\[i\].$mname\[j\],src.data\[i\].$mname\[j\]);"
					} else {
						puts $sCommFile "\t\t\tx.data\[i\].$mname\[j\]=src.data\[i\].$mname\[j\];"
					}
					puts $sCommFile "\t\t\}"
				} elseif {$mdim==""} {
					if {$mstype!=-1} {
						puts $sCommFile "\t\tx.data\[i\].$mname=TAT_Copy_${mtype}(src.data\[i\].${mname});"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tx.data\[i\].${mname}=(char*)malloc(strlen(src.data\[i\].$mname)+1);"
						puts $sCommFile "\t\tstrcpy(x.data\[i\].$mname,src.data\[i\].$mname);"
					} else {
						puts $sCommFile "\t\tx.data\[i\].$mname=src.data\[i\].$mname;"
					}
				} else {TCommProc::genAbort "${name}.${mname}: wrong member dimension"}
			}
		} elseif {$stype!=-1} {
			puts $sCommFile "\t\tx.data\[i\]=TAT_Copy_${type}(src.data\[i\]);"
		} elseif {[regexp "char( |\t)*\\*" $type]} {
			puts $sCommFile "\t\tx.data\[i\]=(char*)malloc(strlen(src.data\[i\])+1);"
			puts $sCommFile "\t\tstrcpy(x.data\[i\],src.data\[i\]);"
		} else {
			puts $sCommFile "\t\tx.data\[i\]=src.data\[i\];"
		}
		puts $sCommFile "\t\}"
	} elseif {$dim==""} {
		if {![string compare [string tolower $type] struct]} {
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mdim [lindex $member $DATATYPE_DIMENSION]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mdim=="var"} {
					puts $sCommFile "\tx.$mname.counter=src.$mname.counter;"
					puts $sCommFile "\tif(x.$mname.counter>0)"
					puts $sCommFile "\t\tx.$mname.data=(${mtype}*)malloc(x.$mname.counter*sizeof(${mtype}));"
					puts $sCommFile "\telse"
					puts $sCommFile "\t\tx.$mname.data=0;"
					puts $sCommFile "\tfor(j=0;j<x.$mname.counter;j++) \{"
					if {$mstype!=-1} {
						puts $sCommFile "\t\tx.$mname.data\[j\]=TAT_Copy_${mtype}(src.${mname}.data\[j\]);"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tx.$mname.data\[j\]=(char*)malloc(strlen(src.$mname.data\[j\])+1);"
						puts $sCommFile "\t\tstrcpy(x.$mname.data\[j\],src.$mname.data\[j\]);"
					} else {
						puts $sCommFile "\t\tx.$mname.data\[j\]=src.$mname.data\[j\];"
					}
					puts $sCommFile "\t\}"

				} elseif {[string is digit -strict $mdim]} {
					puts $sCommFile "\tfor(j=0;j<${mdim};j++) \{"
					if {$mstype!=-1} {
						puts $sCommFile "\t\tx.$mname\[j\]=TAT_Copy_${mtype}(src.${mname}\[j\]);"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tx.$mname\[j\]=(char*)malloc(strlen(src.$mname\[j\])+1);"
						puts $sCommFile "\t\tstrcpy(x.$mname\[j\],src.$mname\[j\]);"
					} else {
						puts $sCommFile "\t\tx.$mname\[j\]=src.$mname\[j\];"
					}
					puts $sCommFile "\t\}"
				} elseif {$mdim==""} {
					if {$mstype!=-1} {
						puts $sCommFile "\tx.$mname=TAT_Copy_${mtype}(src.${mname});"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\tx.${mname}=(char*)malloc(strlen(src.$mname)+1);"
						puts $sCommFile "\tstrcpy(x.$mname,src.$mname);"
					} else {
						puts $sCommFile "\tx.$mname=src.$mname;"
					}
				} else {TCommProc::genAbort "${name}.${mname}: wrong member dimension"}
			}
		} elseif {$stype!=-1} {
			puts $sCommFile "\tx=TAT_Copy_${type}(src);"
		} elseif {[regexp "char( |\t)*\\*" $type]} {
			puts $sCommFile "\tx=(char*)malloc(strlen(src)+1);"
			puts $sCommFile "\tstrcpy(x,src);"
		} else {
			puts $sCommFile "\tx=src;"
		}
	} elseif {$dim=="list"} {
		puts $sCommFile "\t$name cur,scur;"
		puts $sCommFile "\tif(src==0) return 0;"
		puts $sCommFile "\tx=($name)malloc(sizeof(struct _$name));"
		puts $sCommFile "\tcur=x;"
		puts $sCommFile "\tscur=src;"
		puts $sCommFile "\twhile(scur) \{"
		if {![string compare [string tolower $type] struct]} {
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mdim [lindex $member $DATATYPE_DIMENSION]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mdim=="var"} {
					puts $sCommFile "\t\tcur->data.$mname.counter=scur->data.$mname.counter;"
					puts $sCommFile "\t\tif(cur->data.$mname.counter>0)"
					puts $sCommFile "\t\t\tcur->data.$mname.data=(${mtype}*)malloc(cur->data.$mname.counter*sizeof(${mtype}));"
					puts $sCommFile "\t\telse"
					puts $sCommFile "\t\t\tcur->data.$mname.data=0;"
					puts $sCommFile "\t\tfor(j=0;j<cur->data.$mname.counter;j++) \{"
					if {$mstype!=-1} {
						puts $sCommFile "\t\t\tcur->data.$mname.data\[j\]=TAT_Copy_${mtype}(scur->data.${mname}.data\[j\]);"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\t\tcur->data.$mname.data\[j\]=(char*)malloc(strlen(scur->data.$mname.data\[j\])+1);"
						puts $sCommFile "\t\t\tstrcpy(cur->data.$mname.data\[j\],scur->data.$mname.data\[j\]);"
					} else {
						puts $sCommFile "\t\t\tcur->data.$mname.data\[j\]=scur->data.$mname.data\[j\];"
					}
					puts $sCommFile "\t\t\}"

				} elseif {[string is digit -strict $mdim]} {
					puts $sCommFile "\t\tfor(j=0;j<${mdim};j++) \{"
					if {$mstype!=-1} {
						puts $sCommFile "\t\t\tcur->data.$mname\[j\]=TAT_Copy_${mtype}(scur->data.${mname}\[j\]);"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\t\tcur->data.$mname\[j\]=(char*)malloc(strlen(scur->data.$mname\[j\])+1);"
						puts $sCommFile "\t\t\tstrcpy(cur->data.$mname\[j\],scur->data.$mname\[j\]);"
					} else {
						puts $sCommFile "\t\t\tcur->data.$mname\[j\]=scur->data.$mname\[j\];"
					}
					puts $sCommFile "\t\t\}"
				} elseif {$mdim==""} {
					if {$mstype!=-1} {
						puts $sCommFile "\t\tcur->data.$mname=TAT_Copy_${mtype}(scur->data.${mname});"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tcur->data.${mname}=(char*)malloc(strlen(scur->data.$mname)+1);"
						puts $sCommFile "\t\tstrcpy(cur->data.$mname,scur->data.$mname);"
					} else {
						puts $sCommFile "\t\tcur->data.$mname=scur->data.$mname;"
					}
				} else {TCommProc::genAbort "${name}.${mname}: wrong member dimension"}
			}
		} elseif {$stype!=-1} {
			puts $sCommFile "\t\tcur->data=TAT_Copy_${type}(scur->data);"
		} elseif {[regexp "char( |\t)*\\*" $type]} {
			puts $sCommFile "\t\tcur->data=(char*)malloc(strlen(scur->data)+1);"
			puts $sCommFile "\t\tstrcpy(cur->data,scur->data);"
		} else {
			puts $sCommFile "\t\tcur->data=scur->data;"
		}
		puts $sCommFile "\t\tscur=scur->next;"
		puts $sCommFile "\t\tif(scur)"
		puts $sCommFile "\t\t\tcur->next=($name)malloc(sizeof(struct _$name));"
		puts $sCommFile "\t\telse"
		puts $sCommFile "\t\t\tcur->next=0;"
		puts $sCommFile "\t\tcur=cur->next;"
		puts $sCommFile "\t\}"
	} else {TCommProc::genAbort "${name}: wrong dimension"}
	puts $sCommFile "\treturn x;"
	puts $sCommFile "\}"
}

#########################################################################
# Procedure : GenSDLCopyFunction
# Generate copy function for SDL data type
# Parameters:
# index - data type ID
# sCommFile - output channel
#########################################################################
proc GenSDLCopyFunction { index sCommFile } {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_TYPE
	global DATATYPE_DIMENSION
	global DATATYPE_STYPE
	global DATATYPE_ATTR
	global DATATYPE_ENUM
	global DATATYPE_PARAMTYPE
	global DATATYPE_DATACLASS
	global DATATYPE_MEMBERS
	global DATATYPE_RECORD_SIZE
	
	set name [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
	set attr [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_ATTR]]
	set type [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_TYPE]]
	set dim  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_DIMENSION]]
	set stype  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_STYPE]]
	set paramtype  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_PARAMTYPE]]
	set members  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_MEMBERS]]

	puts $sCommFile "$name TAT_Copy_${name}($name src) \{"
	puts $sCommFile "\t$name x;"
	
	if {[string is digit -strict $dim]} {
		puts $sCommFile "\tint i;"
		puts $sCommFile "\tfor(i=0;i<${dim};i++) \{"
		if {$stype!=-1} {
			puts $sCommFile "\t\tx.A\[i\]=TAT_Copy_${type}(src.A\[i\]);"
		} elseif {$type=="char*" || $type=="char *" || $type=="SDL_Charstring" || $type=="SDL_charstring"} {
			puts $sCommFile "\t\tx.A\[i\]=(char*)malloc(strlen(src.A\[i\])+1);"
			puts $sCommFile "\t\tstrcpy(x.A\[i\],src.A\[i\]);"
		} else {
			puts $sCommFile "\t\tx.A\[i\]=src.A\[i\];"
		}
		puts $sCommFile "\t\}"
	} elseif {$dim=="string"} {
		puts $sCommFile "\t${name}_yrec *cur, *scur;"
		puts $sCommFile "\tif(src.Length==0) \{"
		puts $sCommFile "\t\tx.First=NULL;"
		puts $sCommFile "\t\tx.Last=NULL;"
		puts $sCommFile "\t\tx.Length=0;"
		puts $sCommFile "\t\tx.IsAssigned=1;"
		puts $sCommFile "\t\treturn x;"
		puts $sCommFile "\t\}"
		puts $sCommFile "\tx.First=(${name}_yrec *)malloc(sizeof(${name}_yrec));"
		puts $sCommFile "\tx.Last=x.First;"
		puts $sCommFile "\tx.Length=src.Length;"
		puts $sCommFile "\tx.IsAssigned=1;"
		puts $sCommFile "\tcur=x.First;"
		puts $sCommFile "\tscur=src.First;"
		puts $sCommFile "\twhile(scur) \{"
		if {$stype!=-1} {
			puts $sCommFile "\t\tcur->Data=TAT_Copy_${type}(scur->Data);"
		} elseif {$type=="char*" || $type=="char *" || $type=="SDL_Charstring" || $type=="SDL_charstring"} {
			puts $sCommFile "\t\tcur->Data=(char*)malloc(strlen(scur->Data)+1);"
			puts $sCommFile "\t\tstrcpy(cur->Data,scur->Data);"
		} else {
			puts $sCommFile "\t\tcur->Data=scur->Data;"
		}
		puts $sCommFile "\t\tscur=scur->Suc;"
		puts $sCommFile "\t\tif(scur) \{"
		puts $sCommFile "\t\t\tcur->Suc=(${name}_yrec *)malloc(sizeof(${name}_yrec));"
		puts $sCommFile "\t\t\tcur=cur->Suc;"
		puts $sCommFile "\t\t\} else \{"
		puts $sCommFile "\t\t\tcur->Suc=NULL;"
		puts $sCommFile "\t\t\tx.Last=cur;"
		puts $sCommFile "\t\t\}"
		puts $sCommFile "\t\}"
	} elseif {$dim==""} {
		if {[string tolower $type]=="struct"} {
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mstype!=-1} {
					puts $sCommFile "\tx.$mname=TAT_Copy_${mtype}(src.${mname});"
				} elseif {$mtype=="char*" || $mtype=="char *" || $mtype=="SDL_Charstring" || $mtype=="SDL_charstring"} {
					puts $sCommFile "\tx.${mname}=(char*)malloc(strlen(src.$mname)+1);"
					puts $sCommFile "\tstrcpy(x.$mname,src.$mname);"
				} else {
					puts $sCommFile "\tx.$mname=src.$mname;"
				}
			}
		} else {
			if {$stype!=-1} {
				puts $sCommFile "\tx=TAT_Copy_${type}(src);"
			} elseif {$type=="char*" || $type=="char *" || $type=="SDL_Charstring" || $type=="SDL_charstring"} {
				puts $sCommFile "\tx=(char*)malloc(strlen(src)+1);"
				puts $sCommFile "\tstrcpy(x,src);"
			} else {
				puts $sCommFile "\tx=src;"
			}
		}
	} else {
		TCommProc::genAbort "${name}: wrong SDL data type definition"
	}
	puts $sCommFile "\treturn x;"
	puts $sCommFile "\}"
	return
}

#########################################################################
# Procedure : genCopyFunctions
# Generate copy functions for all user-defined data types
# Parameters:
# cfile - output channel (source file)
# hfile - output channel (header file)
#########################################################################
proc genCopyFunctions {cfile hfile} {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_RECORD_SIZE

	set datacount [expr [llength $DataTypeSet] / $DATATYPE_RECORD_SIZE]

	#Generate prototypes in header file
	for {set i 0} {$i<$datacount} {incr i} {
		set name [lindex $DataTypeSet [expr $i * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
		puts $hfile "$name TAT_Copy_${name}($name src);"
	}

	puts $hfile ""

	#Generate copy functions in source file
	for {set i 0} {$i<$datacount} {incr i} {
		GenCopyFunction $i $cfile
		puts $cfile ""
	}
}

#########################################################################
# Procedure : GenCompareFunction
# Generate compare function for a user-defined data type
# Parameters:
# index - data type ID
# sCommFile - output channel
#########################################################################
proc GenCompareFunction { index sCommFile } {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_TYPE
	global DATATYPE_DIMENSION
	global DATATYPE_STYPE
	global DATATYPE_ATTR
	global DATATYPE_ENUM
	global DATATYPE_PARAMTYPE
	global DATATYPE_DATACLASS
	global DATATYPE_MEMBERS
	global DATATYPE_RECORD_SIZE
	
	set name [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
	set attr [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_ATTR]]
	set type [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_TYPE]]
	set dim  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_DIMENSION]]
	set stype  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_STYPE]]
	set paramtype  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_PARAMTYPE]]
	set members  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_MEMBERS]]
	
	# Added to provide SDL support
	if {$paramtype=="SDL"} {
		GenSDLCompareFunction $index $sCommFile
		return
	}
	
	puts $sCommFile "int TAT_Compare_${name}($name x1, $name x2) \{"
	puts $sCommFile "\tint i,j;"
	
	if {$dim=="var"} {
		puts $sCommFile "\tif(x1.counter!=x2.counter) return 1;"
		puts $sCommFile "\tfor(i=0;i<x1.counter;i++) \{"
		if {![string compare [string tolower $type] struct]} {
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mdim [lindex $member $DATATYPE_DIMENSION]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mdim=="var"} {
					puts $sCommFile "\t\tif(x1.data\[i\].$mname.counter!=x2.data\[i\].$mname.counter) return 1;"
					puts $sCommFile "\t\tfor(j=0;j<x1.data\[i\].$mname.counter;j++) \{"
					if {$mstype!=-1} {
						puts $sCommFile "\t\t\tif(TAT_Compare_${mtype}(x1.data\[i\].${mname}.data\[j\],x2.data\[i\].${mname}.data\[j\])) return 1;"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\t\tif(strcmp(x1.data\[i\].$mname.data\[j\],x2.data\[i\].$mname.data\[j\])) return 1;"
					} else {
						puts $sCommFile "\t\t\tif(x1.data\[i\].$mname.data\[j\]!=x2.data\[i\].$mname.data\[j\]) return 1;"
					}
					puts $sCommFile "\t\t\}"

				} elseif {[string is digit -strict $mdim]} {
					puts $sCommFile "\t\tfor(j=0;j<${mdim};j++) \{"
					if {$mstype!=-1} {
						puts $sCommFile "\t\t\tif(TAT_Compare_${mtype}(x1.data\[i\].${mname}\[j\],x2.data\[i\].${mname}\[j\])) return 1;"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\t\tif(strcmp(x1.data\[i\].$mname\[j\],x2.data\[i\].$mname\[j\])) return 1;"
					} else {
						puts $sCommFile "\t\t\tif(x1.data\[i\].$mname\[j\]!=x2.data\[i\].$mname\[j\]) return 1;"
					}
					puts $sCommFile "\t\t\}"
				} elseif {$mdim==""} {
					if {$mstype!=-1} {
						puts $sCommFile "\t\tif(TAT_Compare_${mtype}(x1.data\[i\].${mname},x2.data\[i\].${mname})) return 1;"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tif(strcmp(x1.data\[i\].$mname,x2.data\[i\].$mname)) return 1;"
					} else {
						puts $sCommFile "\t\tif(x1.data\[i\].$mname!=x2.data\[i\].$mname) return 1;"
					}
				} else {TCommProc::genAbort "${name}.${mname}: wrong member dimension"}
			}
		} elseif {$stype!=-1} {
			puts $sCommFile "\t\tif(TAT_Compare_${type}(x1.data\[i\],x2.data\[i\])) return 1;"
		} elseif {[regexp "char( |\t)*\\*" $type]} {
			puts $sCommFile "\t\tif(strcmp(x1.data\[i\],x2.data\[i\])) return 1;"
		} else {
			puts $sCommFile "\t\tif(x1.data\[i\]!=x2.data\[i\]) return 1;"
		}
		puts $sCommFile "\t\}"
	} elseif {[string is digit -strict $dim]} {
		puts $sCommFile "\tfor(i=0;i<${dim};i++) \{"
		if {![string compare [string tolower $type] struct]} {
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mdim [lindex $member $DATATYPE_DIMENSION]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mdim=="var"} {
					puts $sCommFile "\t\tif(x1.data\[i\].$mname.counter!=x2.data\[i\].$mname.counter) return 1;"
					puts $sCommFile "\t\tfor(j=0;j<x1.data\[i\].$mname.counter;j++) \{"
					if {$mstype!=-1} {
						puts $sCommFile "\t\t\tif(TAT_Compare_${mtype}(x1.data\[i\].${mname}.data\[j\],x2.data\[i\].${mname}.data\[j\])) return 1;"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\t\tif(strcmp(x1.data\[i\].$mname.data\[j\],x2.data\[i\].$mname.data\[j\])) return 1;"
					} else {
						puts $sCommFile "\t\t\tif(x1.data\[i\].$mname.data\[j\]!=x2.data\[i\].$mname.data\[j\]) return 1;"
					}
					puts $sCommFile "\t\t\}"

				} elseif {[string is digit -strict $mdim]} {
					puts $sCommFile "\t\tfor(j=0;j<${mdim};j++) \{"
					if {$mstype!=-1} {
						puts $sCommFile "\t\t\tif(TAT_Compare_${mtype}(x1.data\[i\].${mname}\[j\],x2.data\[i\].${mname}\[j\])) return 1;"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\t\tif(strcmp(x1.data\[i\].$mname\[j\],x2.data\[i\].$mname\[j\])) return 1;"
					} else {
						puts $sCommFile "\t\t\tif(x1.data\[i\].$mname\[j\]!=x2.data\[i\].$mname\[j\]) return 1;"
					}
					puts $sCommFile "\t\t\}"
				} elseif {$mdim==""} {
					if {$mstype!=-1} {
						puts $sCommFile "\t\tif(TAT_Compare_${mtype}(x1.data\[i\].${mname},x2.data\[i\].${mname})) return 1;"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tif(strcmp(x1.data\[i\].$mname,x2.data\[i\].$mname)) return 1;"
					} else {
						puts $sCommFile "\t\tif(x1.data\[i\].$mname!=x2.data\[i\].$mname) return 1;"
					}
				} else {TCommProc::genAbort "w${name}.${mname}: wrong member dimension"}
			}
		} elseif {$stype!=-1} {
			puts $sCommFile "\t\tif(TAT_Compare_${type}(x1.data\[i\],x2.data\[i\])) return 1;"
		} elseif {[regexp "char( |\t)*\\*" $type]} {
			puts $sCommFile "\t\tif(strcmp(x1.data\[i\],x2.data\[i\])) return 1;"
		} else {
			puts $sCommFile "\t\tif(x1.data\[i\]!=x2.data\[i\]) return 1;"
		}
		puts $sCommFile "\t\}"
	} elseif {$dim==""} {
		if {![string compare [string tolower $type] struct]} {
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mdim [lindex $member $DATATYPE_DIMENSION]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mdim=="var"} {
					puts $sCommFile "\tif(x1.$mname.counter!=x2.$mname.counter) return 1;"
					puts $sCommFile "\tfor(j=0;j<x1.$mname.counter;j++) \{"
					if {$mstype!=-1} {
						puts $sCommFile "\t\tif(TAT_Compare_${mtype}(x1.${mname}.data\[j\],x2.${mname}.data\[j\])) return 1;"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tif(strcmp(x1.$mname.data\[j\],x2.$mname.data\[j\])) return 1;"
					} else {
						puts $sCommFile "\t\tif(x1.$mname.data\[j\]!=x2.$mname.data\[j\]) return 1;"
					}
					puts $sCommFile "\t\}"

				} elseif {[string is digit -strict $mdim]} {
					puts $sCommFile "\tfor(j=0;j<${mdim};j++) \{"
					if {$mstype!=-1} {
						puts $sCommFile "\t\tif(TAT_Compare_${mtype}(x1.${mname}\[j\],x2.${mname}\[j\])) return 1;"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tif(strcmp(x1.$mname\[j\],x2.$mname\[j\])) return 1;"
					} else {
						puts $sCommFile "\t\tif(x1.$mname\[j\]!=x2.$mname\[j\]) return 1;"
					}
					puts $sCommFile "\t\}"
				} elseif {$mdim==""} {
					if {$mstype!=-1} {
						puts $sCommFile "\tif(TAT_Compare_${mtype}(x1.${mname},x2.${mname})) return 1;"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\tif(strcmp(x1.$mname,x2.$mname)) return 1;"
					} else {
						puts $sCommFile "\tif(x1.$mname!=x2.$mname) return 1;"
					}
				} else {TCommProc::genAbort "${name}.${mname}: wrong member dimension"}
			}
		} elseif {$stype!=-1} {
			puts $sCommFile "\tif(TAT_Compare_${type}(x1,x2)) return 1;"
		} elseif {[regexp "char( |\t)*\\*" $type]} {
			puts $sCommFile "\tif(strcmp(x1,x2)) return 1;"
		} else {
			puts $sCommFile "\tif(x1!=x2) return 1;"
		}
	} elseif {$dim=="list"} {
		puts $sCommFile "\t$name cur1,cur2;"
		puts $sCommFile "\tif(x1==0) return (x2!=0);"
		puts $sCommFile "\tcur1=x1; cur2=x2;"
		puts $sCommFile "\twhile(cur1 && cur2) \{"
		if {![string compare [string tolower $type] struct]} {
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mdim [lindex $member $DATATYPE_DIMENSION]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mdim=="var"} {
					puts $sCommFile "\t\tif(cur1->data.$mname.counter!=cur2->data.$mname.counter) return 1;"
					puts $sCommFile "\t\tfor(j=0;j<cur1->data.$mname.counter;j++) \{"
					if {$mstype!=-1} {
						puts $sCommFile "\t\t\tif(TAT_Compare_${mtype}(cur1->data.${mname}.data\[j\],cur2->data.${mname}.data\[j\])) return 1;"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\t\tif(strcmp(cur1->data.$mname.data\[j\],cur2->data.$mname.data\[j\])) return 1;"
					} else {
						puts $sCommFile "\t\t\tif(cur1->data.$mname.data\[j\]!=cur2->data.$mname.data\[j\]) return 1;"
					}
					puts $sCommFile "\t\t\}"

				} elseif {[string is digit -strict $mdim]} {
					puts $sCommFile "\t\tfor(j=0;j<${mdim};j++) \{"
					if {$mstype!=-1} {
						puts $sCommFile "\t\t\tif(TAT_Compare_${mtype}(cur1->data.${mname}\[j\],cur2->data.${mname}\[j\])) return 1;"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\t\tif(strcmp(cur1->data.$mname\[j\],cur2->data.$mname\[j\])) return 1;"
					} else {
						puts $sCommFile "\t\t\tif(cur1->data.$mname\[j\]!=cur2->data.$mname\[j\]) return 1;"
					}
					puts $sCommFile "\t\t\}"
				} elseif {$mdim==""} {
					if {$mstype!=-1} {
						puts $sCommFile "\t\tif(TAT_Compare_${mtype}(cur1->data.${mname},cur2->data.${mname})) return 1;"
					} elseif {[regexp "char( |\t)*\\*" $mtype]} {
						puts $sCommFile "\t\tif(strcmp(cur1->data.$mname,cur2->data.$mname)) return 1;"
					} else {
						puts $sCommFile "\t\tif(cur1->data.$mname!=cur2->data.$mname) return 1;"
					}
				} else {TCommProc::genAbort "${name}.${mname}: wrong member dimension"}
			}
		} elseif {$stype!=-1} {
			puts $sCommFile "\t\tif(TAT_Compare_${type}(cur1->data,cur2->data)) return 1;"
		} elseif {[regexp "char( |\t)*\\*" $type]} {
			puts $sCommFile "\t\tif(strcmp(cur1->data,cur2->data)) return 1;"
		} else {
			puts $sCommFile "\t\tif(cur1->data!=cur2->data) return 1;"
		}
		puts $sCommFile "\t\tcur1=cur1->next;"
		puts $sCommFile "\t\tcur2=cur2->next;"
		puts $sCommFile "\t\tif(cur1==0) return (cur2!=0);"
		puts $sCommFile "\t\}"
	} else {TCommProc::genAbort "${name}: wrong dimension"}
	puts $sCommFile "\treturn 0;"
	puts $sCommFile "\}"
}

#########################################################################
# Procedure : GenSDLCompareFunction
# Generate compare function for SDL data type
# Parameters:
# index - data type ID
# sCommFile - output channel
#########################################################################
proc GenSDLCompareFunction { index sCommFile } {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_TYPE
	global DATATYPE_DIMENSION
	global DATATYPE_STYPE
	global DATATYPE_ATTR
	global DATATYPE_ENUM
	global DATATYPE_PARAMTYPE
	global DATATYPE_DATACLASS
	global DATATYPE_MEMBERS
	global DATATYPE_RECORD_SIZE
	
	set name [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
	set attr [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_ATTR]]
	set type [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_TYPE]]
	set dim  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_DIMENSION]]
	set stype  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_STYPE]]
	set paramtype  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_PARAMTYPE]]
	set members  [lindex $DataTypeSet [expr $index * $DATATYPE_RECORD_SIZE + $DATATYPE_MEMBERS]]

	puts $sCommFile "int TAT_Compare_${name}($name x1, $name x2) \{"
	
	if {[string is digit -strict $dim]} {
		puts $sCommFile "\tint i;"
		puts $sCommFile "\tfor(i=0;i<${dim};i++) \{"
		if {$stype!=-1} {
			puts $sCommFile "\t\tif(TAT_Compare_${type}(x1.A\[i\],x2.A\[i\])) return 1;"
		} elseif {$type=="char*" || $type=="char *" || $type=="SDL_Charstring" || $type=="SDL_charstring"} {
			puts $sCommFile "\t\tif(strcmp(x1.A\[i\],x2.A\[i\])) return 1;"
		} else {
			puts $sCommFile "\t\tif(x1.A\[i\]!=x2.A\[i\]) return 1;"
		}
		puts $sCommFile "\t\}"
	} elseif {$dim=="string"} {
		puts $sCommFile "\t${name}_yrec *cur1, *cur2;"
		puts $sCommFile "\tif(x1.Length==0) return (x2.Length!=0);"
		puts $sCommFile "\tcur1=x1.First; cur2=x2.First;"
		puts $sCommFile "\twhile(cur1 && cur2) \{"
		if {$stype!=-1} {
			puts $sCommFile "\t\tif(TAT_Compare_${type}(cur1->Data,cur2->Data)) return 1;"
		} elseif {$type=="char*" || $type=="char *" || $type=="SDL_Charstring" || $type=="SDL_charstring"} {
			puts $sCommFile "\t\tif(strcmp(cur1->Data,cur2->Data)) return 1;"
		} else {
			puts $sCommFile "\t\tif(cur1->Data!=cur2->Data) return 1;"
		}
		puts $sCommFile "\t\tcur1=cur1->Suc;"
		puts $sCommFile "\t\tcur2=cur2->Suc;"
		puts $sCommFile "\t\tif(cur1==NULL) return (cur2!=NULL);"
		puts $sCommFile "\t\}"
	} elseif {$dim==""} {
		if {[string tolower $type]=="struct"} {
			foreach member $members {
				set mname [lindex $member $DATATYPE_NAME]
				set mtype [lindex $member $DATATYPE_TYPE]
				set mstype [lindex $member $DATATYPE_STYPE]
				if {$mstype!=-1} {
					puts $sCommFile "\tif(TAT_Compare_${mtype}(x1.${mname},x2.${mname})) return 1;"
				} elseif {$mtype=="char*" || $mtype=="char *" || $mtype=="SDL_Charstring" || $mtype=="SDL_charstring"} {
					puts $sCommFile "\tif(strcmp(x1.$mname,x2.$mname)) return 1;"
				} else {
					puts $sCommFile "\tif(x1.$mname!=x2.$mname) return 1;"
				}
			}
		} else {
			if {$stype!=-1} {
				puts $sCommFile "\tif(TAT_Compare_${type}(x1,x2)) return 1;"
			} elseif {$type=="char*" || $type=="char *" || $type=="SDL_Charstring" || $type=="SDL_charstring"} {
				puts $sCommFile "\tif(strcmp(x1,x2)) return 1;"
			} else {
				puts $sCommFile "\tif(x1!=x2) return 1;"
			}
		}
	} else {
		TCommProc::genAbort "${name}.${mname}: wrong SDL data type definition"
	}
	puts $sCommFile "\treturn 0;"
	puts $sCommFile "\}"
	return
}

#########################################################################
# Procedure : genCompareFunctions
# Generate compare functions for all user-defined data types
# Parameters:
# cfile - output channel (source file)
# hfile - output channel (header file)
#########################################################################
proc genCompareFunctions {cfile hfile} {
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_RECORD_SIZE

	set datacount [expr [llength $DataTypeSet] / $DATATYPE_RECORD_SIZE]

	#Generate prototypes in header file
	for {set i 0} {$i<$datacount} {incr i} {
		set name [lindex $DataTypeSet [expr $i * $DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
		puts $hfile "int TAT_Compare_${name}($name x1, $name x2);"
	}

	#Generate compare functions in source file
	for {set i 0} {$i<$datacount} {incr i} {
		GenCompareFunction $i $cfile
		puts $cfile ""
	}
}

#########################################################################
# Procedure : StartTestCase
# 
# Description: Generate the beginning of a test case
# 
# Returns: None
# 
# Notes: None
# 
#########################################################################
proc StartTestCase { testCaseName testCaseRep paramLst1 timeLabelLst textCommentLst timerNumber branchNumber signalTextList\
                        varDefineLst srcRefLst  instSetLst eventLst mode loopCount args } {
    global InstanceSet INSTANCE_RECORD_SIZE SignalSet SIGNAL_NAME SIGNAL_RECORD_SIZE SIGNAL_PARAMS SIGNAL_FIELDS
    global ATS_EVENT_TYPE ATS_EVENT_TIME ATS_EVENT_OTIME ATS_EVENT_PTIME ATS_EVENT_COND ATS_EVENT_INST ATS_EVENT_VERD
    global ATS_EVENT_SREF ATS_EVENT_COMMENT ATS_EVENT_LABEL ATS_EVENT_DATA

	 #save test case data for further use
    set TCommProc::testCase $testCaseName
    set TCommProc::ATSmode $mode
    set TCommProc::labelList ""
    set TCommProc::stateFunc ""
    set TCommProc::varList ""    
    set TCommProc::evList $eventLst

    #update list of test cases for test table generation
    lappend TCommProc::testCount $testCaseName
    
    #create new file for test case, if needed
    if { $TConfVars::genMultiTC == 1 } {
            if { [catch { set TCommProc::tSourceFile  [open [file join $TConfVars::outputDir ${testCaseName}.c] w ]} res ] } then {
                TCommProc::genAbort "ERROR: file: '[file join $TConfVars::outputDir ${testCaseName}.c]': can not open for writing\n$res"
            }
        fconfigure $TCommProc::tSourceFile -buffersize 524288
        #puts $TCommProc::tSourceFile "#include <stdlib.h>\n"
        #puts $TCommProc::tSourceFile "#include <testkern.h>\n"
        puts $TCommProc::tSourceFile "#include \"${TCommProc::testSuite}.h\"\n"
    }
    
    puts $TCommProc::tLogFile "REMARK: Test case \"$testCaseName\" generation started"
    
	#begin#########################akk302c
	# Generate Compare and Assign functions for signal parameters
	global DataTypeSet
	global DATATYPE_NAME
	global DATATYPE_TYPE
	global DATATYPE_DIMENSION
	global DATATYPE_STYPE
	global DATATYPE_ATTR
	global DATATYPE_ENUM
	global DATATYPE_PARAMTYPE
	global DATATYPE_DATACLASS
	global DATATYPE_MEMBERS
	global DATATYPE_RECORD_SIZE

	global SignalSet
	global SIGNAL_NAME
	global SIGNAL_TYPE
	global SIGNAL_ATTR
	global SIGNAL_PARAMS
	global SIGNAL_STYPES
	global SIGNAL_FIELDS
	global SIGNAL_PARAMTYPE
	global SIGNAL_VARARG
	global SIGNAL_PRIORITY
	global SIGNAL_RECORD_SIZE

    #output signal texts, if it is unique for each test case
    if { $TConfVars::singleMT==0 } then {
        puts $TCommProc::tSourceFile "static char * tat_${TCommProc::testSuite}_${testCaseName}_MessageTable\[\] = \{"
        set count 0
        set messNum [llength $signalTextList]
        foreach {mess} $signalTextList {
            incr count
            if {$count == $messNum} then {
                puts $TCommProc::tSourceFile "\t[embedLine ${mess}]"
            } else {
                puts $TCommProc::tSourceFile "\t[embedLine ${mess}],"
            }
        }
        puts $TCommProc::tSourceFile "\};"
    }
    
    set varNum [llength $varDefineLst]
    if {$varNum > 0} then {
        puts $TCommProc::tSourceFile "static TAT_VARIABLE_DEF  tat_${TCommProc::testSuite}_${testCaseName}_Vardef\[\] = \{"
        set count 0
            foreach {var} $varDefineLst {
            incr count
            puts -nonewline $TCommProc::tSourceFile "\t\{$count, \"[lindex $var 1]\", sizeof([lindex $var 0]), [TCommProc::dataOutFunc [lindex $var 0] [lindex $var 2] ]\}"
            
            if {$count != $varNum} then {
                puts $TCommProc::tSourceFile ","
            }
            
            #add var name to the list
            lappend TCommProc::varList [lindex $var 1]
        }
        puts $TCommProc::tSourceFile "\};"
        
        set count 0
        foreach {var} $varDefineLst {
            #output define to simplify access to variables in test case
            puts $TCommProc::tSourceFile "#define var_${testCaseName}_[lindex $var 1] (*([lindex $var 0]*)id->vars\[$count\])"
            incr count
        }
    }
    
    #output time labels definitions
    set tmessNum [llength $timeLabelLst]
    set TCommProc::labelList ""
    if {$tmessNum > 0} then {
        puts $TCommProc::tSourceFile "static TAT_TIME_LABEL_DEF  tat_${TCommProc::testSuite}_${testCaseName}_TimeLabel\[\] = \{"
        set count 0
            foreach {mess} $timeLabelLst {
            incr count
            puts -nonewline $TCommProc::tSourceFile "\t\{\"[lindex $mess 0]\", [lindex $mess 1]\}"
            if {$count != $tmessNum} then {
                puts $TCommProc::tSourceFile ","
            }
            #add time label name to list
            lappend TCommProc::labelList [lindex $mess 0]
        }
        puts $TCommProc::tSourceFile "\};"
    }
    
    #generate source files information
    set messNum [llength $srcRefLst]
    if {$messNum > 0} then {
        puts $TCommProc::tSourceFile "static TAT_SRC_FILE_DEF tat_${TCommProc::testSuite}_${testCaseName}_SrcFiles\[\] = \{"
        set count 0
        foreach {mess} $srcRefLst {
            puts -nonewline $TCommProc::tSourceFile "\t\{[embedLine [lindex $mess 0]], [lindex $mess 1], [lindex $mess 2], [lindex $mess 3] \}"
            incr count
            if {$count != $messNum} then {
                puts $TCommProc::tSourceFile ","
            }
        }
        puts $TCommProc::tSourceFile "\};"
    }

    puts $TCommProc::hCommFile "extern TAT_TEST_RESULT TAT_TI_${TCommProc::testSuite}_${testCaseName}( TAT_TEST_INSTANCE* id );"
    puts $TCommProc::hCommFile "extern TAT_TEST_RESULT TAT_T_${TCommProc::testSuite}_${testCaseName}( TAT_TEST_INSTANCE* id );"

    puts $TCommProc::tSourceFile "TAT_TEST_RESULT TAT_TI_${TCommProc::testSuite}_${testCaseName}( TAT_TEST_INSTANCE* id )"
    puts $TCommProc::tSourceFile "\{"
    if {$varNum > 0} then {
        puts $TCommProc::tSourceFile "\tid->varDefs = (TAT_VARIABLE_DEF*)\&tat_${TCommProc::testSuite}_${testCaseName}_Vardef;"
    } else {
        puts $TCommProc::tSourceFile "\tid->varDefs = NULL;"
    }
    if {$tmessNum > 0} then {
        puts $TCommProc::tSourceFile "\tid->timeLabels = (TAT_TIME_LABEL_DEF*)\&tat_${TCommProc::testSuite}_${testCaseName}_TimeLabel;"
    } else {
        puts $TCommProc::tSourceFile "\tid->timeLabels = NULL;"
    }
    if {$messNum > 0} then {
        puts $TCommProc::tSourceFile "\tid->srcFiles = (TAT_SRC_FILE_DEF*)\&tat_${TCommProc::testSuite}_${testCaseName}_SrcFiles;"
    } else {
        puts $TCommProc::tSourceFile "\tid->srcFiles = NULL;"
    }
    puts $TCommProc::tSourceFile "\tid->timeLabelCount = $tmessNum;"
    puts $TCommProc::tSourceFile "\tid->srcFilesCount = $messNum;"
    puts $TCommProc::tSourceFile "\tid->varCount = $varNum;"
    
    puts $TCommProc::tSourceFile "\tid->branchCount = $branchNumber;"
    puts $TCommProc::tSourceFile "\tid->repeatCount = $testCaseRep;"
    puts $TCommProc::tSourceFile "\tid->timerCount  = $timerNumber;"
    puts $TCommProc::tSourceFile "\tid->loopCount   = $loopCount;"
    set TCommProc::loopMax $loopCount
    set TCommProc::loopID 0
    set TCommProc::loopIDLst {}
    set TCommProc::loopNest 0
    
    puts $TCommProc::tSourceFile "\treturn TAT_TEST_CONTINUE;"
    puts $TCommProc::tSourceFile "\}"
    
    #generate all instance set declarations
    set count 0
    foreach {mess} $instSetLst {
        puts -nonewline $TCommProc::tSourceFile "static TAT_INSTANCE_ID tat_I_${TCommProc::testSuite}_${testCaseName}_N${count}\[\]=\{"
        incr count
        set messNum [llength $mess]
        set pcount 0
        foreach {inst} $mess {
            incr pcount
            puts -nonewline $TCommProc::tSourceFile "TAT_I_[lindex $InstanceSet [expr $inst*$INSTANCE_RECORD_SIZE]]_ID"
            if {$pcount != $messNum} then {
                puts -nonewline $TCommProc::tSourceFile ","
            }
        }
        puts $TCommProc::tSourceFile "\};"
    }

       
    #generate event's declarations
    set count 0
    foreach {mess} $eventLst {
        #check for supported operator
        if {[lindex $mess $ATS_EVENT_TYPE]!="alt"} {
            set signalName ""
            set signalParamCount 0
            set scount 0
            set srcRef [lindex $mess $ATS_EVENT_SREF]
            set messNum [llength $srcRef]
            #generate table of source references for event
            if {$messNum >0 } then {
                puts -nonewline $TCommProc::tSourceFile "static TAT_SRC_REF_STRUCT tat_${TCommProc::testSuite}_${testCaseName}_${count}_SrcRef\[\] = \{"
                foreach {sref} $srcRef {
                    puts -nonewline $TCommProc::tSourceFile "\{[lindex $sref 0], [lindex $sref 1]\}"
                    incr scount
                    if {$scount < $messNum} then {
                        puts -nonewline $TCommProc::tSourceFile ", "
                    }
                }
                puts $TCommProc::tSourceFile "\};"
            }
            set eventType [lindex $mess $ATS_EVENT_TYPE]
            set eventData [lindex $mess $ATS_EVENT_DATA]
            
            #generate message data
            if {$eventType == "send" || $eventType == "recv" || $eventType == "chck"} then {
                set signalName [lindex $eventData 0]
                if {$signalName == "-1"} then {
                    TCommProc::genAbort "ERROR: ats: file '$TConfVars::ATSFile': undefined signals not supported"
                }
                set signalParamType [lindex $SignalSet [expr {$signalName * $SIGNAL_RECORD_SIZE + $SIGNAL_PARAMS}]]
                set signalName [lindex $SignalSet [expr {$signalName * $SIGNAL_RECORD_SIZE}]]
                set signalParamCount [llength $signalParamType]
                set signalParams [lindex [lindex $mess $ATS_EVENT_DATA] 1]
                #check if there are possible parameters
                if {$signalParamCount >0 } then {
                    puts $TCommProc::tSourceFile "static TAT_SIG_${signalName} tat_${TCommProc::testSuite}_${testCaseName}_${count}_sigdata;"
                }
                   
            }
            
			set data [lindex $mess $ATS_EVENT_DATA]
			if { ($TConfVars::sgParamsComm == 1) && ($eventType == "recv" || $eventType == "send") && ([lindex $data 1] != "") } {
				puts $TCommProc::tSourceFile "static char *tat_${TCommProc::testSuite}_${testCaseName}_${count}_paramsComments\[\] = [TCommProc::createParamsCommentsLst [lindex $data 1] [lindex $SignalSet [expr [lindex $data 0] * $SIGNAL_RECORD_SIZE + $SIGNAL_FIELDS] ] ];"
			}
            set eventSpec ""
            set initialSpec "NULL"
            append eventSpec "static TAT_EVENT_INSTANCE tat_${TCommProc::testSuite}_${testCaseName}_${count}_event = \{"
            switch -exact $eventType {
            "send" {
                append eventSpec "TAT_SEND_EV, TAT_S_${signalName}_ID, "
                if {$signalParamCount == 0} then {
                    append eventSpec "NULL, "
                } else {
                    append eventSpec "\&tat_${TCommProc::testSuite}_${testCaseName}_${count}_sigdata, "
                }
                if {$TConfVars::singleMT == 0} then {
                    set initialSpec "\&tat_${TCommProc::testSuite}_${testCaseName}_MessageTable\[[lindex $eventData 2]\]"
                } else {
                    set initialSpec "\&TAT_${TCommProc::testSuite}MessageTable\[[lindex $eventData 2]\]"
                }
            }
            "chck"
                -
            "recv" {
                append eventSpec "TAT_RECV_EV, TAT_S_${signalName}_ID, "
                if {$signalParamCount == 0} then {
                    append eventSpec "NULL, "
                } else {
                    append eventSpec "\&tat_${TCommProc::testSuite}_${testCaseName}_${count}_sigdata, "
                }
                if {$TConfVars::singleMT == 0} then {
                    set initialSpec "\&tat_${TCommProc::testSuite}_${testCaseName}_MessageTable\[[lindex $eventData 2]\]"
                } else {
                    set initialSpec "\&TAT_${TCommProc::testSuite}MessageTable\[[lindex $eventData 2]\]"
                }
            }
            "actn"
                -
            "code" {
                append eventSpec "TAT_ACTION_EV, 0, [embedLine [lindex $eventData 0]], "
            }
            "cond" {
                append eventSpec "TAT_CONDITION_EV, 0, \"[lindex $eventData 0]\", "
            }
            "refr" {
                append eventSpec "TAT_REFERENCE_EV, 0, \"[lindex $eventData 0]\", "
            }
            "create" {
                append eventSpec "TAT_CREATE_EV, 0, \"[lindex $eventData 0]\", "
            }
            "stop" {
                append eventSpec "TAT_STOP_EV, 0, NULL, "
            }
            
            "par"
            -
            "opt"
            -
            "exc"
            -
            "timer_set"
            -
            "save_time"
            -
            "timer_reset"
            -
            "reset_time"
            -
            "timer_timeout"
            -
            "check_time"
            -
            "loop"
            -
            "start_loop"
            -
            "end_loop"
            {
                append eventSpec "TAT_UNKNOWN_EV, 0, NULL, "
            }

            }
            #output common part of event declaration
            if {$TConfVars::timeCheck == 1} then {
                append eventSpec "\{\{0,0\}, \{0,0\}, \"\"\},"
            }

            append eventSpec "[llength [lindex $instSetLst [lindex $mess $ATS_EVENT_INST]]], "
            append eventSpec "\n(TAT_INSTANCE_ID *) \&tat_I_${TCommProc::testSuite}_${testCaseName}_N${count}, "
            append eventSpec "[embedLine [lindex $mess $ATS_EVENT_COMMENT]], [llength [lindex $mess $ATS_EVENT_SREF]], "
            if {[llength [lindex $mess $ATS_EVENT_SREF]]>0} then {
            append eventSpec "(TAT_SRC_REF_STRUCT *) tat_${TCommProc::testSuite}_${testCaseName}_${count}_SrcRef, "
            } else {
                append eventSpec "(TAT_SRC_REF_STRUCT *)NULL, "
            }
            
            append eventSpec "\nTAT_VERDICT_[lindex $mess $ATS_EVENT_VERD], [lsearch $TCommProc::labelList [lindex $mess $ATS_EVENT_LABEL]], "
            append eventSpec "${initialSpec},\n"
            if { ($TConfVars::sgParamsComm == 1) && ($eventType == "recv" || $eventType == "send") && ([lindex $data 1] != "") } {
                append eventSpec "tat_${TCommProc::testSuite}_${testCaseName}_${count}_paramsComments"
            } else {
                append eventSpec "NULL"
            }
            append eventSpec "\};\n"
            puts $TCommProc::tSourceFile $eventSpec
        }
        incr count
    }

	# Generate assign and compare functions for complex data types
	set signalSetLen [expr [llength $SignalSet]/$SIGNAL_RECORD_SIZE]
	set lineN 0
	foreach { line } $eventLst {
	# Iterate all events
		if {[lindex $line $ATS_EVENT_TYPE]== "recv"} {
			# Generate compare functions for receive signals
			set data [lindex $line $ATS_EVENT_DATA]
			set sigRef [lindex $data 0]
			set sigToCheck [lindex $data 1]
			set params [lindex $SignalSet [expr $sigRef*$SIGNAL_RECORD_SIZE + $SIGNAL_PARAMS]]
			set stypes [lindex $SignalSet [expr $sigRef*$SIGNAL_RECORD_SIZE + $SIGNAL_STYPES]]
			set paramCounter 0
			foreach {param} $params {stype} $stypes {
				# Iterate all params
				if {$stype != -1} {
					# User defined types only
					set targetname [lindex $DataTypeSet [expr $stype*$DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
					set valuelist [lindex $sigToCheck $paramCounter]
					for {set valueCounter 1} {$valueCounter<[llength $valuelist]} {incr valueCounter} {
						# Iterate all possible values
						set paramToCheck [lindex $valuelist $valueCounter]
						if {[isASN1 $paramToCheck]} {
							# Generate compare function if parameter is ASN.1
							genCmpFunc TAT_Cmp_${testCaseName}_${lineN}_${paramCounter}_${valueCounter} $targetname $stype $paramToCheck $TCommProc::tSourceFile
						}
					}
				}
				incr paramCounter 1
			}
		} elseif {[lindex $line $ATS_EVENT_TYPE]=="send"} {
			# Generate assign functions for send signals
			set data [lindex $line $ATS_EVENT_DATA]
			set sigRef [lindex $data 0]
			set sigToCheck [lindex $data 1]
			set params [lindex $SignalSet [expr $sigRef*$SIGNAL_RECORD_SIZE + $SIGNAL_PARAMS]]
			set stypes [lindex $SignalSet [expr $sigRef*$SIGNAL_RECORD_SIZE + $SIGNAL_STYPES]]
			set paramCounter 0
			foreach {param} $params {stype} $stypes {
				# Iterate all params
				if {$stype != -1} {
					# User defined types only
					set targetname [lindex $DataTypeSet [expr $stype*$DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
					set paramToCheck [lindex [lindex $sigToCheck $paramCounter] 1]
					if {[isASN1 $paramToCheck]} {
						# Generate assign function if parameter is ASN.1
						genAssFunc TAT_Ass_${testCaseName}_${lineN}_${paramCounter} $targetname $stype $paramToCheck $TCommProc::tSourceFile
					}
				}
				incr paramCounter
			}
		}
		incr lineN
	}
}

#########################################################################
# Procedure : FinishTestCase
# 
# Description: Generates the completion of a test case
# 
# Returns: None
# 
# Notes: None
# 
#########################################################################
proc FinishTestCase { testCaseName args } {
    
    #close procedure definition for TTS mode
    if {$TCommProc::ATSmode == 1} then {
        puts $TCommProc::tSourceFile "TAT_TEST_RESULT TAT_T_${TCommProc::testSuite}_${testCaseName}( TAT_TEST_INSTANCE* id )"
        puts $TCommProc::tSourceFile "\{"
        set count 0
        set len [llength $TCommProc::stateFunc]
        puts -nonewline $TCommProc::tSourceFile "\tstatic const TAT_STATE_FUNC states\[${len}\]=\{\n\t"
        foreach {tname} $TCommProc::stateFunc {
            incr count
            puts -nonewline $TCommProc::tSourceFile $tname
            if {$count!=$len} then {
                puts -nonewline $TCommProc::tSourceFile ","
                if {[expr $count % 5]==0} then {
                    puts -nonewline $TCommProc::tSourceFile "\n\t"
                }
            }
        }
        puts $TCommProc::tSourceFile "\};"
        puts $TCommProc::tSourceFile "\tTAT_TEST_RESULT res;"
        puts $TCommProc::tSourceFile "\tint prevState;"
        puts $TCommProc::tSourceFile "\tif( id == NULL )"
        puts $TCommProc::tSourceFile "\t\{"
		puts $TCommProc::tSourceFile "\t\tid->testStatus = TAT_ERR_INTERNAL;"
		puts $TCommProc::tSourceFile "\t\tid->testResult = TAT_TEST_CRITICAL;"
		puts $TCommProc::tSourceFile "\t\tid->dumpEvent(id, NULL, NULL);"
		puts $TCommProc::tSourceFile "\t\treturn TAT_TEST_CRITICAL;"
        puts $TCommProc::tSourceFile "\t\}"
        puts $TCommProc::tSourceFile "\twhile( id->nextState != TAT_FINAL_STATE )"
        puts $TCommProc::tSourceFile "\t\{"
        puts $TCommProc::tSourceFile "\t\tprevState=id->nextState;"
        puts $TCommProc::tSourceFile "\t\tres = states\[id->nextState\](id);"
        puts $TCommProc::tSourceFile "\t\tif (res == TAT_TEST_CRITICAL) return TAT_TEST_CRITICAL;"
        if {$TConfVars::testBreak == 1} then {
            puts $TCommProc::tSourceFile "\t\tif (res == TAT_TEST_FINISHED && id->testStatus != TAT_OK)"
            puts $TCommProc::tSourceFile "\t\t\treturn TAT_TEST_FINISHED;"
        }

        puts $TCommProc::tSourceFile "\t\tif (res == TAT_TEST_FINISHED && id->nextState != TAT_FINAL_STATE && id->nextState==prevState)"
        puts $TCommProc::tSourceFile "\t\t\treturn TAT_TEST_CONTINUE;"
        puts $TCommProc::tSourceFile "\t\}"
        puts $TCommProc::tSourceFile "\treturn TAT_TEST_FINISHED;"
        puts $TCommProc::tSourceFile "\}\n\n"
        puts $TCommProc::tSourceFile "/****************************************************************************/"
    }
    if { $TConfVars::genMultiTC == 1 } then {
        close $TCommProc::tSourceFile
    }
    puts $TCommProc::tLogFile "REMARK: Test case \"$testCaseName\" generation finished"
}

#########################################################################
# Procedure : EndTestSuite
# 
# Description: Generates the completion of a test suite
# 
# Returns: None
# 
# Notes: None
# 
#########################################################################
proc EndTestSuite { testSuiteName args } {

    puts $TCommProc::sCommFile "TAT_TEST_TABLE TAT_${testSuiteName}_TestTable\[\]=\{"
    set len [llength $TCommProc::testCount]
    set count 0

    if { $TConfVars::genMultiTC == 1 } then {
        # creat a list file for test case names
        if { [catch {set tListFile [open [file join $TConfVars::outputDir "tclist.txt"] w ] } res ] } then {
                TCommProc::genAbort "ERROR: file: '[file join $TConfVars::outputDir tclist.txt]' can not open for writing/n$res"
        }
        # dumping test case names into the list file
        foreach {fname} $TCommProc::testCount {
            puts $tListFile $fname 
        }
        close $tListFile
    }
    foreach {fname} $TCommProc::testCount {
        puts -nonewline $TCommProc::sCommFile "\{\"${fname}\", TAT_T_${testSuiteName}_${fname}, TAT_TI_${testSuiteName}_${fname}\}"
        incr count
        if {$count!=$len} then {
            puts $TCommProc::sCommFile ","
        }
    }
    puts $TCommProc::sCommFile "\};"
    TCommProc::endTestSuite $testSuiteName
}

#########################################################################
# Procedure : SignalTable
# 
# Description: Generates message table common for all test cases
# 
# Returns: None
# 
# Notes: set TConfVars:singleMT variable
# 
########################################################################
proc SignalTable { messageList args } {
    set TConfVars:singleMT 1
    #output global table with list of text messages
    puts $TCommProc::sCommFile "char * TAT_${TCommProc::testSuite}MessageTable\[\] = \{"
    puts $TCommProc::hCommFile "extern char * TAT_${TCommProc::testSuite}MessageTable\[\];"
    set count 0
    set messNum [llength $messageList]
    foreach {mess} $messageList {
        incr count
        if {$count == $messNum} then {
            puts $TCommProc::sCommFile "\t\"${mess}\""
        } else {
            puts $TCommProc::sCommFile "\t\"${mess}\","
        }
    }
    puts $TCommProc::sCommFile "\};"
    unset count messNum
}

#########################################################################
# Procedure : unknown
# 
# Description: Handle attempts to use non-existent commands
# 
# Returns: None
# 
# Notes: Standart handler of unknown command
# 
#########################################################################
proc unknown { commandName args } {
    TCommProc::genAbort "ERROR: ats: file '$TConfVars::ATSFile': unknown command '${commandName}'"
}

proc _outCheckUnexp {} {
    if {$TConfVars::queueCheck == 1} then {
        puts $TCommProc::tSourceFile "\t\t\tif (id->queue.length) \{"
        puts $TCommProc::tSourceFile "\t\t\t\tid->testStatus=TAT_ERR_EVENT_UNEXP;"
        puts $TCommProc::tSourceFile "\t\t\t\tid->dumpEvent(id, NULL, NULL);"
        if {$TConfVars::testBreak == 1} then {
            puts $TCommProc::tSourceFile "\t\t\t\tid->testResult = TAT_TEST_FINISHED;"
            puts $TCommProc::tSourceFile "\t\t\t\treturn TAT_TEST_FINISHED;"
        }
        puts $TCommProc::tSourceFile "\t\t\t\};"
    }
}

proc _outDiscardUnexp {state} {
    if {$TConfVars::discardUnexp == 1} then {
        #puts $TCommProc::tSourceFile "\t\tif (id->dumpEvent(id,_ev,_ev)!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;"
        puts $TCommProc::tSourceFile "\t\tid->popQueue(id);"
	puts $TCommProc::tSourceFile "\t\tid->nextState=${state};"
        puts $TCommProc::tSourceFile "\t\treturn TAT_TEST_CONTINUE;"
    }
}

proc _checkTimeCondSt { tVar tOffset tEv tVRS tCond} {
    set cond ""
    if {$TConfVars::timeCheck == 1} {
        if {[lindex $tEv 0]=="REL"} {
            set t1 [lindex $tEv 1]
            set t2 [lindex $tEv 2]
            if {[string equal $t1 $t2 ]} then {
                append cond "(${tVar}-${tOffset}==[TCommProc::changeExpr ${t1}])"
            } else {
                append cond "(${tVar}-${tOffset}>=[TCommProc::changeExpr ${t1}])"
                if {[string first "INF" $t2] == -1} then {
                    append cond " && (${tVar}<=[TCommProc::changeExpr ${t2}])"
                }
            }
        } else {
            set t1 [lindex $tEv 1]
            set t2 [lindex $tEv 2]
            if {[string equal $t1 $t2 ]} then {
                append cond "(${tVar}==[TCommProc::changeExpr ${t1}])"
            } else {
                append cond "(${tVar}>=[TCommProc::changeExpr ${t1}])"
                if {[string first "INF" $t2] == -1} then {
                    append cond " && (${tVar}<=[TCommProc::changeExpr ${t2}])"
                }
            }
        }
    }
    if {$tCond!=""} then {
        if {$cond!=""} then {
            append cond " && "
        }
        append cond "([TCommProc::changeExpr $tCond])"
    }
    if {$cond!=""} then {
        puts $TCommProc::tSourceFile "\t\tif (${cond}) \{"
    }
}

proc _checkTimeCondCl { tVar tOffset tEv tVRS tCond} {
    if {($TConfVars::timeCheck == 1) || ($tCond!="")} then {
        puts $TCommProc::tSourceFile "\t\t\}"
    }
}
proc _setLabel {str} {
    if {$str!=""} then {
        puts $TCommProc::tSourceFile "\t\t\t\t[TCommProc::changeExpr @${str} ] = id->clock;"
    }
    puts $TCommProc::tSourceFile "\t\t\t\tid->lastEvent = id->clock;"
}
proc _logical_event {evnum ev ns} {
    global ATS_EVENT_TYPE ATS_EVENT_TIME ATS_EVENT_OTIME ATS_EVENT_PTIME ATS_EVENT_COND ATS_EVENT_INST ATS_EVENT_VERD
    global ATS_EVENT_SREF ATS_EVENT_COMMENT ATS_EVENT_LABEL ATS_EVENT_DATA
    
    set checkTime [lindex [lindex $ev $ATS_EVENT_TIME] 1]
    set vrsTime [lindex [lindex $ev $ATS_EVENT_OTIME] 0]
    set evCond [lindex $ev $ATS_EVENT_COND]
    set evType [lindex $ev $ATS_EVENT_TYPE]
    set evData [lindex $ev $ATS_EVENT_DATA]
    set evName "tat_${TCommProc::testSuite}_${TCommProc::testCase}_${evnum}_event"
    set evLabel [lindex $ev $ATS_EVENT_LABEL]
    
    _checkTimeCondSt "id->clock" "id->lastEvent" $checkTime $vrsTime $evCond
    if {$evType == "cond"} then {
        set condName [lindex $evData 0]
        set condCheck [TCommProc::changeExpr [lindex $evData 1]]
        if { ([string compare -nocase $condName "IF"] == 0) ||
             ([string compare -nocase $condName "WHEN"] == 0) } then {
            puts $TCommProc::tSourceFile "\t\t\tif (${condCheck})\{"
            _setLabel $evLabel
            _outCheckUnexp
            puts $TCommProc::tSourceFile "\t\t\t\tif (id->dumpEvent(id, &${evName}, &${evName})!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;"
            puts $TCommProc::tSourceFile "\t\t\tid->nextState=${ns};"
            puts $TCommProc::tSourceFile "\t\t\treturn TAT_TEST_FINISHED;"
            puts $TCommProc::tSourceFile "\t\t\t\}"
        } elseif {[string compare -nocase $condName "ASSERT"] == 0} then {
            puts $TCommProc::tSourceFile "\t\t\tif (${condCheck})\{"
            _setLabel $evLabel
            _outCheckUnexp
            puts $TCommProc::tSourceFile "\t\t\t\tif (id->dumpEvent(id, &${evName}, &${evName})!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;"
            puts $TCommProc::tSourceFile "\t\t\tid->nextState=${ns};"
            puts $TCommProc::tSourceFile "\t\t\t\} else \{ id->testStatus = TAT_ERR_COND_ASSERT; id->testResult=TAT_TEST_FINISHED;"
            puts $TCommProc::tSourceFile "\t\t\t\tif (id->dumpEvent(id, NULL, &${evName})!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;"
            puts $TCommProc::tSourceFile "\}"
        }
    } else {
        _setLabel $evLabel
        _outCheckUnexp
        puts $TCommProc::tSourceFile "\t\t\t\tif (id->dumpEvent(id, &${evName}, &${evName})!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;"
        puts $TCommProc::tSourceFile "\t\t\tid->nextState=${ns};"
    }
    _checkTimeCondCl "id->clock" "id->lastEvent" $checkTime $vrsTime $evCond
}

proc _timer_event {evnum ev ns} {
    global ATS_EVENT_TYPE ATS_EVENT_TIME ATS_EVENT_OTIME ATS_EVENT_PTIME ATS_EVENT_COND ATS_EVENT_INST ATS_EVENT_VERD
    global ATS_EVENT_SREF ATS_EVENT_COMMENT ATS_EVENT_LABEL ATS_EVENT_DATA
}
proc _passive_event {evnum ev ns} {
    global ATS_EVENT_TYPE ATS_EVENT_TIME ATS_EVENT_OTIME ATS_EVENT_PTIME ATS_EVENT_COND ATS_EVENT_INST ATS_EVENT_VERD
    global ATS_EVENT_SREF ATS_EVENT_COMMENT ATS_EVENT_LABEL ATS_EVENT_DATA
    
    set checkTime [lindex [lindex $ev $ATS_EVENT_TIME] 1]
    set vrsTime [lindex [lindex $ev $ATS_EVENT_OTIME] 0]
    set evCond [lindex $ev $ATS_EVENT_COND]
    set evType [lindex $ev $ATS_EVENT_TYPE]
    set evData [lindex $ev $ATS_EVENT_DATA]
    set evName "tat_${TCommProc::testSuite}_${TCommProc::testCase}_${evnum}_event"
    set evLabel [lindex $ev $ATS_EVENT_LABEL]

    global InstanceSet
    global INSTANCE_RECORD_SIZE

    global DataTypeSet
    global DATATYPE_NAME
    global DATATYPE_TYPE
    global DATATYPE_DIMENSION
    global DATATYPE_STYPE
    global DATATYPE_ATTR
    global DATATYPE_ENUM
    global DATATYPE_PARAMTYPE
    global DATATYPE_DATACLASS
    global DATATYPE_MEMBERS
    global DATATYPE_RECORD_SIZE

    global SignalSet
    global SIGNAL_NAME
    global SIGNAL_TYPE
    global SIGNAL_ATTR
    global SIGNAL_PARAMS
    global SIGNAL_STYPES
    global SIGNAL_FIELDS
    global SIGNAL_PARAMTYPE
    global SIGNAL_VARARG
    global SIGNAL_PRIORITY
    global SIGNAL_RECORD_SIZE
    
    switch -exact $evType {
        "recv" {
	    set lineN [lindex $ev $ATS_EVENT_INST]
            set sigID [lindex $evData 0]
            set sigData [lindex $evData 1]
            set sigName [lindex $SignalSet [expr {$sigID*$SIGNAL_RECORD_SIZE + $SIGNAL_NAME}]]
            set sigParams [lindex $SignalSet [expr $sigID*$SIGNAL_RECORD_SIZE + $SIGNAL_PARAMS]]
            set sigType  [lindex $SignalSet [expr {$sigID*$SIGNAL_RECORD_SIZE + $SIGNAL_STYPES}]]
            set sigField [lindex $SignalSet [expr {$sigID*$SIGNAL_RECORD_SIZE + $SIGNAL_FIELDS}]]
            
            #check conditions before check event data
            _checkTimeCondSt "_ev->eventTime.timeCalc.minTime" "id->lastEvent" $checkTime $vrsTime $evCond
            set checkPar "(_ev->eventID == TAT_S_${sigName}_ID)"
            
            set pcount 0
            foreach sigPar $sigData stype $sigType {
		set kLen [llength $sigPar]
                #if there are values to compare with
				set tmpStr [string trim [lindex $sigPar 1]]
				set commStr ""
				while { 1 } {
					set startInd [string first "/*" $tmpStr]
					if { $startInd == -1 } {
						break
					}
					set finishInd [string first "*/" $tmpStr]
					append commStr [string range $tmpStr $startInd [expr $finishInd + 1]]
					set tmpStr [string replace $tmpStr $startInd [expr $finishInd + 1] ""]
				}
				set tmpStr [string trim $tmpStr]

                if { $tmpStr != "" } {
                    if {$kLen > 1} {
                        set parName "((TAT_SIG_${sigName} *)_ev->eventData)->[lindex $sigField $pcount]"
                        set i 1
                        # try to determine name of function to compare values
                        set cFunc ""
                        if {[lindex $sigType $pcount]!=-1} {
                            #extract second attribute of data type, which specifies function to compare
                            set cFunc [lindex [lindex $DataTypeSet [expr [lindex $sigType $pcount]*$DATATYPE_RECORD_SIZE + $DATATYPE_ATTR]] 1]
                        }
                        set fComp ""
                        while {$i<$kLen} {
                            if {$cFunc!=""} then {
                                append fComp "${cFunc}(${parName},[TCommProc::changeExpr [lindex $sigPar $i] ])==0"
                            } else {
				if {$stype != -1} {
					set targetname [lindex $DataTypeSet [expr $stype*$DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
					set targettype [lindex $DataTypeSet [expr $stype*$DATATYPE_RECORD_SIZE + $DATATYPE_TYPE]]
					if {[isASN1 [lindex $sigPar $i]]} {
						append fComp "(TAT_Cmp_${TCommProc::testCase}_${lineN}_${pcount}_${i}(${parName},id) == 0)"
					} else {
						append fComp "(!TAT_Compare_${targetname}(${parName},[TCommProc::changeExpr [lindex $sigPar $i]]))"
					}
				} else {
					append fComp "(${parName}==([TCommProc::changeExpr [lindex $sigPar $i]]))"
				}
                            }
                            incr i
                            if {$i<$kLen} then {
                                append fComp " || "
                            }
                        }
                        append checkPar "&&\n\t\t\t\t(${fComp})"
                    }
                }
                incr pcount
            }

            puts $TCommProc::tSourceFile "\t\t\tif (${checkPar})\{"
            _setLabel $evLabel
            set pcount 0
            foreach {sigPar} $sigData {
                set vName [lindex $sigPar 0]
                if {$vName!=""} then {
			if {[lindex $sigType $pcount]==-1} {
				# Basic data type; use assignment operator =
               			puts $TCommProc::tSourceFile "\t\t\t\tvar_${TCommProc::testCase}_${vName}=((TAT_SIG_${sigName} *)_ev->eventData)->[lindex $sigField $pcount];"
			} else {
				# User defined data type; use copy function and free before copying
				puts $TCommProc::tSourceFile "\t\t\t\tTAT_Free_[lindex $sigParams $pcount](&(var_${TCommProc::testCase}_${vName}));"
				puts $TCommProc::tSourceFile "\t\t\t\tvar_${TCommProc::testCase}_${vName}=TAT_Copy_[lindex $sigParams $pcount](((TAT_SIG_${sigName} *)_ev->eventData)->[lindex $sigField $pcount]);"
			}
		}
                incr pcount
            }
            puts $TCommProc::tSourceFile "\t\t\t\tif (id->dumpEvent(id, _ev, &${evName})!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;"
            foreach sigPar $sigData stype $sigType field $sigField {
		if {$stype != -1} {
			set targetname [lindex $DataTypeSet [expr $stype*$DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
			set targettype [lindex $DataTypeSet [expr $stype*$DATATYPE_RECORD_SIZE + $DATATYPE_TYPE]]		
			puts $TCommProc::tSourceFile "\t\t\t\tTAT_Free_${targetname}(&(((TAT_SIG_${sigName} *)_ev->eventData)->${field}));"
		}
	    }
            puts $TCommProc::tSourceFile "\t\t\t\tid->popQueue(id);"
            puts $TCommProc::tSourceFile "\t\t\t\tid->nextState=${ns};"
            puts $TCommProc::tSourceFile "\t\t\t\treturn TAT_TEST_CONTINUE;"
            puts $TCommProc::tSourceFile "\t\t\t\}"
            _checkTimeCondCl "_ev->eventTime.timeCalc.minTime" "id->lastEvent" $checkTime $vrsTime $evCond
        }
        default {
            _checkTimeCondSt "id->clock" "id->lastEvent" $checkTime $vrsTime $evCond
            puts $TCommProc::tSourceFile "\t\t\tif (id->dumpEvent(id,&${evName}, &${evName})!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;"
            _setLabel $evLabel
            puts $TCommProc::tSourceFile "\t\t\tid->nextState=${ns};"
            _checkTimeCondCl "id->clock" "id->lastEvent" $checkTime $vrsTime $evCond
        }
    }
}

proc _active_event {evnum ev ns active branchID br njs} {
    global InstanceSet INSTANCE_RECORD_SIZE
    global ATS_EVENT_TYPE ATS_EVENT_TIME ATS_EVENT_OTIME ATS_EVENT_PTIME ATS_EVENT_COND ATS_EVENT_INST ATS_EVENT_VERD
    global ATS_EVENT_LABEL ATS_EVENT_DATA
    
    set checkTime [lindex [lindex $ev $ATS_EVENT_TIME] 1]
    set vrsTime [lindex [lindex $ev $ATS_EVENT_OTIME] 0]
    set evCond [lindex $ev $ATS_EVENT_COND]
    set evType [lindex $ev $ATS_EVENT_TYPE]
    set evData [lindex $ev $ATS_EVENT_DATA]
    set evName "tat_${TCommProc::testSuite}_${TCommProc::testCase}_${evnum}_event"
    set evLabel [lindex $ev $ATS_EVENT_LABEL]
    
    _checkTimeCondSt "id->clock" "id->lastEvent" $checkTime $vrsTime $evCond
    _outCheckUnexp
    
    global DataTypeSet
    global DATATYPE_NAME
    global DATATYPE_TYPE
    global DATATYPE_DIMENSION
    global DATATYPE_STYPE
    global DATATYPE_ATTR
    global DATATYPE_ENUM
    global DATATYPE_PARAMTYPE
    global DATATYPE_DATACLASS
    global DATATYPE_MEMBERS
    global DATATYPE_RECORD_SIZE

    global SignalSet
    global SIGNAL_NAME
    global SIGNAL_TYPE
    global SIGNAL_ATTR
    global SIGNAL_PARAMS
    global SIGNAL_STYPES
    global SIGNAL_FIELDS
    global SIGNAL_PARAMTYPE
    global SIGNAL_VARARG
    global SIGNAL_PRIORITY
    global SIGNAL_RECORD_SIZE

    switch -exact $evType {
        "actn" {
            puts $TCommProc::tSourceFile "\t\t\tif (id->dumpEvent(id,&${evName}, &${evName})!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;"
            puts $TCommProc::tSourceFile "\t\t\t[TCommProc::changeExpr [lindex $evData 0]];"
            _setLabel $evLabel
        }
        "send" {
            set sigID [lindex $evData 0]
            set sigData [lindex $evData 1]
	    set sigName [lindex $SignalSet [expr $sigID*$SIGNAL_RECORD_SIZE + $SIGNAL_NAME]]
	    set sigType [lindex $SignalSet [expr $sigID*$SIGNAL_RECORD_SIZE + $SIGNAL_TYPE]]
	    set params [lindex $SignalSet [expr $sigID*$SIGNAL_RECORD_SIZE + $SIGNAL_PARAMS]]
	    set stypes [lindex $SignalSet [expr $sigID*$SIGNAL_RECORD_SIZE + $SIGNAL_STYPES]]
	    set fields [lindex $SignalSet [expr $sigID*$SIGNAL_RECORD_SIZE + $SIGNAL_FIELDS]]
	    set lineN [lindex $ev $ATS_EVENT_INST]

            #copy source infor data to new event
            puts $TCommProc::tSourceFile "\t\t\tmemcpy(&__ev, &${evName}, sizeof(__ev));"
            puts -nonewline $TCommProc::tSourceFile "\t\t\tif (TAT_Send_[lindex $SignalSet [expr {$sigID*$SIGNAL_RECORD_SIZE}]](id, &__ev, &${evName}"

            set paramCounter -1
            foreach sigPar $sigData stype $stypes {
		incr paramCounter 1
		if {$stype != -1} {
			set targetname [lindex $DataTypeSet [expr $stype*$DATATYPE_RECORD_SIZE + $DATATYPE_NAME]]
			set targettype [lindex $DataTypeSet [expr $stype*$DATATYPE_RECORD_SIZE + $DATATYPE_TYPE]]
			if {[isASN1 [lindex $sigPar 1]]} {
				puts -nonewline $TCommProc::tSourceFile ", TAT_Ass_${TCommProc::testCase}_${lineN}_${paramCounter}(id)"
			} else {
				puts -nonewline $TCommProc::tSourceFile ", TAT_Copy_${targetname}([TCommProc::changeExpr [lindex $sigPar 1]])"
			}
		} else {
			puts -nonewline $TCommProc::tSourceFile ", [TCommProc::changeExpr [lindex $sigPar 1]]"
		}
            }
            puts $TCommProc::tSourceFile ")!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;"
            _setLabel $evLabel
        }
	"start_loop" {
		if { [lindex $evData 2] != "" } {
			puts $TCommProc::tSourceFile "\tif ( !([TCommProc::changeExpr [lindex $evData 2]]) )"
			puts $TCommProc::tSourceFile "\t\{"
			puts $TCommProc::tSourceFile "\t\tid->nextState=${njs};"
			puts $TCommProc::tSourceFile "\t\treturn TAT_TEST_CONTINUE;"
			puts $TCommProc::tSourceFile "\t\}"
			if { $TCommProc::loopNest > 0 } {
				lappend TCommProc::loopIDLst -1
			}
		} elseif { [lindex $evData 0] == [lindex $evData 1] } {
			if { $TCommProc::loopID >= $TCommProc::loopMax } {
				TCommProc::genAbort "ERROR: ats: file '$TConfVars::ATSFile': inconsistency between declared and actual loop numbers.\n"
			}
			puts $TCommProc::tSourceFile "\tif ( id->testCaseLoops\[${TCommProc::loopID}\] >= [TCommProc::changeExpr [lindex $evData 0]] )"
			puts $TCommProc::tSourceFile "\t\{"
			puts $TCommProc::tSourceFile "\t\tid->nextState=${njs};"
			puts $TCommProc::tSourceFile "\t\treturn TAT_TEST_CONTINUE;"
			puts $TCommProc::tSourceFile "\t\} else \{"
			puts $TCommProc::tSourceFile "\t\tid->testCaseLoops\[${TCommProc::loopID}\]++;"
			puts $TCommProc::tSourceFile "\t\}"
			if { $TCommProc::loopNest > 0 } {
				lappend TCommProc::loopIDLst $TCommProc::loopID
			}
			incr TCommProc::loopID
		} else {
		        TCommProc::genAbort "ERROR: ats: file '$TConfVars::ATSFile': contains loop with two different parameters.\n"
		}
		incr TCommProc::loopNest
	}
	"end_loop" {
		set ns $njs
		if { $TCommProc::loopNest == [llength $TCommProc::loopIDLst] } {
			if { [lindex $TCommProc::loopIDLst [expr $TCommProc::loopNest - 1]] != -1 } {
				puts $TCommProc::tSourceFile "\t\t\tid->testCaseLoops\[[lindex $TCommProc::loopIDLst [expr $TCommProc::loopNest - 1]]\] = 0;"
			}
			set TCommProc::loopIDLst [lrange $TCommProc::loopIDLst 0 [expr [llength $TCommProc::loopIDLst] - 2]]
		}
		incr TCommProc::loopNest -1
	}
        default {
            puts $TCommProc::tSourceFile "\t\t\tif (id->dumpEvent(id,&${evName}, &${evName})!=TAT_TEST_CONTINUE) return TAT_TEST_CRITICAL;"
            _setLabel $evLabel
        }
        
    }
    
    puts $TCommProc::tSourceFile "\t\t\tid->nextState=${ns};"
    if {$active>1} then {
        puts $TCommProc::tSourceFile "\t\t\tid->testCaseBranches\[$branchID\]=$br;"
    }
    _checkTimeCondCl "id->clock" "id->lastEvent" $checkTime $vrsTime $evCond
}


proc State {stateID logicals timers passive active branchID transitions} {
    global InstanceSet INSTANCE_RECORD_SIZE SignalSet SIGNAL_NAME SIGNAL_RECORD_SIZE SIGNAL_PARAMS
    global ATS_EVENT_TYPE ATS_EVENT_TIME ATS_EVENT_OTIME ATS_EVENT_PTIME ATS_EVENT_COND ATS_EVENT_INST ATS_EVENT_VERD
    global ATS_EVENT_SREF ATS_EVENT_COMMENT ATS_EVENT_LABEL ATS_EVENT_DATA

    if {$TCommProc::ATSmode != 1} then {
        TCommProc::genAbort "ERROR: ats: file '$TConfVars::ATSFile': contains mix of structures\n"
    }
    lappend TCommProc::stateFunc "tat_${TCommProc::testCase}_state${stateID}"
    puts $TCommProc::tSourceFile "static TAT_TEST_RESULT tat_${TCommProc::testCase}_state${stateID}(TAT_TEST_INSTANCE *id)\n\{"
    if {$passive > 0} then {
        puts $TCommProc::tSourceFile "\tTAT_EVENT_INSTANCE *_ev;"
    }
    if {$active > 0} then {
        puts $TCommProc::tSourceFile "\tTAT_EVENT_INSTANCE __ev;"
    }
    set i 0
    set timer_ [expr $logicals + $timers]
    set passive_ [expr $timer_ + $passive]
    set active_ [expr $passive_ + $active]

    #encode 'CONDITION' ASSERT, IF, WHILE, WHEN
    set logicalTimeCond ""
    set tmpLst [list ]
    while {$i<$logicals} {
        set ti [lindex $transitions $i]
        set checkTime [lindex [lindex [lindex $TCommProc::evList [lindex $ti 0] ] $ATS_EVENT_TIME] 1]
        if {([string first "INF" [lindex $checkTime 2]] == -1) && ( [lindex $checkTime 0] != "REL" ) } {
            lappend tmpLst "(id->clock > [TCommProc::changeExpr [lindex $checkTime 2]])"
        }
        _logical_event [lindex $ti 0] [lindex $TCommProc::evList [lindex $ti 0] ] [lindex $ti 1]
        incr i
        set logicalTimeCond [join $tmpLst " && "]
    }
    
    while {$i<$timer_} {
        set ti [lindex $transitions $i]
        _timer_event [lindex $ti 0] [lindex $TCommProc::evList [lindex $ti 0] ] [lindex $ti 1]
        incr i
    }
    
    set passiveTimeCond ""
    set tmpLst [list ]
    #handle PASSIVE events
    if {$passive > 0} then {
        puts $TCommProc::tSourceFile "\tif (id->queue.length>0) \{"
        puts $TCommProc::tSourceFile "\t\t_ev = id->getQueue(id);"
        while {$i<$passive_} {
            set ti [lindex $transitions $i]
			if { [lindex $ti 0] != -1 } {
				set checkTime [lindex [lindex [lindex $TCommProc::evList [lindex $ti 0] ] $ATS_EVENT_TIME] 1]
				if { ([string first "INF" [lindex $checkTime 2]] == -1) && ( [lindex $checkTime 0] != "REL" ) } {
					lappend tmpLst "(id->clock > [TCommProc::changeExpr [lindex $checkTime 2]])"
				}
				_passive_event [lindex $ti 0] [lindex $TCommProc::evList [lindex $ti 0] ] [lindex $ti 1]
			} else {
				puts $TCommProc::tSourceFile "\t\tid->nextState = -1;"
			}
            incr i
        }
        set passiveTimeCond [join $tmpLst " && "]
	_outCheckUnexp
	_outDiscardUnexp $stateID
        puts $TCommProc::tSourceFile "\t\};"

        set evCond [lindex $[lindex $TCommProc::evList [lindex $ti 0] ] $ATS_EVENT_COND]
        if { ($passive == 1) && ($evCond != "") } {
            puts $TCommProc::tSourceFile "\tif ( !([TCommProc::changeExpr $evCond]) ) \{"
            puts $TCommProc::tSourceFile "\t\tid->nextState=[lindex $ti 1];"
            puts $TCommProc::tSourceFile "\t\};"
        }
    }
    
    #handle ACTIVE events 
    set br 0
    if {$active>1} then {
        puts $TCommProc::tSourceFile "\t\tswitch (id->testCaseBranches\[$branchID\])"
        puts $TCommProc::tSourceFile "\t\t\{"
    }
    
    set tmpLst [list ]
    set activeTimeCond ""
    while {$i<$active_} {
        if {$active>1} then {
            puts $TCommProc::tSourceFile "\t\tcase ${br}: \{"
            incr br
        }
        set ti [lindex $transitions $i]

		if { [lindex $ti 0] != -1 } {
			set checkTime [lindex [lindex [lindex $TCommProc::evList [lindex $ti 0] ] $ATS_EVENT_TIME] 1]
			set condName [lindex [lindex [lindex $TCommProc::evList [lindex $ti 0] ] $ATS_EVENT_DATA] 0]
			if {([string first "INF" [lindex $checkTime 2]] == -1) &&
				( [lindex $checkTime 0] != "REL" ) &&
				($condName != "TAT_SYNCHRONIZATION")} {
				lappend tmpLst "(id->clock > [TCommProc::changeExpr [lindex $checkTime 2]])"
			}

			if { ($active>1) && ($br==$active) } then {
				set br 0
			}
			
			set njs 0
			if { [llength $ti] > 2 } {
				set njs [lindex $ti 2]
			}
			_active_event [lindex $ti 0] [lindex $TCommProc::evList [lindex $ti 0] ]  [lindex $ti 1] $active $branchID $br $njs
		} else {
            puts $TCommProc::tSourceFile "\t\tid->nextState = -1;"
		}
        incr i

        if {$active>1} then {
            puts $TCommProc::tSourceFile "\t\t\tbreak;"
            puts $TCommProc::tSourceFile "\t\t\};"
        }

#        if {$active>1} then {
#            if {$br==$active} then {
#                set br 0
#            }
#            puts $TCommProc::tSourceFile "\t\t\tid->testCaseBranches\[$branchID\]=$br;"
#            puts $TCommProc::tSourceFile "\t\t\tbreak;"
#            puts $TCommProc::tSourceFile "\t\t\};"
#        }
    }
    
    set activeTimeCond [join $tmpLst " && "]

    if {$active>1} then {
        puts $TCommProc::tSourceFile "\t\t\}"
    }

    set evCond [lindex $[lindex $TCommProc::evList [lindex $ti 0] ] $ATS_EVENT_COND]
    if { ($active == 1) && ($evCond != "") } {
        puts $TCommProc::tSourceFile "\tif ( !([TCommProc::changeExpr $evCond]) ) \{"
        puts $TCommProc::tSourceFile "\t\tid->nextState=[lindex $ti 1];"
        puts $TCommProc::tSourceFile "\t\};"
    }

    if { ($active>1) && ($activeTimeCond != "") } then {
        puts $TCommProc::tSourceFile "\tif( ${activeTimeCond} ) "
        puts $TCommProc::tSourceFile "\t\{"
        puts $TCommProc::tSourceFile "\t\tid->testCaseBranches\[$branchID\] = (id->testCaseBranches\[$branchID\]+1) % 3;"
        puts $TCommProc::tSourceFile "\t\}"
    }

    if { $TConfVars::timeCheck == 1 } then {
        set globTimeCond ""
        set tmpLst [list]
        if { $activeTimeCond != "" } then {
            lappend tmpLst "($activeTimeCond)"
        }
        if { $passiveTimeCond != "" } then {
            lappend tmpLst "($passiveTimeCond)"
        }
        if { $logicalTimeCond != "" } then {
            lappend tmpLst "($logicalTimeCond)"
        }
        set globTimeCond [join $tmpLst " && "]
        if { $globTimeCond != "" } then {
            puts $TCommProc::tSourceFile "\tif( ${globTimeCond} ) "
            puts $TCommProc::tSourceFile "\t\{"
            puts $TCommProc::tSourceFile "\t\tid->testStatus = TAT_ERR_TIMEOUT;"
            puts $TCommProc::tSourceFile "\t\tid->testResult = TAT_TEST_FINISHED;"
            puts $TCommProc::tSourceFile "\t\tif (id->dumpEvent(id, NULL,NULL)!=TAT_TEST_CONTINUE)"
            puts $TCommProc::tSourceFile "\t\t\treturn TAT_TEST_CRITICAL;"
            puts $TCommProc::tSourceFile "\t\}"
        }
    }
    puts $TCommProc::tSourceFile "\treturn TAT_TEST_FINISHED;"
    puts $TCommProc::tSourceFile "\}"
}

proc IfTag {tag} {
    puts $TCommProc::tSourceFile "\tif (id->tag==${tag})\{"
}

proc EndTag {args} {
    puts $TCommProc::tSourceFile "\t\}"
}

proc EventTree {logicals timers passive active branchID args} {
    if {$TCommProc::ATSmode != 0} then {
        TCommProc::genAbort "ERROR: ats: file '$TConfVars::ATSFile': contains mix of structures\n"
    }
}

proc genWrapper {} {

	global SignalSet InstanceSet argv
	global SIGNAL_TYPE SIGNAL_RECORD_SIZE SIGNAL_PARAMS SIGNAL_NAME SIGNAL_FIELDS SIGNAL_STYPES
	global INSTANCE_NAME INSTANCE_TYPE INSTANCE_RECORD_SIZE

	if { [catch {set fpWrapperC [open [file join $TConfVars::outputDir ${TConfVars::genWrapper}.c] w ] } res ] } then {
		puts "ERROR: file: '${TConfVars::genWrapper}.c' can not open for writing\n$res"
		exit
	}

	if { [catch {set fpWrapperH [open [file join $TConfVars::outputDir ${TConfVars::genWrapper}.h] w ] } res ] } then {
		puts "ERROR: file: '${TConfVars::genWrapper}.h' can not open for writing\n$res"
		exit
	}

	#output header part
	puts $fpWrapperH "#ifndef [string toupper ${TConfVars::genWrapper}]_H"
	puts $fpWrapperH "#define [string toupper ${TConfVars::genWrapper}]_H"
	puts $fpWrapperH "/******************************************************************************"
	puts $fpWrapperH "*"
	puts $fpWrapperH "*       COPYRIGHT 2001-2004 MOTOROLA, ALL RIGHTS RESERVED"
	puts $fpWrapperH "*"
	puts $fpWrapperH "*       The code is the property of Motorola St.Petersburg Software Development"
	puts $fpWrapperH "*       and is Motorola Confidential Proprietary Information."
	puts $fpWrapperH "*"
	puts $fpWrapperH "*       The copyright notice above does not evidence any"
	puts $fpWrapperH "*       actual or intended publication of such source code."
	puts $fpWrapperH "*"
	puts $fpWrapperH "* Functions:    "
	puts $fpWrapperH "*"
	puts $fpWrapperH "* History:      Use the ClearCase command \"History\""
	puts $fpWrapperH "*               to display revision history information."
	puts $fpWrapperH "*               "
	puts $fpWrapperH "* Description:  Test suite \"${TCommProc::testSuite}\" wrapper header"
	puts $fpWrapperH "*"
	puts $fpWrapperH "* Notes:        None"
	puts $fpWrapperH "*"
	puts $fpWrapperH "******************************************************************************/\n"
	puts $fpWrapperH "#include \"${TCommProc::testSuite}.h\""
	puts $fpWrapperH "#include <txtlog.h>"
	puts $fpWrapperH "#include <mprlog.h>\n"

	puts $fpWrapperC "/******************************************************************************"
	puts $fpWrapperC "*"
	puts $fpWrapperC "*       COPYRIGHT 2001-2003 MOTOROLA, ALL RIGHTS RESERVED"
	puts $fpWrapperC "*"
	puts $fpWrapperC "*       The code is the property of Motorola St.Petersburg Software Development"
	puts $fpWrapperC "*       and is Motorola Confidential Proprietary Information."
	puts $fpWrapperC "*"
	puts $fpWrapperC "*       The copyright notice above does not evidence any"
	puts $fpWrapperC "*       actual or intended publication of such source code."
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Functions:    "
	puts $fpWrapperC "*"
	puts $fpWrapperC "* History:      Use the ClearCase command \"History\""
	puts $fpWrapperC "*               to display revision history information."
	puts $fpWrapperC "*               "
	puts $fpWrapperC "* Description:  Test suite \"${TCommProc::testSuite}\" wrapper functions"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Notes:        None"
	puts $fpWrapperC "*"
	puts $fpWrapperC "******************************************************************************/"

	puts $fpWrapperC "/* Standart include files */"
	puts $fpWrapperC "#include \"${TConfVars::genWrapper}.h\"\n"

	puts $fpWrapperH "/* User include files */"
	if { [llength $TConfVars::genInclWrapper] != 0} {
		foreach inclF $TConfVars::genInclWrapper {
			puts $fpWrapperC "#include <${inclF}>"
		}
		puts $fpWrapperC ""
	}

	puts $fpWrapperC "/* TODO: Add necessary include files */\n"

	puts $fpWrapperC "TAT_TEST_INSTANCE * TI_ID;"
	puts $fpWrapperC "static TAT_TIME_TYPE ${TCommProc::testSuite}_Clock=0;\n"
	if {$TConfVars::SDL} {
		puts $fpWrapperC "static SDL_Time ${TCommProc::testSuite}_SDLClock=\{0,0\};"
		puts $fpWrapperC "int SDL_Output_Count=0;\n"
	}
	
	# SDL support
	if {$TConfVars::SDL} {
		set instCount [expr [llength $InstanceSet] / $INSTANCE_RECORD_SIZE]
		if {$TConfVars::DefaultEnv==""} {
			for {set i 0} {$i<$instCount} {incr i} {
				if {[lindex $InstanceSet [expr $i * $INSTANCE_RECORD_SIZE + $INSTANCE_TYPE]]=="env"} {
					set TConfVars::DefaultEnv [lindex $InstanceSet [expr $i * $INSTANCE_RECORD_SIZE + $INSTANCE_NAME]]
				}
			}
			if {$TConfVars::DefaultEnv==""} {
				puts "ERROR: found no environment instances"
				exit
			} else {
				puts "Warning: default environment instance not specified, using $TConfVars::DefaultEnv"
			}
		}
		if {$TConfVars::DefaultModel==""} {
			for {set i 0} {$i<$instCount} {incr i} {
				if {[lindex $InstanceSet [expr $i * $INSTANCE_RECORD_SIZE + $INSTANCE_TYPE]]=="model"} {
					set TConfVars::DefaultModel [lindex $InstanceSet [expr $i * $INSTANCE_RECORD_SIZE + $INSTANCE_NAME]]
				}
			}
			if {$TConfVars::DefaultModel==""} {
				puts "ERROR: found no model instances"
				exit
			} else {
				puts "Warning: default model instance not specified, using $TConfVars::DefaultModel"
			}
		}
		genSDLEnvFunctions $fpWrapperC
	}
	
	# SDL system reset support
	if {$TConfVars::SDL && ($TConfVars::Reset!="")} {
		puts $fpWrapperC "/* Signal to reset SDL system */"
		puts $fpWrapperC "TAT_INSTANCE_ID ${TCommProc::testSuite}_resetSignalInstances\[\]=\{TAT_I_${TConfVars::DefaultEnv}_ID,TAT_I_${TConfVars::DefaultModel}_ID\};"
		puts $fpWrapperC "TAT_EVENT_INSTANCE ${TCommProc::testSuite}_resetSignal;"
	}
	puts $fpWrapperC "/************************************************************************************"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Function    : ${TCommProc::testSuite}_resetSys"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Description : This function invoked from test to initialize testing system"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Return      : TAT_TEST_CONTINUE - if all OK"
	puts $fpWrapperC "*               TAT_TEST_CRITICAL - if something wrong"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Notes       : "
	puts $fpWrapperC "*"
	puts $fpWrapperC "*************************************************************************************/"
	puts $fpWrapperC "TAT_TEST_RESULT ${TCommProc::testSuite}_resetSys(TAT_TEST_INSTANCE *id)"
	puts $fpWrapperC "\{"
	puts $fpWrapperC "\tint res = 0;\n"
	puts $fpWrapperC "\t${TCommProc::testSuite}_Clock=0;\n"
	if {$TConfVars::SDL} {
		puts $fpWrapperC "\t/* Reset SDL time */"
		puts $fpWrapperC "\t${TCommProc::testSuite}_SDLClock.s=0;"
		puts $fpWrapperC "\t${TCommProc::testSuite}_SDLClock.ns=0;\n"
		if {$TConfVars::Reset!=""} {
			puts $fpWrapperC "\t/* Send SDL system reset signal */"
			puts $fpWrapperC "\t${TCommProc::testSuite}_resetSignal.eventType=TAT_SEND_EV;"
			puts $fpWrapperC "\t${TCommProc::testSuite}_resetSignal.eventID=TAT_S_${TConfVars::Reset}_ID;"
			puts $fpWrapperC "\t${TCommProc::testSuite}_resetSignal.eventInstanceSize=2;"
			puts $fpWrapperC "\t${TCommProc::testSuite}_resetSignal.eventInstance=(TAT_INSTANCE_ID*)&${TCommProc::testSuite}_resetSignalInstances;"
			puts $fpWrapperC "\tres=id->sendMessage(id,&${TCommProc::testSuite}_resetSignal);\n"
		}
	}
	puts $fpWrapperC "\t/* TODO: Add necessary functionality to initialize testing system */\n"
	puts $fpWrapperC "\tif ( res != 0 )"
	puts $fpWrapperC "\t\{"
	puts $fpWrapperC "\t\tid->testStatus = TAT_ERR_RESET_IUT;"
	puts $fpWrapperC "\t\treturn TAT_TEST_CRITICAL;"
	puts $fpWrapperC "\t\}"
	puts $fpWrapperC "\telse"
	puts $fpWrapperC "\t\treturn TAT_TEST_CONTINUE;"
	puts $fpWrapperC "\}\n"

	puts $fpWrapperC "/************************************************************************************"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Function    : ${TCommProc::testSuite}_clockF"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Description : This function invoked from test to get current time value"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Return      : Current time value"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Notes       : "
	puts $fpWrapperC "*"
	puts $fpWrapperC "*************************************************************************************/"
	puts $fpWrapperC "TAT_TIME_TYPE ${TCommProc::testSuite}_clockF (TAT_TEST_INSTANCE *id)"
	puts $fpWrapperC "\{"
	puts $fpWrapperC "\t/* TODO: Add necessary functionality to determine current time */\n"
	puts $fpWrapperC "\treturn ${TCommProc::testSuite}_Clock;"
	puts $fpWrapperC "\}\n"

	puts $fpWrapperC "/************************************************************************************"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Function    : ${TCommProc::testSuite}_startCase"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Description : This function invoked at the begining of test case"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Return      : TAT_TEST_CONTINUE - if all OK"
	puts $fpWrapperC "*               TAT_TEST_CRITICAL - if something wrong"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Notes       : "
	puts $fpWrapperC "*"
	puts $fpWrapperC "*************************************************************************************/"
	puts $fpWrapperC "TAT_TEST_RESULT ${TCommProc::testSuite}_startCase (TAT_TEST_INSTANCE *id)"
	puts $fpWrapperC "\{"
	puts $fpWrapperC "\t/* TODO: Add necessary functionality to execute */"
	puts $fpWrapperC "\t/* on the begining of each test case running    */\n"
	puts $fpWrapperC "\treturn TAT_TEST_CONTINUE;"
	puts $fpWrapperC "\}\n"

	puts $fpWrapperC "/************************************************************************************"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Function    : ${TCommProc::testSuite}_finishCase"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Description : This function invoked at the ending of test case"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Return      : TAT_TEST_CONTINUE - if all OK"
	puts $fpWrapperC "*               TAT_TEST_CRITICAL - if something wrong"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Notes       : "
	puts $fpWrapperC "*"
	puts $fpWrapperC "*************************************************************************************/"
	puts $fpWrapperC "TAT_TEST_RESULT ${TCommProc::testSuite}_finishCase (TAT_TEST_INSTANCE *id)"
	puts $fpWrapperC "\{"
	puts $fpWrapperC "\t/* TODO: Add necessary functionality to execute */"
	puts $fpWrapperC "\t/* on the ending of each test case running    */\n"
	puts $fpWrapperC "\treturn TAT_TEST_CONTINUE;"
	puts $fpWrapperC "\}\n"

	set sigNumber [expr [llength $SignalSet] / $SIGNAL_RECORD_SIZE ]

	#output function for processing send message events
	puts $fpWrapperC "/************************************************************************************"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Function    : ${TCommProc::testSuite}_sendMess"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Description : This function invoked from test to send signal to testing system"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Return      : TAT_TEST_CONTINUE - if all OK"
	puts $fpWrapperC "*               TAT_TEST_CRITICAL - if something wrong"
	puts $fpWrapperC "*"
	puts $fpWrapperC "* Notes       : "
	puts $fpWrapperC "*"
	puts $fpWrapperC "*************************************************************************************/"
	puts $fpWrapperC "TAT_TEST_RESULT ${TCommProc::testSuite}_sendMess(TAT_TEST_INSTANCE *id, TAT_EVENT_INSTANCE *event)"
	puts $fpWrapperC "\{"
	puts $fpWrapperC "\tint res = 0;\n"
	if {$TConfVars::SDL} {
		puts $fpWrapperC "\txSignalNode SignalIn;"
		puts $fpWrapperC "\tSDL_PId pid;\n"
	}
	puts $fpWrapperC "\tif (!id || !event)"
	puts $fpWrapperC "\t\{"
	puts $fpWrapperC "\t\treturn TAT_TEST_CRITICAL;"
	puts $fpWrapperC "\t\}\n"
	puts $fpWrapperC "\tif (event->eventType == TAT_SEND_EV)"
	puts $fpWrapperC "\t\{"
	if {$TConfVars::SDL} {
		puts $fpWrapperC "\t\tpid.GlobalNodeNr=event->eventInstance\[TAT_INST_FROM\];"
		puts $fpWrapperC "\t\tpid.LocalPId=NULL;"
	}
	puts $fpWrapperC "\t\tswitch (event->eventID)"
	puts $fpWrapperC "\t\t\{"

	#generate function sendMess to input signals to testing system
	for {set sigCounter 0} { $sigCounter < $sigNumber } { incr sigCounter } {

		#extract only input signals
		#adp033c: added "inout" signals
		set sgType [lindex $SignalSet [expr $sigCounter * $SIGNAL_RECORD_SIZE + $SIGNAL_TYPE]]
       		if {($sgType=="in")||($sgType=="inout")} {

			set signalName [lindex $SignalSet [expr $sigCounter * $SIGNAL_RECORD_SIZE + $SIGNAL_NAME]]
			set signalParamFieldLst [lindex $SignalSet [expr $sigCounter * $SIGNAL_RECORD_SIZE + $SIGNAL_FIELDS]]
			set signalParamFieldCount [llength $signalParamFieldLst]
			set signalParamTypeLst [lindex $SignalSet [expr $sigCounter * $SIGNAL_RECORD_SIZE + $SIGNAL_PARAMS]]
			set signalParamSTypeLst [lindex $SignalSet [expr $sigCounter * $SIGNAL_RECORD_SIZE + $SIGNAL_STYPES]]

	    		puts $fpWrapperC "\t\tcase TAT_S_${signalName}_ID:"
			puts $fpWrapperC "\t\t\t\{"

			# SDL support
			if {$TConfVars::SDL} {
				puts $fpWrapperC "\t\t\t\tSignalIn=xGetSignal($signalName,xNotDefPId,pid);"
				foreach param $signalParamFieldLst {
					if {$TConfVars::SDL==1} {
						puts $fpWrapperC "\t\t\t\t((yPDef_${signalName} *)(SignalIn))->${param}=((TAT_SIG_${signalName}*)event->eventData)->${param};"
					} elseif {$TConfVars::SDL==2} {
						puts $fpWrapperC "\t\t\t\t((ySignalPar_${signalName} *)(SignalIn))->${param}=((TAT_SIG_${signalName}*)event->eventData)->${param};"
					}
				}
				if {$TConfVars::SDL==1} {
					puts $fpWrapperC "\t\t\t\tSDL_Output(SignalIn xSigPrioPar(xDefaultSignalPrio),(xIdNode *)0);"
				} elseif {$TConfVars::SDL==2} {
					puts $fpWrapperC "\t\t\t\tSDL_Output(SignalIn xSigPrioPar(xDefaultPrioSignal),(xIdNode *)0);"
				}
				puts $fpWrapperC "\t\t\t\tSDL_Output_Count++;"
			} else {
				set paramPtrLst {}
				set paramFieldStr ""
				set paramTypeStr ""
				if {$signalParamFieldCount >0 } then {
					foreach {paramField} $signalParamFieldLst {
						lappend paramPtrLst "((TAT_SIG_${signalName} *)event->eventData)->${paramField}"
					}
					set paramFieldStr [join $paramPtrLst ",\n\t\t\t\t\t\t\t\t\t\t\t\t\t"]
					set paramTypeStr [join $signalParamTypeLst ", "]
				}

				puts $fpWrapperC "\t\t\t\tres = ${TCommProc::testSuite}_${signalName}_Handler(${paramFieldStr});"
			
				# adp033c: by default free all signal params after processing 
				foreach paramField $signalParamFieldLst paramType $signalParamTypeLst paramSType $signalParamSTypeLst {
					if {$paramSType!=-1} {
						puts $fpWrapperC "\t\t\t\tTAT_Free_${paramType}(&(((TAT_SIG_${signalName} *)event->eventData)->${paramField}));"
					}
				}
				
				#generating extern definition to include
				puts $fpWrapperH "extern int ${TCommProc::testSuite}_${signalName}_Handler(${paramTypeStr});"
			}
			
			puts $fpWrapperC "\t\t\t\tbreak;"
			puts $fpWrapperC "\t\t\t\}"
		}
	}
	puts $fpWrapperH "\n"

	puts $fpWrapperC "\t\tdefault:"
	puts $fpWrapperC "\t\t\t\{"
	puts $fpWrapperC "\t\t\t\tputs(\"ERROR: (wrapper) unknown signal to transmit to model.\");"
	puts $fpWrapperC "\t\t\t\tres = 1;"
	puts $fpWrapperC "\t\t\t\}"
	puts $fpWrapperC "\t\t\}"
	puts $fpWrapperC "\t\}\n"
	puts $fpWrapperC "\tif ( res != 0 )"
	puts $fpWrapperC "\t\{"
	puts $fpWrapperC "\t\tid->testStatus = TAT_ERR_SEND_EVENT;"
	puts $fpWrapperC "\t\treturn TAT_TEST_CRITICAL;"
	puts $fpWrapperC "\t\}"
	puts $fpWrapperC "\telse"
	puts $fpWrapperC "\t\treturn TAT_TEST_CONTINUE;"
	puts $fpWrapperC "\}\n"

	#generate structures declaration and initializing
	for {set sigCounter 0} { $sigCounter < $sigNumber } { incr sigCounter } {

		#extract only output signals
		#adp033c: added "inout" signals
		set sgType [lindex $SignalSet [expr $sigCounter * $SIGNAL_RECORD_SIZE + $SIGNAL_TYPE]]
        	if {($sgType=="out")||($sgType=="inout")} {
			set signalParamTypeLst [lindex $SignalSet [expr $sigCounter * $SIGNAL_RECORD_SIZE + $SIGNAL_PARAMS]]
			set signalParamTypeCount [llength $signalParamTypeLst]
			set signalName [lindex $SignalSet [expr $sigCounter * $SIGNAL_RECORD_SIZE + $SIGNAL_NAME]]
			set sigParamPtr "NULL"
			if {$signalParamTypeCount >0 } then {
				set paramInitLst ""
				foreach {paramType} $signalParamTypeLst {
					lappend paramInitLst "0"
				}
				set sigParams [join $paramInitLst ", "]
				puts $fpWrapperH "\nextern TAT_SIG_${signalName} model_sig${signalName};"
				puts $fpWrapperC "TAT_SIG_${signalName} model_sig${signalName} = \{ $sigParams \};"
				set sigParamPtr "&model_sig${signalName}"
			}
            
			puts $fpWrapperH "extern TAT_EVENT_INSTANCE model_ev${signalName};\n"
			puts $fpWrapperC "TAT_INSTANCE_ID instances_ev${signalName}\[\] = \{0, 0\};"
			puts $fpWrapperC "TAT_EVENT_INSTANCE model_ev${signalName} = \{\
												TAT_RECV_EV, TAT_S_${signalName}_ID, ${sigParamPtr}, {{0,0}, {0,0}, \"\"},2,\n\
												instances_ev${signalName}, NULL, 0, NULL, TAT_VERDICT_PASS, -1, NULL, NULL\};\n"
		}
	}

	#generate functions for receiving signals from testing system to test
	for {set sigCounter 0} { $sigCounter < $sigNumber } { incr sigCounter } {

		#extract only output signals
		#adp033c: added "inout" signals
		set sgType [lindex $SignalSet [expr $sigCounter * $SIGNAL_RECORD_SIZE + $SIGNAL_TYPE]]
		if {($sgType=="out")||($sgType=="inout")} {

			set signalName [lindex $SignalSet [expr $sigCounter * $SIGNAL_RECORD_SIZE + $SIGNAL_NAME]]
			set signalParamTypeLst [lindex $SignalSet [expr $sigCounter * $SIGNAL_RECORD_SIZE + $SIGNAL_PARAMS]]
			set signalParamTypeCount [llength $signalParamTypeLst]
			set signalParamFieldLst [lindex $SignalSet [expr $sigCounter * $SIGNAL_RECORD_SIZE + $SIGNAL_FIELDS]]

			set funcParamLst {}
			set funcParamStr ""
			set funcParamSetLst {}
			set funcParamSetStr ""
			if {$signalParamTypeCount >0 } then {
				set paramInitLst ""
				for {set paramCount 0} { $paramCount < $signalParamTypeCount } { incr paramCount } {
					lappend funcParamLst "[lindex $signalParamTypeLst $paramCount] [lindex $signalParamFieldLst $paramCount]"
					append funcParamSetStr "\tmodel_sig${signalName}.[lindex $signalParamFieldLst $paramCount] = [lindex $signalParamFieldLst $paramCount];\n"
				}
			}
			lappend funcParamLst "int from"
			lappend funcParamLst "int to"
			set funcParamStr [join $funcParamLst ", "]
			lappend signalParamTypeLst "int"
			lappend signalParamTypeLst "int"
			puts $fpWrapperH "int ${TCommProc::testSuite}_Recv_${signalName}([join $signalParamTypeLst ", "]);"

			puts $fpWrapperC "/************************************************************************************"
			puts $fpWrapperC "*"
			puts $fpWrapperC "* Function    : ${TCommProc::testSuite}_Recv_${signalName}"
			puts $fpWrapperC "*"
			puts $fpWrapperC "* Description : This function transfer signals from testing system to test"
			puts $fpWrapperC "*"
			puts $fpWrapperC "* Return      : 1"
			puts $fpWrapperC "*"
			puts $fpWrapperC "* Notes       : "
			puts $fpWrapperC "*"
			puts $fpWrapperC "*************************************************************************************/"
			puts $fpWrapperC "int ${TCommProc::testSuite}_Recv_${signalName}(${funcParamStr})"
			puts $fpWrapperC "\{"
			puts -nonewline $fpWrapperC $funcParamSetStr
			puts $fpWrapperC "\tinstances_ev${signalName}\[0\] = from;"
			puts $fpWrapperC "\tinstances_ev${signalName}\[1\] = to;"
			puts $fpWrapperC "\tTAT_ReceiveMessage(TI_ID, &model_ev${signalName});"
			puts $fpWrapperC "\treturn 1;"
			puts $fpWrapperC "\}\n"
		}
	}
	# SDL support - generate passive wrapper
	if {$TConfVars::SDL} {
		puts $fpWrapperC "TAT_TEST_RESULT res;\n"

		puts $fpWrapperC "int WrapInit()"
		puts $fpWrapperC "\{"
		puts $fpWrapperC "\tTI_ID = TAT_CreateTestInstance(128, 3, 4096, ${TCommProc::testSuite}_resetSys,\n\
										    \t\t\t\t\t\t\t\t\t\t\t\t ${TCommProc::testSuite}_sendMess,\n\
										    \t\t\t\t\t\t\t\t\t\t\t\t ${TCommProc::testSuite}_clockF,\n\
										    \t\t\t\t\t\t\t\t\t\t\t\t ${TCommProc::testSuite}_startCase,\n\
										    \t\t\t\t\t\t\t\t\t\t\t\t ${TCommProc::testSuite}_finishCase);"
		puts $fpWrapperC "\tif (!TI_ID)"
		puts $fpWrapperC "\t\{"
		puts $fpWrapperC "\t\tprintf(\"ERROR: can't initialize test instance\\n\");"
		puts $fpWrapperC "\t\treturn 1;"
		puts $fpWrapperC "\t\}\n"

		puts $fpWrapperC "\t/* Register dumping modules */"
		puts $fpWrapperC "\tTAT_RegisterLogFunc(TI_ID, TAT_OpenLogMPR, TAT_LogFuncMPR, TAT_CloseLogMPR);"
		puts $fpWrapperC "\tTAT_RegisterLogFunc(TI_ID, TAT_OpenLogTXT, TAT_LogFuncTXT, TAT_CloseLogTXT);\n"

		puts $fpWrapperC "\t/* Call generated function to additional initializing test instance structure */"
		puts $fpWrapperC "\tres = TAT_StartSuite(TI_ID, TAT_Init_${TCommProc::testSuite});\n"
	
		puts $fpWrapperC "\treturn (res==TAT_TEST_CRITICAL);"
		puts $fpWrapperC "\}\n"

		puts $fpWrapperC "int WrapRun()"
		puts $fpWrapperC "\{"
		puts $fpWrapperC "\t/* Run test suite */"
		puts $fpWrapperC "\tif(res == TAT_TEST_CONTINUE) \{"
		puts $fpWrapperC "\t\tres = TAT_ContinueSuite(TI_ID);"
		puts $fpWrapperC "\t\treturn 0;"
		puts $fpWrapperC "\t\}"
		puts $fpWrapperC "\treturn 1;"
		puts $fpWrapperC "\}\n"
	
		puts $fpWrapperC "int WrapClose()"
		puts $fpWrapperC "\{"
		puts $fpWrapperC "\tTAT_FreeTestInstance(TI_ID);"
		puts $fpWrapperC "\treturn (res==TAT_TEST_CRITICAL);"
		puts $fpWrapperC "\}"
	} else {
		puts $fpWrapperC "int main(int argc, char *argv\[\])"
		puts $fpWrapperC "\{"
		puts $fpWrapperC "\tTAT_TEST_RESULT res;\n"

		puts $fpWrapperC "\t/* Create and initialize test instance structure */"
		puts $fpWrapperC "\tTI_ID = TAT_CreateTestInstance(128, 3, 4096, ${TCommProc::testSuite}_resetSys,\n\
										    \t\t\t\t\t\t\t\t\t\t\t\t ${TCommProc::testSuite}_sendMess,\n\
										    \t\t\t\t\t\t\t\t\t\t\t\t ${TCommProc::testSuite}_clockF,\n\
										    \t\t\t\t\t\t\t\t\t\t\t\t ${TCommProc::testSuite}_startCase,\n\
										    \t\t\t\t\t\t\t\t\t\t\t\t ${TCommProc::testSuite}_finishCase);"
		puts $fpWrapperC "\tif (!TI_ID)"
		puts $fpWrapperC "\t\{"
		puts $fpWrapperC "\t\tprintf(\"ERROR: can't initialize test instance\\n\");"
		puts $fpWrapperC "\t\treturn 1;"
		puts $fpWrapperC "\t\}\n"

		puts $fpWrapperC "\t/* Register dumping modules */"
		puts $fpWrapperC "\tTAT_RegisterLogFunc(TI_ID, TAT_OpenLogMPR, TAT_LogFuncMPR, TAT_CloseLogMPR);"
		puts $fpWrapperC "\tTAT_RegisterLogFunc(TI_ID, TAT_OpenLogTXT, TAT_LogFuncTXT, TAT_CloseLogTXT);\n"

		puts $fpWrapperC "\t/* Call generated function to additional initializing test instance structure */"
		puts $fpWrapperC "\tres = TAT_StartSuite(TI_ID, TAT_Init_${TCommProc::testSuite});\n"

		puts $fpWrapperC "\t/* Run test suite */"
		puts $fpWrapperC "\twhile (res == TAT_TEST_CONTINUE)"
		puts $fpWrapperC "\t\{"
		puts $fpWrapperC "\t\tres = TAT_ContinueSuite(TI_ID);\n"
		puts $fpWrapperC "\t\t/* TODO: Realize functionality to transfer of control to testing system */"
		puts $fpWrapperC "\t\t/*       to get response to test`s action.                              */\n"
		puts $fpWrapperC "\t\}\n"

		puts $fpWrapperC "\tif (res == TAT_TEST_CRITICAL)"
		puts $fpWrapperC "\t\{"
		puts $fpWrapperC "\t\tprintf(\"ERROR: critical error during execution\\n\");"
		puts $fpWrapperC "\t\}\n"

		puts $fpWrapperC "\t/* Free test instance structure */"
		puts $fpWrapperC "\tTAT_FreeTestInstance(TI_ID);"
		puts $fpWrapperC "\treturn (res==TAT_TEST_CRITICAL);"
		puts $fpWrapperC "\}"
	}

	puts $fpWrapperH "\n#endif /* [string toupper ${TConfVars::genWrapper}]_H */"
	close $fpWrapperC
	close $fpWrapperH
}

proc genSDLEnvFunctions {channel} {
	global SignalSet InstanceSet argv
	global SIGNAL_NAME
	global SIGNAL_TYPE
	global SIGNAL_ATTR
	global SIGNAL_PARAMS
	global SIGNAL_STYPES
	global SIGNAL_FIELDS
	global SIGNAL_RECORD_SIZE
	global INSTANCE_NAME
	global INSTANCE_TYPE
	global INSTANCE_ATTR
	global INSTANCE_PARAMS
	global INSTANCE_STYPES
	global INSTANCE_FIELDS
	global INSTANCE_PARAMTYPE
	global INSTANCE_VARARG
	global INSTANCE_PRIORITY
	global INSTANCE_RECORD_SIZE

	puts $channel "#ifdef XTENV"
	puts $channel "#define RETURN return SDL_Time_Lit((xint32)0,(xint32)0);"
	puts $channel "#else"
	puts $channel "#define RETURN return;"
	puts $channel "#endif\n"
	
	puts $channel "SDL_Time SDL_Clock(void)"
	puts $channel "\{"
	puts $channel "\treturn ${TCommProc::testSuite}_SDLClock;"
	puts $channel "\}\n"

	puts $channel "void SDL_Tick(SDL_Time T)"
	puts $channel "\{"
	puts $channel "\tif(T.s>${TCommProc::testSuite}_SDLClock.s || (T.s==${TCommProc::testSuite}_SDLClock.s && T.ns>${TCommProc::testSuite}_SDLClock.ns)) \{"
	puts $channel "\t\t${TCommProc::testSuite}_SDLClock.s++;"
	puts $channel "\t\t${TCommProc::testSuite}_SDLClock.ns=0;"
	puts $channel "\t\}"
	puts $channel "\tif(T.s<${TCommProc::testSuite}_SDLClock.s || (T.s==${TCommProc::testSuite}_SDLClock.s && T.ns<${TCommProc::testSuite}_SDLClock.ns)) ${TCommProc::testSuite}_SDLClock=T;"
	puts $channel "#ifdef TAT_FLOAT_TIME"
	puts $channel "\t${TCommProc::testSuite}_Clock=(float)(${TCommProc::testSuite}_SDLClock.s)+(float)(${TCommProc::testSuite}_SDLClock.ns)/1000000000.0;"
	puts $channel "#else"
	puts $channel "\t${TCommProc::testSuite}_Clock=${TCommProc::testSuite}_SDLClock.s;"
	puts $channel "\tif(${TCommProc::testSuite}_SDLClock.ns>=500000000) ${TCommProc::testSuite}_Clock++;"
	puts $channel "#endif"
	puts $channel "\}\n"
	
	puts $channel "#ifndef XNOGLOBALNODENUMBER"
	puts $channel "extern int xGlobalNodeNumber(void) \{return TAT_I_${TConfVars::DefaultModel}_ID;\}"
	puts $channel "#endif\n"

	puts $channel "#ifndef XNOINITENV"
	puts $channel "extern void xInitEnv(void) \{WrapInit();\}"
	puts $channel "#endif\n"

	puts $channel "#ifndef XNOCLOSEENV"
	puts $channel "extern void xCloseEnv(void) \{WrapClose();\}"
	puts $channel "#endif\n"

	puts $channel "extern void xOutEnv( xSignalNode *SignalOut )\n\{"
	puts $channel "\tTAT_INSTANCE_ID TAT_RECEIVER=TAT_I_${TConfVars::DefaultEnv}_ID;"
	puts $channel "\tif((*SignalOut)->Receiver.GlobalNodeNr!=xEnv.GlobalNodeNr) TAT_RECEIVER=(*SignalOut)->Receiver.GlobalNodeNr;"
	set signalCount [expr [llength $SignalSet] / $SIGNAL_RECORD_SIZE]
	for {set i 0} {$i<$signalCount} {incr i} {
		set sgname [lindex $SignalSet [expr $i * $SIGNAL_RECORD_SIZE + $SIGNAL_NAME]]
		set sgtype [lindex $SignalSet [expr $i * $SIGNAL_RECORD_SIZE + $SIGNAL_TYPE]]
		set sgparams [lindex $SignalSet [expr $i * $SIGNAL_RECORD_SIZE + $SIGNAL_PARAMS]]
		set sgstypes [lindex $SignalSet [expr $i * $SIGNAL_RECORD_SIZE + $SIGNAL_STYPES]]
		set sgfields [lindex $SignalSet [expr $i * $SIGNAL_RECORD_SIZE + $SIGNAL_FIELDS]]
		if {$sgtype=="out" || $sgtype=="inout"} {
			if {${TConfVars::SDL}==1 || ${TConfVars::SDL}==2} {
				puts $channel "\tif(((*SignalOut)->NameNode)==${sgname}) \{"
				puts -nonewline $channel "\t\t${TCommProc::testSuite}_Recv_${sgname}("
				foreach field $sgfields type $sgparams stype $sgstypes {
					if {$stype==-1 } {
						puts -nonewline $channel "((yPDP_${sgname})(*SignalOut))->${field},"
					} else {
						puts -nonewline $channel "TAT_Copy_${type}(((yPDP_${sgname})(*SignalOut))->${field}),"
					}
				}
				puts $channel "TAT_I_${TConfVars::DefaultModel}_ID,TAT_RECEIVER);"
				puts $channel "\t\txReleaseSignal(SignalOut);"
				puts $channel "\t\treturn;"
				puts $channel "\t\}"
			} else {
				TCommProc::genAbort "Wrong TAU version"
			}
		}
	}
	puts $channel "\}\n"

	puts $channel "#ifndef XTENV"
	puts $channel "extern void xInEnv ( SDL_Time Time_for_next_event )"
	puts $channel "#else"
	puts $channel "extern SDL_Duration xInEnv ( SDL_Time Time_for_next_event )"
	puts $channel "#endif"
	puts $channel "\{"
	puts $channel "\tSDL_Output_Count=0;"
	puts $channel "\tif(WrapRun()) SDL_Halt();"
	puts $channel "\tif(SDL_Output_Count==0) SDL_Tick(Time_for_next_event);"
	puts $channel "\tRETURN"
	puts $channel "\}\n"
}

#display template information
puts "Code Generation Template for C - version 2.0"
puts "COPYRIGHT 2001-2003 MOTOROLA, ALL RIGHTS RESERVED\n"

#process command line
TConfVars::processCommandLine $argc $argv

#process ATS file
#source $TConfVars::ATSFile
if { [catch { source $TConfVars::ATSFile } res ] } then {
    puts "ERROR: ats: file '$TConfVars::ATSFile': can't be load\n$res"
    exit 1
}

if { $TConfVars::genWrapper != "" } {
	if { [catch { genWrapper } res ] } then {
	    puts "ERROR: wrapper: Generating wrapper unsuccessful\n$res"
	    exit 1
	}
}