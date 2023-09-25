#!/bin/sh
# statusbar.tcl \
exec tclsh "$0" "$@"

set openocd "/usr/bin/openocd"
set interface "/usr/share/openocd/scripts/interface/stlink.cfg"
set target_gd "/usr/share/openocd/scripts/target/gd32e1x.cfg"
set target_stm "/usr/share/openocd/scripts/target/stm32f4x.cfg"
set port "3333"
set stm_key "s"
set gd_key "g"

if { ${argc} == 1 }  {
	set arg [ lindex ${argv} 0]
	if { ${arg} == ${stm_key} } {
		set target ${target_stm}
	} elseif { ${arg} == ${gd_key} }  {
		set target ${target_gd}
	} else {
		puts "bad arg"
		exit 2
	}
} else {
	set target ${target_gd}
}

exec ${openocd} -f ${interface} -f ${target}
