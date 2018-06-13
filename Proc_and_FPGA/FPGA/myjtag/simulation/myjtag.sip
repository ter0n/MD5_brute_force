set_global_assignment -entity "myjtag" -library "lib_myjtag" -name IP_TOOL_NAME "Qsys"
set_global_assignment -entity "myjtag" -library "lib_myjtag" -name IP_TOOL_VERSION "17.1"
set_global_assignment -entity "myjtag" -library "lib_myjtag" -name IP_TOOL_ENV "Qsys"
set_global_assignment -library "lib_myjtag" -name SPD_FILE [file join $::quartus(sip_path) "../myjtag.spd"]
set_global_assignment -library "lib_myjtag" -name MISC_FILE [file join $::quartus(sip_path) "../../myjtag.qsys"]

set_global_assignment -library "lib_myjtag" -name MISC_FILE [file join $::quartus(sip_path) "myjtag.v"]
set_global_assignment -library "lib_myjtag" -name MISC_FILE [file join $::quartus(sip_path) "submodules/sld_virtual_jtag.v"]
