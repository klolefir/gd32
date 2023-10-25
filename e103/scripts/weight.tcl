#!/bin/sh
#asm_weigth.tcl \
exec tclsh "$0" "$@"

if { 0 } {  
set instr [
	push 1
	ldr
]
}

proc check_instr { line } {
	set instr_weight 0
	if { [ string match "*word*" $line ] }  {
	return $instr_weight
}
}

set badarg_msg "Please, specify the arguments: filename funcname"
set badfunc_msg "No such function!"

if { ${argc} != 2 } {
	puts stderr ${badarg_msg}
	exit 1
}

set filename [ lindex ${argv} 0 ]
set funcname [ lindex ${argv} 1 ]

if { [ catch { set fd [ open "${filename}" "r" ] } open_err ] } {
	puts stderr "${open_err}"
	exit 2
}

while { 1 } {
	gets $fd line
	if { [ eof $fd ] } {
		puts stderr "${badfunc_msg}"
		exit 1
	}
	
	if { [ string match "*$funcname*" $line ] }  {
		break
	}
}

set instr_num 0
while { 1 } {
	gets $fd line
	if { [ eof $fd ] || $line == "" } {
		puts $instr_num
		break
	}
	incr instr_num
}
