#!/bin/sh
# statusbar.tcl \
exec tclsh "$0" "$@"

set openocd "/usr/bin/openocd"
set interface_stlink "/usr/share/openocd/scripts/interface/stlink.cfg"
set interface_cmsisdap "/usr/share/openocd/scripts/interface/cmsis-dap.cfg"
set target_gd "/usr/share/openocd/scripts/target/gd32e1x.cfg"
set target_stmf4 "/usr/share/openocd/scripts/target/stm32f4x.cfg"
set target_stmf1 "/usr/share/openocd/scripts/target/stm32f1x.cfg"
set port "3333"
set stlink_key "st"
set cmsisdap_key "cd"
set stmf4_key "s4"
set stmf1_key "s1"
set gd_key "g"

if { ${argc} == 2 }  {
	set tar [ lindex ${argv} 0]
	set int [ lindex ${argv} 1]
	if { ${tar} == ${stmf4_key} } {
		set target ${target_stmf4}
	} elseif { ${tar} == ${stmf1_key} } {
		set target ${target_stmf1}
	} elseif { ${tar} == ${gd_key} }  {
		set target ${target_gd}
	} else {
		puts stderr "bad target"
		exit 2
	}

	if { ${int} == ${stlink_key} } {
		set interface ${interface_stlink}
	} elseif { ${int} == ${cmsisdap_key} }  {
		set interface ${interface_cmsisdap}
	} else {
		puts stderr "bad interface"
		exit 2
	}
} else {
	set target ${target_gd}
	set interface ${interface_stlink}
}

exec ${openocd} -f ${interface} -f ${target}
