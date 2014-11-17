#package require xml
package require dom
package require BWidget

namespace eval Img {
	variable Motologo
	variable scriptdir [file dirname [info script]]
	if {[catch {image create photo -file [file join $scriptdir images Moto.gif]} ::Img::Motologo]} {
		set Motologo ""
	}
}

# XML processing procedures and variables
namespace eval XML {

	# XML elements used in TAT
	variable Elements [list TATPROJECT \
		CONFIG INSTANCES INSTANCE ATTRIBUTE PARAMETER SIGNALS SIGNAL DATATYPE TYPEDEF MEMBER MACROS MACRO VALUELIST VALUE AXIOMS AXIOM INITIAL \
		SPECFILES SET FILE \
		TOOLS TOOL INPUT OUTPUT EXEC ATTACHIN ATTACHOUT TOOLCONFIG \
		TESTSUITE TESTCASE PIPE DATAIN DATAOUT\
	]
	
	# Element attributes
	variable attr

	set attr(TATPROJECT) [list name info title user leader]

	set attr(CONFIG) [list name base comment]
	set attr(INSTANCES) {}
	set attr(SIGNALS) {}
	set attr(DATATYPE) {}
	set attr(MACROS) {}
	set attr(INSTANCE) [list name type comment annotation priority]
	set attr(SIGNAL) [list name type comment annotation priority paramstype]
	set attr(TYPEDEF) [list name type dimension comment annotation paramstype prefix dataclass]
	set attr(MACRO) [list name type replace datatype comment annotation priority vector research priolist]
	set attr(VALUELIST) [list condition comment annotation]
	set attr(VALUE) [list value]
	set attr(PARAMETER) [list field type comment]
	set attr(MEMBER) [list name type comment]
	set attr(ATTRIBUTE) [list value comment]
	set attr(AXIOMS) [list initialstate]
	set attr(AXIOM) [list name axiom check]
	set attr(INITIAL) [list initial]

	set attr(SPECFILES) {}
	set attr(SET) [list name type]
	set attr(FILE) [list path]

	set attr(TOOLS) {}
	set attr(TOOL) [list name path configscript ispipe]
	set attr(INPUT) [list name type]
	set attr(OUTPUT) [list name type]
	set attr(TOOLCONFIG) [list name options]
	set attr(EXEC) [list name toolconfig]
	set attr(ATTACHIN) [list from to value]
	set attr(ATTACHOUT) [list from to value]

	set attr(TESTSUITE) {}
	set attr(TESTCASE) [list name]
	set attr(PIPE) [list name isexec]
	set attr(DATAIN) [list name value]
	set attr(DATAOUT) [list name value]

	# XML file name
	variable filename {}

	# DOM document root
	variable root {}

	# Returns XML file name
	proc getConfigFile {} {
		variable filename
		return $filename
	}

	# Returns working directory
	proc getWorkDir {} {
		variable filename
		return [file dirname $filename]
	}

	# Returns list of all available configurations in project, often used by tool config scripts
	proc getConfigList {} {
		set tatproj [lindex [::XML::findElements "" TATPROJECT {}] 0]
		if {$tatproj==""} {
			return ""
		} else {
			return [::XML::listAttrByType $tatproj CONFIG name]
		}
	}

	# Initialization.
	proc init {} {
		variable root
		variable filename
		if {$root!=""} {::dom::destroy $root}
		set filename ""
	}
	
	# Create new document
	proc Create {} {
		variable root
		init
		set root [::dom::create]
	}

	# Remove all XML nodes except elements. First call with DOM document root, then recursive over the entire DOM tree.
	# Any XML document contains text nodes (at least tabs and spaces used for text indentation) and other useless junk.
	proc Trim {parent} {
		foreach node [::dom::node children $parent] {
			if {[string compare [::dom::node cget $node -nodeType] element]} {
				::dom::node removeChild $parent $node
				::dom::destroy $node
			} else {
				Trim $node
			}
		}
	}

	# Load document from file
	proc LoadFile {name} {
		variable filename
		variable root
		init
		set filename [file normalize $name]
		if {[catch {
			cd [file dirname $name]
			set root [::dom::parse [read [set in [open "$filename" r]]]]
			close $in
		} err]} {
			tk_messageBox -title Error -message $err
			return 1
		}
		Trim $root
		return 0
	}

	# Save document to file
	proc SaveFile {name} {
		variable filename
		variable root
		set filename [file normalize $name]
		if {[catch {
			cd [file dirname $filename]
			puts -nonewline [set out [open $name w]] [::dom::serialize $root -indent yes]
			close $out
		} err]} {
			tk_messageBox -title Error -message $err
			return 1
		}
		return 0
	}
	
	# Add XML element
	proc AddElement {parent type attrlist} {
		variable root
		if {$parent==""} {set parent $root}
		set element [::dom::document createElement $parent $type]
		foreach {attrname attrvalue} $attrlist {
			::dom::element setAttribute $element $attrname $attrvalue
		}
		return $element
	}

	# Delete XML element
	proc DeleteElement {node} {
		if {$node==""} {return}
		set parent [::dom::node parent $node]
		::dom::node removeChild $parent $node
		::dom::destroy $node
	}

	# Move XML element one position up
	proc MoveUp {node} {
		set parent [::dom::node cget $node -parentNode]
		set index [lsearch [::dom::node children $parent] $node]
		if {$index>0} {
			::dom::node insertBefore $parent $node [lindex [::dom::node children $parent] [expr $index - 1]]
		}
	}

	# Move XML element one position down
	proc MoveDown {node} {
		set parent [::dom::node cget $node -parentNode]
		set index [lsearch [::dom::node children $parent] $node]
		if {$index<[expr [llength [::dom::node children $parent]] -1]} {
			::dom::node insertBefore $parent [lindex [::dom::node children $parent] [expr $index + 1]] $node
		}
	}

	# Enumerate parent's subnodes, choose elements of specified type and add their specified attribute to the list
	proc listAttrByType {parent type attr} {
		set list {}
		foreach node [::dom::node children $parent] {
			if {([::dom::node cget $node -nodeType]=="element")&&([::dom::node cget $node -nodeName]==$type)} {
				lappend list [::dom::element getAttribute $node $attr]
			}
		}
		return $list
	}

	# Find elements under a given parent element by type and attributes
	proc findElements {parent type attrlist args} {
		variable root
		if {$root==""} {return ""}
		if {$parent==""} {set parent $root}
		set elemlist {}
		foreach node [::dom::node children $parent] {
			if {[string compare [::dom::node cget $node -nodeType] element]} continue
			set flag 1
			if {[string compare [::dom::element cget $node -tagName] $type]} {
				set flag 0
			} else {
				foreach {attr value} $attrlist {
					if {[string compare [::dom::element getAttribute $node $attr] $value]} {
						set flag 0
						break
					}
				}
			}
			if {$flag} {
				lappend elemlist $node
			}
			if {[lsearch $args "-recursive"]>=0} {
				set elemlist [concat $elemlist [findElements $node $type $attrlist $args]]
			}
		}
		return $elemlist
	}
}

# Project tree procedures and variables
namespace eval ProjTree {
	variable projTree {}
	variable xScrollBar {}
	variable yScrollBar {}
	variable selectedNode {}

	# Check if XML node shall be displayed in tree view
	# ATTRIBUTEs inside TOOLCONFIG are not visible, they are used by tool config scripts only
	proc isDisplayableElement {node} {
		if {[::dom::node cget $node -nodeType]!="element"} {return 0}
		if {[::dom::node cget [::dom::node parent $node] -nodeName]=="TOOLCONFIG"} {return 0}
		return 1
	}

	# Returns text to be displayed in the tree for the specified node
	proc getNodeText {node} {
		if {[::dom::node cget $node -nodeType]!="element"} {return [::dom::node cget $node -nodeName]}
		set nodeType [::dom::element cget $node -tagName]
		set text $nodeType
		switch $nodeType {
			TATPROJECT {set text "Project [::dom::element getAttribute $node name]"}
			CONFIG {set text "Config [::dom::element getAttribute $node name]"}
			INSTANCES {set text "Instances"}
			INSTANCE {set text "[::dom::element getAttribute $node name]"}
			SIGNALS {set text "Signals"}
			SIGNAL {set text "[::dom::element getAttribute $node name]"}
			DATATYPE {set text "Data types"}
			TYPEDEF {set text "[::dom::element getAttribute $node name]"}
			MACROS {set text "Macro definitions"}
			MACRO {set text "[::dom::element getAttribute $node name]"}
			VALUELIST {set text "Valuelist [::dom::element getAttribute $node condition]"}
			VALUE {set text "<[::dom::element getAttribute $node value]>"}
			ATTRIBUTE {set text "<[::dom::element getAttribute $node value]>"}
			PARAMETER {set text "[::dom::element getAttribute $node type] [::dom::element getAttribute $node field]"}
			MEMBER {set text "[::dom::element getAttribute $node type] [::dom::element getAttribute $node name]"}
			AXIOMS {set text "Axioms"}
			AXIOM {set text "[::dom::element getAttribute $node name]"}
			INITIAL {set text "Initial: [::dom::element getAttribute $node initial]"}
			SPECFILES {set text "Specification files"}
			SET {set text [::dom::element getAttribute $node name]}
			FILE {set text [::dom::element getAttribute $node path]}
			TOOLS {set text "Tools & Pipes"}
			TOOL {
				if {[::dom::element getAttribute $node ispipe]==1} {
					set text "Pipe [::dom::element getAttribute $node name]"
				} else {
					set text "Tool [::dom::element getAttribute $node name]"
				}
			}
			INPUT {set text "Input [::dom::element getAttribute $node name]"}
			OUTPUT {set text "Output [::dom::element getAttribute $node name]"}
			TOOLCONFIG {set text "ToolConfig [::dom::element getAttribute $node name]"}
			EXEC {set text "Exec [::dom::element getAttribute $node name]"}
			ATTACHIN {set text "Input [::dom::element getAttribute $node to]<-[::dom::element getAttribute $node from]"}
			ATTACHOUT {set text "Output [::dom::element getAttribute $node from]->[::dom::element getAttribute $node to]"}
			TESTSUITE {set text "Test suite"}
			TESTCASE {set text "Test case [::dom::element getAttribute $node name]"}
			PIPE {set text "Pipe [::dom::element getAttribute $node name]"}
			DATAIN {set text "Input [::dom::element getAttribute $node name] <[::dom::element getAttribute $node value]>"}
			DATAOUT {set text "Output [::dom::element getAttribute $node name] <[::dom::element getAttribute $node value]>"}
		}
		return $text
	}

	# Update selected node's text
	proc refreshSelectedNode {} {
		variable projTree
		variable selectedNode
		if {$selectedNode!=""} {
			$projTree itemconfigure $selectedNode -text [getNodeText $selectedNode]
		}
	}

	# Callback, invoked whenever selection changes
	proc selectCommand {tree nodes} {
		variable projTree
		variable selectedNode
		Tracking 0
		if {[::PropPage::Validate]} {
			Select $selectedNode
		} else {
			::PropPage::Destroy
			refreshSelectedNode
			if {[llength $nodes]==0} {
				set selectedNode ""
			} else {
				set selectedNode [lindex $nodes 0]
				::PropPage::Create
			}
		}
		Tracking 1
	}
	
	# Create and place tree and scrollbars inside master widget
	# Note: with ScrolledWindow this could be done much easier.
	proc Init {master} {
		variable projTree
		variable xScrollBar
		variable yScrollBar
		set projTree [Tree ${master}.projTree -selectcommand ::ProjTree::selectCommand -xscrollcommand "${master}.xScroll set" -yscrollcommand "${master}.yScroll set"]
		set yScrollBar [scrollbar ${master}.yScroll -command "${master}.projTree yview"]
 		set xScrollBar [scrollbar ${master}.xScroll -command "${master}.projTree xview" -orient horizontal]
		grid $projTree $yScrollBar -sticky nsew
		grid $xScrollBar -sticky ew
		grid rowconfig $master 0 -weight 1
		grid columnconfig $master 0 -weight 1
	}

	# Internal recursive procedure for building tree from XML document
	proc buildTree {treenode xmlnode} {
		variable projTree
		if {$treenode=="" || $xmlnode==""} {return}
		foreach node [::dom::node children $xmlnode] {
			if {[isDisplayableElement $node]} {
				$projTree insert end $treenode $node -text [getNodeText $node]
				buildTree $node $node
			}
		}
	}	

	# Build tree from XML document
	proc Fill {} {
		if {$XML::root==""} {return}
		Clear
		buildTree root $XML::root
	}

	# Clear the tree (the tree widget is not destroyed)
	proc Clear {} {
		variable projTree
		variable selectedNode
		set selectedNode {}
		$projTree selection clear
		$projTree delete [$projTree nodes root]
	}

	# Change selection
	proc Select {node} {
		variable projTree
		$projTree selection set $node
		$projTree see $node
	}

	# Unfold subtree
	proc OpenTree {node} {
		variable projTree
		$projTree opentree $node 0
	}

	# Add tree element
	proc AddElement {parent node} {
		variable projTree
		$projTree insert end $parent $node -text [getNodeText $node]
		return $node
	}

	# Delete tree element
	proc DeleteElement {node} {
		variable projTree
		$projTree delete $node
	}

	# Controls if selectCommand will be invoked when selection changes
	proc Tracking {enable} {
		variable projTree
		if {$enable} {
			#Console::Dump "Tracking ON"
			$projTree configure -selectcommand ::ProjTree::selectCommand
		} else {
			#Console::Dump "Tracking OFF"
			$projTree configure -selectcommand {}
		}
	}

	# Move tree element one position up
	proc MoveUp {element} {
		variable projTree
		set parent [$projTree parent $element]
		set index [$projTree index $element]
		if {$index>0} {
			$projTree move $parent $element [expr $index - 1]
		}
	}

	# Move tree element one position down
	proc MoveDown {element} {
		variable projTree
		set parent [$projTree parent $element]
		set index [$projTree index $element]
		if {$index<[expr [llength [$projTree nodes $parent]] - 1]} {
			$projTree move $parent $element [expr $index + 1]
		}
	}

}

# User command handlers: menu commands, property page buttons, etc.
namespace eval Commands {

	proc File_New {} {
		if {$::XML::root!=""} {
			switch [tk_messageBox -title "Exit" -icon question -message "Save ${::XML::filename} ?" -type yesnocancel] {
				yes {if {![File_Save]} {return}}
				cancel {return}
			}
		}
		XML::Create
		set tatproj [XML::AddElement "" TATPROJECT {}]
		XML::AddElement $tatproj SPECFILES {}
		XML::AddElement $tatproj TOOLS {}
		XML::AddElement $tatproj TESTSUITE {}
		ProjTree::Fill
	}

	proc File_Open {} {
		if {$::XML::root!=""} {
			switch [tk_messageBox -title "Exit" -icon question -message "Save ${::XML::filename} ?" -type yesnocancel] {
				yes {if {![File_Save]} {return}}
				cancel {return}
			}
		}
		set types {{{TAT Configurations} {.xml .sic}} {{All Files} *}}
		set filename [tk_getOpenFile -filetypes $types]
		if {$filename!=""} {
			if {[::XML::LoadFile $filename]} {return 1}
			::ProjTree::Fill
			::GUI::SetTitle "$::XML::filename - TAT"
		}
		return 0
	}

	proc File_Save {} {
		if {$::XML::root==""} {return 0}
		::PropPage::SaveToXML
		if {$::XML::filename!=""} {
			if {[::XML::SaveFile $::XML::filename]} {return 0} else {return 1}
		} else {
			return [File_SaveAs]
		}
	}

	proc File_SaveAs {} {
		if {$::XML::root==""} {return 0}
		::PropPage::SaveToXML
		set types {{{TAT Configurations} {.xml .sic}} {{All Files} *}}
		set filename [tk_getSaveFile -filetypes $types -defaultextension .xml]
		if {$filename!=""} {
			if {[::XML::SaveFile $filename]} {return 0}
			::GUI::SetTitle "$::XML::filename - TAT"
			return 1
		} else {return 0}
	}

	proc File_Exit {} {
		if {$::XML::root==""} exit
		switch [tk_messageBox -title "Exit" -icon question -message "Save ${::XML::filename} ?" -type yesnocancel] {
			yes {if {[File_Save]} {exit}}
			no {exit}
			cancel {return}
		}
	}

	proc Project_GenFileLists {} {
		::Files::genAllFileSetLists
	}

	proc Project_Build {} {
		if {[File_Save]} {
			Console::Clear
			Console::Dump "Build started"
			Tools::buildProject
		} else {
			tk_messageBox -title Error -message "Failed to save project file, can't build"
		}
	}

	proc Project_Run {} {
		if {[File_Save]} {
			Console::Clear
			Console::Dump "Execution started"
			Tools::runProject
		} else {
			tk_messageBox -title Error -message "Failed to save project file, can't run"
		}
	}
	
	proc Help_About {} {
		if {$::Img::Motologo != ""} {
			set aboutdlg [Dialog ${::GUI::mainWindow}.about -title "About TAT" -image $::Img::Motologo -modal local]
		} else {
			set aboutdlg [Dialog ${::GUI::mainWindow}.about -title "About TAT" -modal local]
		}
		pack [label [$aboutdlg getframe].dlgtext -text "TAT 2.0\n\n(C) Motorola, Inc. 2006\n\nPlease forward TAT comments and questions to\nR36959@motorola.com\n+7 (812) 329 19 42"]
		$aboutdlg add -text OK -command "destroy $aboutdlg"
		$aboutdlg draw
	}

	proc delete {} {
		set node $::ProjTree::selectedNode
		::ProjTree::Tracking 0
		::PropPage::Destroy
		::ProjTree::DeleteElement $node
		set ::ProjTree::selectedNode {}
		::XML::DeleteElement $node
		::ProjTree::Tracking 1
	}

	proc up {} {
		set node $::ProjTree::selectedNode
		::XML::MoveUp $node
		::ProjTree::MoveUp $node
	}

	proc down {} {
		set node $::ProjTree::selectedNode
		::XML::MoveDown $node
		::ProjTree::MoveDown $node
	}

	proc addConfig {} {
		set list [::XML::listAttrByType $::ProjTree::selectedNode CONFIG name]
		set count 0
		if {[llength $list]==0} {
			set cname "default"
		} else {
			while {[lsearch $list "config${count}"]>=0} {incr count}
			set cname "config${count}"
		}
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode CONFIG [list name $cname base "" comment ""]]]
		::ProjTree::AddElement $node [::XML::AddElement $node INSTANCES {}]
		::ProjTree::AddElement $node [::XML::AddElement $node SIGNALS {}]
		::ProjTree::AddElement $node [::XML::AddElement $node DATATYPE {}]
		::ProjTree::AddElement $node [::XML::AddElement $node MACROS {}]
		::ProjTree::AddElement $node [::XML::AddElement $node AXIOMS {}]
		#::ProjTree::AddElement $node [::XML::AddElement $node INITIAL {}]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc addInstance {} {
		set list [::XML::listAttrByType $::ProjTree::selectedNode INSTANCE name]
		set count 0
		while {[lsearch $list "instance${count}"]>=0} {incr count}
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode INSTANCE [list name "instance${count}" type env priority 100]]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc addSignal {} {
		set list [::XML::listAttrByType $::ProjTree::selectedNode SIGNAL name]
		set count 0
		while {[lsearch $list "signal${count}"]>=0} {incr count}
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode SIGNAL [list name "signal${count}" type in priority 100 paramstype C]]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}
	
	proc addTypedef {} {
		set list [::XML::listAttrByType $::ProjTree::selectedNode TYPEDEF name]
		set count 0
		while {[lsearch $list "type${count}"]>=0} {incr count}
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode TYPEDEF [list name "type${count}" paramstype C]]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc addMacro {} {
		set list [::XML::listAttrByType $::ProjTree::selectedNode MACRO name]
		set count 0
		while {[lsearch $list "#macro${count}"]>=0} {incr count}
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode MACRO [list name "#macro${count}" type list replace replace datatype string]]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc addAttribute {} {
		set attr [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode ATTRIBUTE {}]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $attr
	}

	proc addParameter {} {
		set list [::XML::listAttrByType $::ProjTree::selectedNode PARAMETER field]
		set count 0
		while {[lsearch $list "param${count}"]>=0} {incr count}
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode PARAMETER [list field "param${count}"]]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc addMember {} {
		set list [::XML::listAttrByType $::ProjTree::selectedNode MEMBER name]
		set count 0
		while {[lsearch $list "member${count}"]>=0} {incr count}
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode MEMBER [list name "member${count}"]]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc addValuelist {} {
		set list [::XML::listAttrByType $::ProjTree::selectedNode VALUELIST condition]
		if {[llength $list]==0} {
			set vcond "@always"
		} else {
			set count 0
			while {[lsearch $list "@condition${count}"]>=0} {incr count}
			set vcond "@condition${count}"
		}
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode VALUELIST [list condition $vcond]]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc addValue {} {
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode VALUE {}]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc addAxiom {} {
		set list [::XML::listAttrByType $::ProjTree::selectedNode AXIOM name]
		set count 0
		while {[lsearch $list "axiom${count}"]>=0} {incr count}
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode AXIOM [list name "axiom${count}"]]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc addFileSet {} {
		set list [::XML::listAttrByType $::ProjTree::selectedNode SET name]
		if {[llength $list]==0} {
			set name "default"
		} else {
			set count 0
			while {[lsearch $list "set${count}"]>=0} {incr count}
			set name "set${count}"
		}
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode SET [list name $name]]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc addFiles {} {
		set filelist [tk_getOpenFile -multiple yes]
		foreach file $filelist {
			if {[file isfile $file]} {
				::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode FILE [list path [::Files::RelPath $file [::XML::getWorkDir]]]]
			}
		}
		::ProjTree::OpenTree $::ProjTree::selectedNode
	}

	proc addTool {ispipe} {
		set list [::XML::listAttrByType $::ProjTree::selectedNode TOOL name]
		set count 0
		while {[lsearch $list "tool${count}"]>=0} {incr count}
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode TOOL [list name "tool${count}" ispipe $ispipe]]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc addToolConfig {} {
		set list [::XML::listAttrByType $::ProjTree::selectedNode TOOLCONFIG name]
		set count 0
		if {[llength $list]==0} {
			set name "default"
		} else {
			set count 0
			while {[lsearch $list "config${count}"]>=0} {incr count}
			set name "config${count}"
		}
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode TOOLCONFIG [list name $name]]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc addInput {} {
		set list [::XML::listAttrByType $::ProjTree::selectedNode INPUT name]
		set count 0
		while {[lsearch $list "input${count}"]>=0} {incr count}
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode INPUT [list name "input${count}" type FILE]]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc addOutput {} {
		set list [::XML::listAttrByType $::ProjTree::selectedNode OUTPUT name]
		set count 0
		while {[lsearch $list "output${count}"]>=0} {incr count}
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode OUTPUT [list name "output${count}" type FILE]]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc addExec {} {
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode EXEC {}]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc addTestCase {} {
		set list [::XML::listAttrByType $::ProjTree::selectedNode TESTCASE name]
		set count 0
		while {[lsearch $list "testcase${count}"]>=0} {incr count}
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode TESTCASE [list name "testcase${count}"]]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc addPipe {} {
		set node [::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode PIPE {isexec 0}]]
		::ProjTree::OpenTree $::ProjTree::selectedNode
		::ProjTree::Select $node
	}

	proc browseOpen {var args} {
		upvar $var result
		if {[set tmp [eval tk_getOpenFile $args]]!=""} {set result $tmp}
	}

	proc browseSave {var args} {
		upvar $var result
		if {[set tmp [eval tk_getSaveFile $args]]!=""} {set result $tmp}
	}

	proc browseDirectory {var args} {
		upvar $var result
		if {[set tmp [eval tk_chooseDirectory $args]]!=""} {set result $tmp}
	}

	proc relative {} {
		if {$::PropPage::relative} {
			set ::PropPage::Properties(path) [::Files::RelPath $::PropPage::Properties(path) [::XML::getWorkDir]]
		} else {
			set ::PropPage::Properties(path) [file normalize [file join [::XML::getWorkDir] $::PropPage::Properties(path)]]
		}
		set ::PropPage::relative [string equal [file pathtype $::PropPage::Properties(path)] relative]
	}

	proc run_pipe {} {
		::Console::Clear
		::Tools::runPipe $::ProjTree::selectedNode
	}

	proc run_testcase {} {
		::Console::Clear
		::Tools::runTestCase $::ProjTree::selectedNode
	}
	
	proc build_testcase {} {
		::Console::Clear
		::Tools::buildTestCase $::ProjTree::selectedNode
	}	
}

# Main window procedures
namespace eval GUI {	
	variable mainMenu {
		"&File" {} {} 0 {
			{command "&New" {} "Create new project" {Ctrl n} -command Commands::File_New}
			{command "&Open..." {} "Open existing project" {Ctrl o} -command Commands::File_Open}
			{command "&Save" {} "Save current project" {Ctrl s} -command Commands::File_Save}
			{command "Sa&ve As..." {} "Save current project" {} -command Commands::File_SaveAs}
			{command "E&xit" {} "Exit TAT" {Alt x} -command Commands::File_Exit}
		}
		"&Project" {} {} 0 {
			{command "&Generate file lists" {} "Generate file lists from all file sets in project" {Ctrl g} -command Commands::Project_GenFileLists}
			{command "&Build" {} "Build project" {Ctrl b} -command Commands::Project_Build}
			{command "&Run" {} "Run project" {Ctrl r} -command Commands::Project_Run}
		}
		"&Help" {} {} 0 {
			{command "&About" {} "Show general product information" {} -command Commands::Help_About}
		}
	}
	variable mainWindow [MainFrame .main -menu $::GUI::mainMenu -separator both -textvariable statusBar]
	variable panedWindow1 [PanedWindow [$mainWindow getframe].paned -side left]
	variable upperPane [$panedWindow1 add]
	variable lowerPane [$panedWindow1 add]
	variable panedWindow2 [PanedWindow ${upperPane}.paned -side bottom]
	variable leftPane [$panedWindow2 add]
	variable rightPane [$panedWindow2 add]
	
	proc Init {} {
		global tcl_platform
		variable mainWindow
		variable panedWindow1
		variable panedWindow2
		variable leftPane
		variable rightPane
		variable lowerPane
		if {$tcl_platform(platform) == "windows"} {
			wm iconbitmap . -default [file join [file dirname [info script]] images tat.ico]
		}
		pack $mainWindow $panedWindow1 $panedWindow2 -fill both -expand yes
		ProjTree::Init $leftPane
		Console::Init $lowerPane
		wm title . "TAT"
		wm protocol . WM_DELETE_WINDOW ::Commands::File_Exit
	}

	proc SetTitle {title} {
		wm title . $title
	}
}

# Console procedures
namespace eval Console {} {
	variable consoleText

	# Initialize console
	proc Init {master} {
		variable consoleText
		set scrolled [ScrolledWindow ${master}.scrolled]
		set consoleText [text ${scrolled}.consoleText -height 3 -state disabled]
		$scrolled setwidget $consoleText
		pack $scrolled -fill both -expand yes
	}

	proc Clear {} {
		variable consoleText
		$consoleText configure -state normal
		$consoleText delete 1.0 end
		$consoleText configure -state disabled
	}
	
	proc Dump {message} {
		variable consoleText
		$consoleText configure -state normal
		$consoleText insert end ${message}\n
		$consoleText configure -state disabled
	}
}

# Properties page procedures and functions
namespace eval PropPage {
	variable propPage {}
	variable Properties
	variable IOs
	variable chan_list

	proc controls_TATPROJECT {} {
		variable propPage
		foreach attr $::XML::attr(TATPROJECT) {
			pack [label ${propPage}.label_$attr -text $attr]
			pack [entry ${propPage}.entry_$attr -textvariable ::PropPage::Properties($attr)] -fill x
		}
		pack [button ${propPage}.button_addConfig -text "Add Config" -command ::Commands::addConfig] -fill x
	}

	proc controls_CONFIG {} {
		variable propPage
		pack [label ${propPage}.label_name -text name]
		pack [entry ${propPage}.entry_name -textvariable ::PropPage::Properties(name)] -fill x
		set tatproj [::dom::node parent $::ProjTree::selectedNode]
		set base_config_list [list ""]
		foreach config [::XML::findElements $tatproj CONFIG {}] {
			if {[string compare $config $::ProjTree::selectedNode]} {
				lappend base_config_list [::dom::element getAttribute $config name]
			}
		}
		pack [label ${propPage}.label_base -text base]
		pack [ComboBox ${propPage}.combo_base -textvariable ::PropPage::Properties(base) -values $base_config_list -editable no] -fill x
		pack [label ${propPage}.label_comment -text comment]
		pack [entry ${propPage}.entry_comment -textvariable ::PropPage::Properties(comment)] -fill x
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
	}

	proc controls_INSTANCES {} {
		variable propPage
		pack [button ${propPage}.button_addInst -text "Add Instance" -command ::Commands::addInstance] -fill x
	}
	
	proc controls_INSTANCE {} {
		variable propPage
		pack [label ${propPage}.label_name -text name]
		pack [entry ${propPage}.entry_name -textvariable ::PropPage::Properties(name)] -fill x
		pack [label ${propPage}.label_type -text type]
		pack [ComboBox ${propPage}.combo_type -textvariable ::PropPage::Properties(type) -values [list env model vmodel monitor ignore] -editable no] -fill x
		pack [label ${propPage}.label_comment -text comment]
		pack [entry ${propPage}.entry_comment -textvariable ::PropPage::Properties(comment)] -fill x
		pack [label ${propPage}.label_anno -text annotation]
		pack [entry ${propPage}.entry_anno -textvariable ::PropPage::Properties(annotation)] -fill x
		pack [label ${propPage}.label_prio -text priority]
		pack [entry ${propPage}.entry_prio -textvariable ::PropPage::Properties(priority)] -fill x
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
		pack [button ${propPage}.button_addAttr -text "Add Attribute" -command ::Commands::addAttribute] -fill x
		pack [button ${propPage}.button_addParam -text "Add Parameter" -command ::Commands::addParameter] -fill x
	}	
	
	proc controls_SIGNALS {} {
		variable propPage
		pack [button ${propPage}.button_addSig -text "Add Signal" -command ::Commands::addSignal] -fill x
	}

	proc controls_SIGNAL {} {
		variable propPage
		pack [label ${propPage}.label_name -text name]
		pack [entry ${propPage}.entry_name -textvariable ::PropPage::Properties(name)] -fill x
		pack [label ${propPage}.label_type -text type]
		pack [ComboBox ${propPage}.combo_type -textvariable ::PropPage::Properties(type) -values [list in out inout] -editable no] -fill x
		pack [label ${propPage}.label_comment -text comment]
		pack [entry ${propPage}.entry_comment -textvariable ::PropPage::Properties(comment)] -fill x
		pack [label ${propPage}.label_anno -text annotation]
		pack [entry ${propPage}.entry_anno -textvariable ::PropPage::Properties(annotation)] -fill x
		pack [label ${propPage}.label_prio -text priority]
		pack [entry ${propPage}.entry_prio -textvariable ::PropPage::Properties(priority)] -fill x
		pack [label ${propPage}.label_ptype -text paramstype]
		pack [entry ${propPage}.entry_ptype -textvariable ::PropPage::Properties(paramstype)] -fill x
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
		pack [button ${propPage}.button_addAttr -text "Add Attribute" -command ::Commands::addAttribute] -fill x
		pack [button ${propPage}.button_addParam -text "Add Parameter" -command ::Commands::addParameter] -fill x
	}

	proc controls_DATATYPE {} {
		variable propPage
		pack [button ${propPage}.button_addType -text "Add Data Type" -command ::Commands::addTypedef] -fill x
	}

	proc controls_TYPEDEF {} {
		variable propPage
		pack [label ${propPage}.label_name -text name]
		pack [entry ${propPage}.entry_name -textvariable ::PropPage::Properties(name)] -fill x
		pack [label ${propPage}.label_type -text type]
		pack [entry ${propPage}.entry_type -textvariable ::PropPage::Properties(type)] -fill x
		pack [label ${propPage}.label_dim -text dimension]
		pack [entry ${propPage}.entry_dim -textvariable ::PropPage::Properties(dimension)] -fill x
		pack [label ${propPage}.label_comment -text comment]
		pack [entry ${propPage}.entry_comment -textvariable ::PropPage::Properties(comment)] -fill x
		pack [label ${propPage}.label_anno -text annotation]
		pack [entry ${propPage}.entry_anno -textvariable ::PropPage::Properties(annotation)] -fill x
		pack [label ${propPage}.label_prio -text priority]
		pack [entry ${propPage}.entry_prio -textvariable ::PropPage::Properties(priority)] -fill x
		pack [label ${propPage}.label_ptype -text paramstype]
		pack [entry ${propPage}.entry_ptype -textvariable ::PropPage::Properties(paramstype)] -fill x
		pack [label ${propPage}.label_prefix -text prefix]
		pack [entry ${propPage}.entry_prefix -textvariable ::PropPage::Properties(prefix)] -fill x
		pack [label ${propPage}.label_dclass -text dataclass]
		pack [entry ${propPage}.entry_dclass -textvariable ::PropPage::Properties(dataclass)] -fill x
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
		pack [button ${propPage}.button_addAttr -text "Add Attribute" -command ::Commands::addAttribute] -fill x
		pack [button ${propPage}.button_addMem -text "Add Member" -command ::Commands::addMember] -fill x
	}
	
	proc controls_ATTRIBUTE {} {
		variable propPage
		pack [label ${propPage}.label_val -text value]
		pack [entry ${propPage}.entry_val -textvariable ::PropPage::Properties(value)] -fill x
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
	}

	proc controls_PARAMETER {} {
		variable propPage
		pack [label ${propPage}.label_field -text field]
		pack [entry ${propPage}.entry_field -textvariable ::PropPage::Properties(field)] -fill x
		pack [label ${propPage}.label_type -text type]
		pack [entry ${propPage}.entry_type -textvariable ::PropPage::Properties(type)] -fill x
		pack [label ${propPage}.label_comment -text comment]
		pack [entry ${propPage}.entry_comment -textvariable ::PropPage::Properties(comment)] -fill x
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
	}

	proc controls_MEMBER {} {
		variable propPage
		pack [label ${propPage}.label_name -text name]
		pack [entry ${propPage}.entry_name -textvariable ::PropPage::Properties(name)] -fill x
		pack [label ${propPage}.label_type -text type]
		pack [entry ${propPage}.entry_type -textvariable ::PropPage::Properties(type)] -fill x
		pack [label ${propPage}.label_comment -text comment]
		pack [entry ${propPage}.entry_comment -textvariable ::PropPage::Properties(comment)] -fill x
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
	}

	proc controls_MACROS {} {
		variable propPage
		pack [button ${propPage}.button_addMacro -text "Add Macro" -command ::Commands::addMacro] -fill x
	}

	proc controls_MACRO {} {
		variable propPage
		pack [label ${propPage}.label_name -text name]
		pack [entry ${propPage}.entry_name -textvariable ::PropPage::Properties(name)] -fill x
		pack [label ${propPage}.label_type -text type]
		pack [ComboBox ${propPage}.combo_type -textvariable ::PropPage::Properties(type) -values [list list derive function abort index] -editable no] -fill x
		pack [label ${propPage}.label_replace -text replace]
		pack [ComboBox ${propPage}.combo_replace -textvariable ::PropPage::Properties(replace) -values [list replace subset extend remove] -editable no] -fill x
		pack [label ${propPage}.label_dtype -text datatype]
		pack [ComboBox ${propPage}.combo_dtype -textvariable ::PropPage::Properties(datatype) -values [list string number id undefined] -editable no] -fill x
		pack [label ${propPage}.label_comment -text comment]
		pack [entry ${propPage}.entry_comment -textvariable ::PropPage::Properties(comment)] -fill x
		pack [label ${propPage}.label_anno -text annotation]
		pack [entry ${propPage}.entry_anno -textvariable ::PropPage::Properties(annotation)] -fill x
		pack [label ${propPage}.label_prio -text priority]
		pack [entry ${propPage}.entry_prio -textvariable ::PropPage::Properties(priority)] -fill x
		pack [label ${propPage}.label_vector -text vector]
		pack [entry ${propPage}.entry_vector -textvariable ::PropPage::Properties(vector)] -fill x
		pack [label ${propPage}.label_research -text research]
		pack [entry ${propPage}.entry_research -textvariable ::PropPage::Properties(research)] -fill x
		pack [label ${propPage}.label_priolist -text priolist]
		pack [entry ${propPage}.entry_priolist -textvariable ::PropPage::Properties(priolist)] -fill x
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
		pack [button ${propPage}.button_addVList -text "Add Value List" -command ::Commands::addValuelist] -fill x
	}

	proc controls_VALUELIST {} {
		variable propPage
		pack [label ${propPage}.label_cond -text condition]
		pack [entry ${propPage}.entry_cond -textvariable ::PropPage::Properties(condition)] -fill x
		pack [label ${propPage}.label_comment -text comment]
		pack [entry ${propPage}.entry_comment -textvariable ::PropPage::Properties(comment)] -fill x
		pack [label ${propPage}.label_anno -text annotation]
		pack [entry ${propPage}.entry_anno -textvariable ::PropPage::Properties(annotation)] -fill x
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
		pack [button ${propPage}.button_addVal -text "Add Value" -command ::Commands::addValue] -fill x
	}

	proc controls_VALUE {} {
		variable propPage
		pack [label ${propPage}.label_val -text value]
		pack [entry ${propPage}.entry_val -textvariable ::PropPage::Properties(value)] -fill x
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
	}

	proc controls_AXIOMS {} {
		variable propPage
		pack [label ${propPage}.label_init -text initialstate]
		pack [entry ${propPage}.entry_init -textvariable ::PropPage::Properties(initialstate)] -fill x
		pack [button ${propPage}.button_addAxiom -text "Add Axiom" -command ::Commands::addAxiom] -fill x
	}

	proc controls_AXIOM {} {
		variable propPage
		pack [label ${propPage}.label_name -text name]
		pack [entry ${propPage}.entry_name -textvariable ::PropPage::Properties(name)] -fill x
		pack [label ${propPage}.label_axiom -text axiom]
		pack [entry ${propPage}.entry_axiom -textvariable ::PropPage::Properties(axiom)] -fill x
		pack [label ${propPage}.label_check -text check]
		pack [entry ${propPage}.entry_check -textvariable ::PropPage::Properties(check)] -fill x
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
	}

	proc controls_INITIAL {} {
		variable propPage
		pack [label ${propPage}.label_init -text initial]
		pack [entry ${propPage}.entry_init -textvariable ::PropPage::Properties(initial)] -fill x
	}

	proc controls_SPECFILES {} {
		variable propPage
		pack [button ${propPage}.button_addSet -text "Add File Set" -command ::Commands::addFileSet] -fill x
	}
	
	proc controls_SET {} {
		variable propPage
		pack [label ${propPage}.label_name -text name]
		pack [entry ${propPage}.entry_name -textvariable ::PropPage::Properties(name)] -fill x
		pack [label ${propPage}.label_type -text type]
		pack [entry ${propPage}.entry_type -textvariable ::PropPage::Properties(type)] -fill x
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
		pack [button ${propPage}.button_addSet -text "Add Files" -command ::Commands::addFiles] -fill x
	}

	proc controls_FILE {} {
		variable propPage
		pack [entry ${propPage}.entry_path -textvariable ::PropPage::Properties(path)] -fill x
		pack [button ${propPage}.button_browse -text Browse... -command {::Commands::browseOpen ::PropPage::Properties(path)}] -anchor w
		set ::PropPage::relative [string equal [file pathtype $::PropPage::Properties(path)] relative]
		pack [checkbutton ${propPage}.check_relative -text "Relative path" -variable ::PropPage::relative -command ::Commands::relative] -anchor w
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
	}

	proc controls_TOOLS {} {
		variable propPage
		pack [button ${propPage}.button_addTool -text "Add Tool" -command {::Commands::addTool 0}] -fill x
		pack [button ${propPage}.button_addPipe -text "Add Pipe" -command {::Commands::addTool 1}] -fill x
	}

	proc controls_TOOL {} {
		variable propPage
		variable Properties
		pack [label ${propPage}.label_name -text name]
		pack [entry ${propPage}.entry_name -textvariable ::PropPage::Properties(name)] -fill x
		if {$Properties(ispipe)!=1} {
			pack [label ${propPage}.label_path -text path]
			pack [entry ${propPage}.entry_path -textvariable ::PropPage::Properties(path)] -fill x
			pack [button ${propPage}.button_browsetoolpath -text "Browse..." -command {::Commands::browseOpen ::PropPage::Properties(path) -filetypes {{{TCL scripts} {.tcl}} {{Windows executable files} {.exe .com .bat .cmd}} {{All Files} *}}}] -anchor w
			pack [label ${propPage}.label_cscript -text "Configuration script"]
			pack [entry ${propPage}.entry_cscript -textvariable ::PropPage::Properties(configscript)] -fill x
			pack [button ${propPage}.button_browsecscript -text "Browse..." -command {::Commands::browseOpen ::PropPage::Properties(configscript) -filetypes {{{TCL scripts} {.tcl}} {{All Files} *}}}] -anchor w
		}
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
		if {$Properties(ispipe)==1} {
			pack [button ${propPage}.button_addInput -text "Add Input" -command ::Commands::addInput] -fill x
			pack [button ${propPage}.button_addOutput -text "Add Output" -command ::Commands::addOutput] -fill x
			pack [button ${propPage}.button_addExec -text "Add Exec" -command ::Commands::addExec] -fill x
		} else {
			pack [button ${propPage}.button_addToolConfig -text "Add ToolConfig" -command ::Commands::addToolConfig] -fill x
		}
	}

	proc controls_TOOLCONFIG {} {
		global env
		variable propPage
		set parent [::dom::node parent $::ProjTree::selectedNode]
		set toolname [::dom::element getAttribute $parent name]
		set ispipe [::dom::element getAttribute $parent ispipe]
		pack [label ${propPage}.label_name -text name]
		pack [entry ${propPage}.entry_name -textvariable ::PropPage::Properties(name)] -fill x
		pack [label ${propPage}.label_opts -text options]
		pack [entry ${propPage}.entry_opts -textvariable ::PropPage::Properties(options)] -fill x
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
		if {$ispipe!=1} {
			if {[catch {source [file join [::XML::getWorkDir] [subst [::dom::element getAttribute $parent configscript]]]} err]} {
				tk_messageBox -title "Error" -message $err
				::Tools::resetConfigScript
			} else {
				set sw [ScrolledWindow ${propPage}.sw -scrollbar vertical -auto vertical ]
				set sf [ScrollableFrame ${sw}.sf -constrainedwidth yes]
				$sw setwidget $sf
				pack $sw -fill both -expand yes
				Script_configTool [$sf getframe] $::ProjTree::selectedNode
			}
		} else {
			pack [label ${propPage}.info -text "Note: $toolname is a pipe."] -fill x
		}
	}

	proc controls_INPUT {} {
		variable propPage
		set parent [::dom::node parent $::ProjTree::selectedNode]
		set toolname [::dom::element getAttribute $parent name]
		set ispipe [::dom::element getAttribute $parent ispipe]
		pack [label ${propPage}.label_name -text name]
		pack [entry ${propPage}.entry_name -textvariable ::PropPage::Properties(name)] -fill x
		pack [label ${propPage}.label_type -text type]
		pack [ComboBox ${propPage}.combo_type -textvariable ::PropPage::Properties(type) -values [list FILE FILE_LIST] -editable no] -fill x
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
		if {$ispipe!=1} {
			pack [label ${propPage}.info -text "Note: $toolname is not a pipe."] -fill x
		}
	}

	proc controls_OUTPUT {} {
		# A good idea, isn't it? :-)
		controls_INPUT
	}

	proc controls_EXEC {} {
		variable propPage
		set parent [::dom::node parent $::ProjTree::selectedNode]
		set tools [::dom::node parent $parent]
		set toolname [::dom::element getAttribute $parent name]
		set ispipe [::dom::element getAttribute $parent ispipe]
		set tool_name_list {}
		foreach tool [::XML::findElements $tools TOOL {}] {
			if {[string compare $tool $parent]} {
				lappend tool_name_list [::dom::element getAttribute $tool name]
			}
		}			
		pack [label ${propPage}.label_name -text "name"]
		pack [ComboBox ${propPage}.combo_name -textvariable ::PropPage::Properties(name) -values $tool_name_list -editable no -modifycmd ::PropPage::update_EXEC] -fill x
		pack [label ${propPage}.label_toolconfig -text "toolconfig"]
		pack [ComboBox ${propPage}.combo_toolconfig -textvariable ::PropPage::Properties(toolconfig) -editable no] -fill x
		update_EXEC
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
		if {$ispipe!=1} {
			pack [label ${propPage}.info -text "Note: $toolname is not a pipe."] -fill x
		}
	}

	# Invoked when user selects a tool in EXEC properties
	proc update_EXEC {} {
		variable propPage
		variable Properties
		set selected [${propPage}.combo_name getvalue]
		# If no tool is selected, the toolconfig combo box is empty
		if {$selected<0} {
			${propPage}.combo_toolconfig configure -values {} -text ""
			return
		}
		# Finding the selected tool
		set this_tool [::dom::node parent $::ProjTree::selectedNode]
		set tools [::dom::node parent $this_tool]
		set i -1
		set exec_tool ""
		foreach tool [::XML::findElements $tools TOOL {}] {
			if {[string compare $tool $this_tool]} {incr i}
			if {$i==$selected} {
				set exec_tool $tool
				break
			}
		}
		# This shall never occur
		if {$exec_tool==""} {
			return -code error "Tool not found, unexpected error"
		}
		# Get list of tool configurations
		set list [concat [list ""] [::XML::listAttrByType $exec_tool TOOLCONFIG name]]
		${propPage}.combo_toolconfig configure -values $list
		# If the value in combobox doesn't match any list value, first value (empty) is selected
		if {[${propPage}.combo_toolconfig getvalue]<0} {
			${propPage}.combo_toolconfig setvalue @0
		}
		# Now it's time to update ATTACHINs and ATTACHOUTs under this EXEC
		set tool_inputs {}
		set tool_outputs {}
		# Make two separate lists - inputs and outputs
		foreach tool_io [::Tools::getToolIOs $exec_tool] {
			switch [lindex $tool_io 2] {
				in {lappend tool_inputs [lindex $tool_io 0]}
				out {lappend tool_outputs [lindex $tool_io 0]}
			}
		}
		set exec_inputs {}
		set exec_outputs {}
		# Iterate through ATTACHIN subnodes, delete those ones that don't match any tool input
		foreach attachin [::XML::findElements $::ProjTree::selectedNode ATTACHIN {}] {
			set port [::dom::element getAttribute $attachin to]
			if {[lsearch $tool_inputs $port]<0 || [lsearch $exec_inputs $port]>=0} {	
				::XML::DeleteElement $attachin
				::ProjTree::DeleteElement $attachin
			} else {
				lappend exec_inputs $port
			}
		}
		# Same thing for ATTACHOUT subnodes
		foreach attachout [::XML::findElements $::ProjTree::selectedNode ATTACHOUT {}] {
			set port [::dom::element getAttribute $attachout from]
			if {[lsearch $tool_outputs $port]<0 || [lsearch $exec_outputs $port]>=0} {	
				::XML::DeleteElement $attachout
				::ProjTree::DeleteElement $attachout
			} else {
				lappend exec_outputs $port
			}
		}
		# Iterate through tool inputs and add ATTACHIN subnodes if they are not present
		foreach port $tool_inputs {
			if {[lsearch $exec_inputs $port]<0} {
				::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode ATTACHIN [list to $port]]
				lappend exec_inputs $port
			}
		}
		# Same thing for tool outputs
		foreach port $tool_outputs {
			if {[lsearch $exec_outputs $port]<0} {
				::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode ATTACHOUT [list from $port]]
				lappend exec_outputs $port
			}
		}
	}

	proc controls_ATTACHIN {} {
		variable propPage
		variable Properties
		variable chan_list
		pack [label ${propPage}.label_to -text $::PropPage::Properties(to) -relief sunken] -fill x
		set this_exec [::dom::node parent $::ProjTree::selectedNode]
		set this_pipe [::dom::node parent $this_exec]
		set from_list [list ""]
		foreach io [::Tools::getToolIOs $this_pipe] {
			if {[lindex $io 2]=="in"} {
				lappend from_list [lindex $io 0]
			}
		}
		set chan_list $from_list
		foreach exec [::XML::findElements $this_pipe EXEC {}] {
			if {$exec==$this_exec} {
				break
			} else {
				set toolname [::dom::element getAttribute $exec name]
				foreach attachout [::XML::findElements $exec ATTACHOUT {}] {
					set outname [::dom::element getAttribute $attachout from]
					lappend from_list "($toolname)$outname"
					lappend chan_list [::dom::element getAttribute $attachout to]
				}
			}
		}	
		pack [label ${propPage}.label_from -text "from"]
		pack [ComboBox ${propPage}.combo_from -values $from_list -editable no -modifycmd ::PropPage::update_ATTACHIN] -fill x
		set i [lsearch $chan_list $Properties(from)]
		if {$i>=0} {
			${propPage}.combo_from setvalue @$i
		} else {
			set Properties(from) ""
		}
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
	}

	# Invoked when user selects input source in the "from" combo box
	proc update_ATTACHIN {} {
		variable propPage
		variable Properties
		variable chan_list
		# Just mapping combo box value list to input/internal channel list
		set i [${propPage}.combo_from getvalue]
		if {$i>=0} {
			set Properties(from) [lindex $chan_list $i]
		}
	}

	proc controls_ATTACHOUT {} {
		variable propPage
		variable Properties
		pack [label ${propPage}.label_from -text $::PropPage::Properties(from) -relief sunken] -fill x
		set execnode [::dom::node parent $::ProjTree::selectedNode]
		set pipenode [::dom::node parent $execnode]
		set to_list [list "Other tool input"]
		foreach io [::Tools::getToolIOs $pipenode] {
			if {[lindex $io 2]=="out"} {lappend to_list [lindex $io 0]}
		}
		pack [label ${propPage}.label_to -text "to"]
		pack [ComboBox ${propPage}.combo_to -values $to_list -editable no -modifycmd ::PropPage::update_ATTACHOUT] -fill x
		set i [lsearch $to_list $Properties(to)]
		if {$i>0} {
			${propPage}.combo_to setvalue @$i
		} elseif {$Properties(to)!=""} {
			${propPage}.combo_to setvalue @0
		}
		pack [label ${propPage}.label_value -text "value"]
		pack [entry ${propPage}.entry_value -textvariable ::PropPage::Properties(value)] -fill x
		pack [button ${propPage}.button_browsevalue -text "Browse..." -command "::Commands::browseSave ::PropPage::Properties(value)"] -anchor w
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
	}

	# invoked when user selects output destination in the "to" combo box
	proc update_ATTACHOUT {} {
		variable propPage
		variable Properties
		# Output connected to other tool input (internal channel)
		if {[${propPage}.combo_to getvalue]==0} {
			set execnode [::dom::node parent $::ProjTree::selectedNode]
			set pipenode [::dom::node parent $execnode]
			set chanlist {}
			# Building list of all channels in the pipe - inputs, outputs, internal
			foreach io [::Tools::getToolIOs $pipenode] {
				lappend chanlist [lindex $io 0]
			}
			foreach attachout [::XML::findElements $pipenode ATTACHOUT {} -recursive] {
				if {${attachout}!=${::ProjTree::selectedNode}} {
					lappend chanlist [::dom::element getAttribute $attachout to]
				}
			}
			# If channel name is duplicate or empty, a new unique name must be generated
			if {[lsearch $chanlist $Properties(to)]>=0 || $Properties(to)==""} {
				set i 0
				while {[lsearch $chanlist CHAN_$i]>=0} {incr i}
				set ::PropPage::Properties(to) CHAN_$i
			}
		# Output connected to pipe output
		} else {
			set ::PropPage::Properties(to) [${propPage}.combo_to get]
		}
	}

	proc controls_TESTSUITE {} {
		variable propPage
		pack [button ${propPage}.button_addTestCase -text "Add Test Case" -command ::Commands::addTestCase] -fill x
	}

	proc controls_TESTCASE {} {
		variable propPage
		pack [label ${propPage}.label_name -text name]
		pack [entry ${propPage}.entry_name -textvariable ::PropPage::Properties(name)] -fill x
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
		pack [button ${propPage}.button_addPipe -text "Add Pipe" -command ::Commands::addPipe] -fill x
		pack [button ${propPage}.button_run -text "Run" -command ::Commands::run_testcase] -fill x
		pack [button ${propPage}.button_build -text "Build" -command ::Commands::build_testcase] -fill x
	}
	
	proc controls_PIPE {} {
		variable propPage
		set tools [lindex [::XML::findElements "" TOOLS {} -recursive] 0]
		set toollist {}
		if {$tools!=""} {
			foreach tool [::XML::findElements $tools TOOL {ispipe 1}] {
				lappend toollist [::dom::element getAttribute $tool name]
			}
		}
		pack [label ${propPage}.label_name -text name]
		pack [ComboBox ${propPage}.combo_name -textvariable ::PropPage::Properties(name) -editable no -values $toollist -modifycmd ::PropPage::update_PIPE] -fill x
		pack [checkbutton ${propPage}.check_isexec -text "Executable" -variable ::PropPage::Properties(isexec)] -fill x
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
		pack [button ${propPage}.button_del -text "Delete" -command ::Commands::delete] -fill x
		pack [button ${propPage}.button_run -text "Run" -command ::Commands::run_pipe] -fill x
		update_PIPE
	}

	# Update DATAINs and DATAOUTs when tool is selected from combo box
	proc update_PIPE {} {
		variable propPage
		variable Properties
		# Finding the selected tool
		set pipe_tool [lindex [::XML::findElements "" TOOL [list name $::PropPage::Properties(name)] -recursive] 0]
		# This shall never occur
		if {$pipe_tool==""} {
			set Properties(name) ""
			return
		}
		# Update DATAINs and DATAOUTs in this PIPE
		set tool_inputs {}
		set tool_outputs {}
		# Make two separate lists - inputs and outputs
		foreach tool_io [::Tools::getToolIOs $pipe_tool] {
			switch [lindex $tool_io 2] {
				in {lappend tool_inputs [lindex $tool_io 0]}
				out {lappend tool_outputs [lindex $tool_io 0]}
			}
		}
		set pipe_inputs {}
		set pipe_outputs {}
		# Iterate through DATAIN subnodes, delete those ones that don't match any tool input
		foreach datain [::XML::findElements $::ProjTree::selectedNode DATAIN {}] {
			set port [::dom::element getAttribute $datain name]
			if {[lsearch $tool_inputs $port]<0 || [lsearch $pipe_inputs $port]>=0} {	
				::XML::DeleteElement $datain
				::ProjTree::DeleteElement $datain
			} else {
				lappend pipe_inputs $port
			}
		}
		# Same thing for DATAOUT subnodes
		foreach dataout [::XML::findElements $::ProjTree::selectedNode DATAOUT {}] {
			set port [::dom::element getAttribute $dataout name]
			if {[lsearch $tool_outputs $port]<0 || [lsearch $pipe_outputs $port]>=0} {	
				::XML::DeleteElement $dataout
				::ProjTree::DeleteElement $dataout
			} else {
				lappend pipe_outputs $port
			}
		}
		# Iterate through tool inputs and add DATAIN subnodes if they are not present
		foreach port $tool_inputs {
			if {[lsearch $pipe_inputs $port]<0} {
				::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode DATAIN [list name $port]]
				lappend pipe_inputs $port
			}
		}
		# Same thing for tool outputs
		foreach port $tool_outputs {
			if {[lsearch $pipe_outputs $port]<0} {
				::ProjTree::AddElement $::ProjTree::selectedNode [::XML::AddElement $::ProjTree::selectedNode DATAOUT [list name $port]]
				lappend pipe_outputs $port
			}
		}
	}

	proc controls_DATAIN {} {
		variable propPage
		pack [label ${propPage}.label_name -text $::PropPage::Properties(name) -relief sunken] -fill x
		pack [entry ${propPage}.entry_value -textvariable ::PropPage::Properties(value)] -fill x
		pack [button ${propPage}.button_browse -text "Browse..." -command {::Commands::browseOpen ::PropPage::Properties(value)}] -anchor w
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
	}

	proc controls_DATAOUT {} {
		variable propPage
		pack [label ${propPage}.label_name -text $::PropPage::Properties(name) -relief sunken] -fill x
		pack [entry ${propPage}.entry_value -textvariable ::PropPage::Properties(value)] -fill x
		pack [button ${propPage}.button_browse -text "Browse..." -command {::Commands::browseSave ::PropPage::Properties(value)}] -anchor w
		pack [button ${propPage}.button_up -text "Move Up" -command ::Commands::up] -fill x
		pack [button ${propPage}.button_down -text "Move Down" -command ::Commands::down] -fill x
	}	

	# Display controls in property page
	proc packControls {} {
		set tagName [::dom::element cget $::ProjTree::selectedNode -tagName]
		if {[lsearch $::XML::Elements $tagName]<0} return
		controls_${tagName}
	}

	# Copy selected element's attributes from DOM to properties array
	proc LoadFromXML {} {
		variable Properties
		set type [::dom::element cget $::ProjTree::selectedNode -tagName]
		if {[lsearch $::XML::Elements $type]<0} return
		foreach attr $::XML::attr($type) {
			set Properties($attr) [::dom::element getAttribute $::ProjTree::selectedNode $attr]
		}
	}

	# Copy selected element's attributes from properties array to DOM
	proc SaveToXML {} {
		variable Properties
		if {$::ProjTree::selectedNode==""} return
		set type [::dom::element cget $::ProjTree::selectedNode -tagName]
		if {[lsearch $::XML::Elements $type]<0} return
		foreach attr $::XML::attr($type) {
			::dom::element setAttribute $::ProjTree::selectedNode $attr $Properties($attr)
		}
		# TOOLCONFIG is a special case
		if {[::dom::element cget $::ProjTree::selectedNode -tagName]=="TOOLCONFIG"} {
			if {[::dom::element getAttribute [::dom::node parent $::ProjTree::selectedNode] ispipe]!=1} {
				Script_getToolConfig $::ProjTree::selectedNode
			}
		}
	}

	# Create property page when a tree element is selected
	proc Create {} {
		variable Properties
		variable propPage
		pack [set propPage [frame ${::GUI::rightPane}.prop]] -fill both -expand yes -anchor n
		LoadFromXML
		packControls
	}

	# Destroy property page when a tree element is deselected
	proc Destroy {} {
		variable propPage
		if {$propPage==""} return
		SaveToXML
		destroy $propPage
		set propPage {}
	}
	
	# These tags have a unique attribute "name"
	variable UniqueNameTags [list CONFIG INSTANCE SIGNAL TYPEDEF MACRO MEMBER AXIOM SET TOOL INPUT OUTPUT TOOLCONFIG TESTCASE]

	# Check if property page data is valid (unique identifiers, non-empty fields, etc.)
	proc Validate {} {
		variable Properties
		variable UniqueNameTags
		if {$ProjTree::selectedNode==""} {return 0}
		set tagName [::dom::element cget $ProjTree::selectedNode -tagName]
		set parent [::dom::node parent $ProjTree::selectedNode]
		if {[lsearch $UniqueNameTags $tagName]>=0} {
			if {$Properties(name)==""} {
				tk_messageBox -title Error -message "Name not specified"
				return 1
			}
			set list [::XML::findElements $parent $tagName [list name $Properties(name)] -recursive]
			if {[llength $list]>1 || [llength $list]==1 && [lindex $list 0]!=$ProjTree::selectedNode} {
				tk_messageBox -title Error -message "Duplicate name \"$Properties(name)\""
				return 1
			}
		}
		return 0
	}
}

# File related procedures used in SPECFILES
namespace eval Files {
	
	# Generate list file from file set
	proc genFileSetList {fileset} {
		#tk_messageBox -title genFileSetList -message $fileset
		if {[string compare [::dom::element cget $fileset -tagName] SET]} return
		set name [::dom::element getAttribute $fileset name]
		if {[string length $name]<1} return
		set output [open [file join [::XML::getWorkDir] ${name}.lst] w]
		foreach file [::XML::findElements $fileset FILE {}] {
			puts $output [file normalize [file join [::XML::getWorkDir] [::dom::element getAttribute $file path]]]
		}
		close $output
	}

	# Generate list files from all file sets in the project
	proc genAllFileSetLists {} {
		if {$::XML::root==""} return
		foreach fileset [::XML::findElements "" SET {} -recursive] {
			genFileSetList $fileset
		}
	}
	
	# Converts absolute path to relative one.
	proc RelPath {path current} {
		set cdir [file normalize $current]
		switch [file pathtype $path] {
			relative {
				return $path
			}
			volumerelative {
				set cdirlist [lrange [file split $cdir] 1 end]
				set pathlist [lrange [file split $path] 1 end]
				for {set i 0} {$i < [llength $cdirlist]} {incr i} {
					if {[string compare [lindex $cdirlist $i] [lindex $pathlist $i]]} break
				}
				set updirs {}
				for {set j $i} {$j < [llength $cdirlist]} {incr j} {
					lappend updirs ".."
				}
				return [join [concat $updirs [lrange $pathlist $i end]] /]
			}
			absolute {
				set cdirlist [file split $cdir]
				set pathlist [file split $path]
				if {[string compare -nocase [lindex $cdirlist 0] [lindex $pathlist 0]]} {
					return $path
				}
				for {set i 1} {$i < [llength $cdirlist]} {incr i} {
					if {[string compare [lindex $cdirlist $i] [lindex $pathlist $i]]} break
				}
				set updirs {}
				for {set j $i} {$j < [llength $cdirlist]} {incr j} {
					lappend updirs ".."
				}
				return [join [concat $updirs [lrange $pathlist $i end]] /]
			}
		}
	}
}

# Tools and pipes related procedures
namespace eval Tools {

	# In case of errors with config scripts these procedures should be reset to empty procs
	proc resetConfigScript {} {
		proc ::PropPage::Script_configTool {frame toolconfig} {}
		proc ::PropPage::Script_getToolConfig {toolconfig} {}
		proc ::PropPage::Script_getToolSignals {} {}
		proc ::PropPage::Script_runTool {path io toolconfig} {}
		proc ::Tools::Script_configTool {frame toolconfig} {}
		proc ::Tools::Script_getToolConfig {toolconfig} {}
		proc ::Tools::Script_getToolSignals {} {}
		proc ::Tools::Script_runTool {path io toolconfig} {}
	}

	# Returns list of tool's or pipe's inputs and outputs.
	proc getToolIOs {toolnode} {
		global env
		set iolist {}
		if {[string compare [::dom::element cget $toolnode -tagName] "TOOL"]} {
			return {}
		}
		if {[::dom::element getAttribute $toolnode ispipe]==1} {
			foreach ionode [::dom::node children $toolnode] {
				if {[string compare [::dom::node cget $ionode -nodeType] element]} continue
				switch [::dom::element cget $ionode -tagName] {
					INPUT {lappend iolist [list [::dom::element getAttribute $ionode name] [::dom::element getAttribute $ionode type] in]}
					OUTPUT {lappend iolist [list [::dom::element getAttribute $ionode name] [::dom::element getAttribute $ionode type] out]}
				}
			}
		} else {
			if {[catch {source [file join [::XML::getWorkDir] [subst [::dom::element getAttribute $toolnode configscript]]]} err]} {
				::Tools::resetConfigScript
				set iolist ""
			} else {
				set iolist [Script_getToolSignals]
			}
		}
		return $iolist
	}

	# Execute tool (or pipe)
	proc runTool {toolname toolconfig iolist} {
		global env
		set toolnode [lindex [::XML::findElements "" TOOL [list name $toolname] -recursive] 0]
		if {$toolnode==""} {
			Console::Dump "Tool \"$toolname\" not found"
			return 1
		}
		# Pipe
		if {[::dom::element getAttribute $toolnode ispipe]==1} {
			set channels {}
			set files {}
			# Initial channel set - pipe inputs and outputs
			foreach io $iolist {
				lappend channels [lindex $io 0]
				lappend files [lindex $io 1]
			}
			# Iterate through EXECs
			foreach exec [::XML::findElements $toolnode EXEC {}] {
				set ios {}
				# Iterate through EXEC's inputs
				foreach attachin [::XML::findElements $exec ATTACHIN {}] {
					set chname [::dom::element getAttribute $attachin from]
					set ioname [::dom::element getAttribute $attachin to]
					set i [lsearch $channels $chname]
					# If this channel is defined, add to tool i/o list
					if {$i>=0} {
						lappend ios [list $ioname [lindex $files $i]]
					}
				}
				# Iterate through EXEC's outputs
				foreach attachout [::XML::findElements $exec ATTACHOUT {}] {
					set chname [::dom::element getAttribute $attachout to]
					set ioname [::dom::element getAttribute $attachout from]
					set i [lsearch $channels $chname]
					# If this channel is already defined, this means tool output attached to pipe output
					if {$i>=0} {
						lappend ios [list $ioname [lindex $files $i]]
					# Otherwise this means tool output attached to other tool's input
					} else {
						set filename [::dom::element getAttribute $attachout value]
						lappend ios [list $ioname $filename]
						# Now this internal channel is also defined	
						lappend channels $chname
						lappend files $filename
					}
				}
				set execname [::dom::element getAttribute $exec name]
				set exectoolcfg [::dom::element getAttribute $exec toolconfig]
				if {[runTool $execname $exectoolcfg $ios]} {
					Console::Dump "Error running tool \"$execname\", aborting pipe \"$toolname\""
					return 1
				}
			}
		# Tool
		} else {
			set path [::dom::element getAttribute $toolnode path]
			set cfgscript [::dom::element getAttribute $toolnode configscript]
			set toolcfgnode [lindex [::XML::findElements $toolnode TOOLCONFIG [list name $toolconfig]] 0]
			#if {$toolcfgnode==""} {
			#	Console::Dump "Tool configuration \"$toolconfig\" for tool \"$toolname\" not found"
			#	return 1
			#}
			set scriptpath [file join [::XML::getWorkDir] [subst $cfgscript]]
			if {[catch {source $scriptpath} err]} {
				Console::Dump "Failed to load configuration script \"$scriptpath\" for tool \"$toolname\":\n$err"
				::Tools::resetConfigScript
				return 1
			}
			if {[catch {Script_runTool $path $iolist $toolcfgnode} res]} {
				Console::Dump "Error running tool \"$toolname\":\n$res"
			} else {
				Console::Dump $res
			}
		}
		return 0
	}

	# Execute pipe
	proc runPipe {pipenode} {
		set ios {}
		foreach datain [::XML::findElements $pipenode DATAIN {}] {
			lappend ios [list [::dom::element getAttribute $datain name] [::dom::element getAttribute $datain value]]
		}
		foreach dataout [::XML::findElements $pipenode DATAOUT {}] {
			lappend ios [list [::dom::element getAttribute $dataout name] [::dom::element getAttribute $dataout value]]
		}
		set pipename [::dom::element getAttribute $pipenode name]
		if {[runTool $pipename {} $ios]} {
			Console::Dump "Pipe \"$pipename\" executed with errors"
		} else {
			Console::Dump "Pipe \"$pipename\" executed successfully"
		}
	}

	# Run testcase (execute only executable pipes)
	proc runTestCase {testcasenode} {
		Console::Dump "Running test case [::dom::element getAttribute $testcasenode name]"
		foreach pipenode [::XML::findElements $testcasenode PIPE [list isexec 1]] {
			runPipe $pipenode
		}
	}

	# Build testcase (execute all pipes)
	proc buildTestCase {testcasenode} {
		Console::Dump "Building test case [::dom::element getAttribute $testcasenode name]"
		foreach pipenode [::XML::findElements $testcasenode PIPE {}] {
			runPipe $pipenode
		}
	}

	# Build all testcases
	proc buildProject {} {
		::Files::genAllFileSetLists
		foreach testcase [::XML::findElements "" TESTCASE {} -recursive] {
			buildTestCase $testcase
		}
	}

	# Run all testcases
	proc runProject {} {
		foreach testcase [::XML::findElements "" TESTCASE {} -recursive] {
			runTestCase $testcase
		}
	}
}

# Initializing main window
GUI::Init

# Loading file specified in command line
if {$argc>0} {
	set filename [lindex $argv 0]
	if {![::XML::LoadFile $filename]} {
		::ProjTree::Fill
		::GUI::SetTitle "$::XML::filename - TAT"
	}
}