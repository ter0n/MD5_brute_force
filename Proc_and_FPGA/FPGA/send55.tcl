set usb [lindex [get_hardware_names] 0]
set device_name [lindex [get_device_names -hardware_name $usb] 0]
#puts "*************************"
#puts "programming cable:"
#puts $usb

#IR scan codes:  001 -> push
#                010 -> pop

proc push {value} {
global device_name usb
open_device -device_name $device_name -hardware_name $usb

if {$value > 256} {
return "value entered exceeds 8 bits" }

set push_value [int2bits $value]
set diff [expr {8 - [string length $push_value]%8}]

if {$diff != 8} {
set push_value [format %0${diff}d$push_value 0] }

puts $push_value

device_lock -timeout 10000
device_virtual_ir_shift -instance_index 0 -ir_value 1 -no_captured_ir_value
device_virtual_dr_shift -instance_index 0 -dr_value $push_value -length 8 -no_captured_dr_value
device_unlock
close_device
}

proc pop {} {
global device_name usb
variable x
open_device -device_name $device_name -hardware_name $usb
device_lock -timeout 10000
device_virtual_ir_shift -instance_index 0 -ir_value 2 -no_captured_ir_value
set x [device_virtual_dr_shift -instance_index 0 -length 8]
device_unlock
close_device
write_to_file $x 
puts $x
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

proc write_to_file {buf} {
	set fileid [open "FPGA_buffer" w+]

	seek $fileid 0 start

	puts $fileid $buf

	#seek $fileid 0 start

	#set chars [gets $fileid line1];
	#set line2 [gets $fileid];

	#puts "$chars символов в строке \"$line1\""
	#puts "Вторая строка в файле: \"$line2\""

	#seek $fileid 0 start

	#set buffer [read $fileid];
	#puts "\nВ файле содержится текст:\n$buffer"
	close $fileid
}

#puts "read board KEYs:"
pop
#puts "write board LEDs:"
push 0x53


