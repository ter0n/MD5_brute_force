set usb [lindex [get_hardware_names] 0]
set device_name [lindex [get_device_names -hardware_name $usb] 0]
#puts "*************************"
#puts "programming cable:"
#puts $usb

#IR scan codes:  001 -> push
#                010 -> pop

#proc push {value} {
proc push {} {
	global device_name usb
	open_device -device_name $device_name -hardware_name $usb
	#set value [read_from_file]

	#читаем из файла A, B, C, D
	set fileid [open "buffer_to_FPGA" r+]
	seek $fileid 0 start
	set A [gets $fileid];
	set B [gets $fileid];
	set C [gets $fileid];
	set D [gets $fileid];
	close $fileid
	# ------------------------------------------------------------------- #
	puts $A
	puts $B
	puts $C
	puts $D
	
	set A_bit [int2bits $A]
	set diff [expr {32 - [string length $A_bit]%32}]
	if {$diff != 32} {
		set A_bit [format %0${diff}d$A_bit 0] 
	}
	set B_bit [int2bits $B]
	set diff [expr {32 - [string length $B_bit]%32}]
	if {$diff != 32} {
		set B_bit [format %0${diff}d$B_bit 0] 
	}
	set C_bit [int2bits $C]
	set diff [expr {32 - [string length $C_bit]%32}]
	if {$diff != 32} {
		set C_bit [format %0${diff}d$C_bit 0] 
	}
	set D_bit [int2bits $D]
	set diff [expr {32 - [string length $D_bit]%32}]
	if {$diff != 32} {
		set D_bit [format %0${diff}d$D_bit 0] 
	}
	
	#puts $A_bit
	#puts $B_bit
	#puts $C_bit
	#puts $D_bit
	
	device_lock -timeout 10000
	
	device_virtual_ir_shift -instance_index 0 -ir_value 6 -no_captured_ir_value
	device_virtual_dr_shift -instance_index 0 -dr_value $A_bit -length 32 -no_captured_dr_value
	
	device_virtual_ir_shift -instance_index 0 -ir_value 7 -no_captured_ir_value
	device_virtual_dr_shift -instance_index 0 -dr_value $B_bit -length 32 -no_captured_dr_value
	
	device_virtual_ir_shift -instance_index 0 -ir_value 8 -no_captured_ir_value
	device_virtual_dr_shift -instance_index 0 -dr_value $C_bit -length 32 -no_captured_dr_value
	
	device_virtual_ir_shift -instance_index 0 -ir_value 9 -no_captured_ir_value
	device_virtual_dr_shift -instance_index 0 -dr_value $D_bit -length 32 -no_captured_dr_value
	
	device_virtual_ir_shift -instance_index 0 -ir_value 5 -no_captured_ir_value
	device_virtual_dr_shift -instance_index 0 -dr_value 10101 -length 5 -no_captured_dr_value
	
	device_unlock
	#if {$value > 256} {
		#return "value entered exceeds 8 bits" }

	#set push_value [int2bits $value]
	#set diff [expr {8 - [string length $push_value]%8}]

	#if {$diff != 8} {
	#set push_value [format %0${diff}d$push_value 0] }

	#puts $push_value

	#device_lock -timeout 10000
	#device_virtual_ir_shift -instance_index 0 -ir_value 1 -no_captured_ir_value
	#device_virtual_dr_shift -instance_index 0 -dr_value $push_value -length 8 -no_captured_dr_value
	#device_unlock
	close_device
	puts -----------------------
}

proc pop {} {
global device_name usb
variable x
variable x1
variable x2
variable x3

variable FPGA_symb_count
set FPGA_symb_count 0

open_device -device_name $device_name -hardware_name $usb


while { $FPGA_symb_count == 0 } {
	device_lock -timeout 1000
	device_virtual_ir_shift -instance_index 0 -ir_value 10 -no_captured_ir_value
	set FPGA_symb_count [device_virtual_dr_shift -instance_index 0 -length 4]
	device_unlock
}

set str_index 0
set str_length [string length $FPGA_symb_count]
set symb_count 0
while { $str_index < $str_length } {
    set symb [string index $FPGA_symb_count $str_index]
	set symb_count [expr { ($symb_count << 1) + $symb}]
	incr str_index
}

puts $symb_count
puts -----------------------
puts $FPGA_symb_count
puts -----------------------

device_lock -timeout 1000
set symb_num 0

set fileid [open "buffer_from_FPGA" w+]
seek $fileid 0 start	
while { $symb_num < $symb_count } {
	device_virtual_ir_shift -instance_index 0 -ir_value 11 -no_captured_ir_value
	set new_symb [device_virtual_dr_shift -instance_index 0 -length 8]
	puts $new_symb
	puts $fileid $new_symb
	incr symb_num
}
close $fileid
device_unlock


#write_to_file $x 
#write_to_file [device_virtual_dr_shift -instance_index 0 -length 128] 

}

proc int2bits {i} {    
	set res ""
	while {$i>0} {
		set res [expr {$i%2}]$res
		set i [expr {$i/2}]}
		if {$res==""} {set res 0}
	return $res
}

proc bin2hex bin {
	## No sanity checking is done
	array set t {
		0000 0 0001 1 0010 2 0011 3 0100 4
		0101 5 0110 6 0111 7 1000 8 1001 9
		1010 a 1011 b 1100 c 1101 d 1110 e 1111 f
	}
	set diff [expr {4-[string length $bin]%4}]
	if {$diff != 4} {
	set bin [format %0${diff}d$bin 0] }
	regsub -all .... $bin {$t(&)} hex
	return [subst $hex]
}

proc bin2dec bin {
	## No sanity checking is done
	array set t {
		0000 0 0001 1 0010 2 0011 3 0100 4
		0101 5 0110 6 0111 7 1000 8 1001 9
		1010 a 1011 b 1100 c 1101 d 1110 e 1111 f
	}
	set diff [expr {4-[string length $bin]%4}]
	if {$diff != 4} {
		set bin [format %0${diff}d$bin 0] 
	}
	regsub -all .... $bin {$t(&)} result
	return [subst $result]
}

proc write_to_file {buf} {
	set fileid [open "buffer_from_FPGA" w+]
	seek $fileid 0 start
	puts $fileid $buf
	close $fileid
}

proc read_from_file {} {
	set fileid [open "buffer_to_FPGA" r+]
	seek $fileid 0 start
	set chars [read $fileid];
	close $fileid
	return $chars
}

push 
pop



