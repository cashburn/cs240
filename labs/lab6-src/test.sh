#!/usr/bin/expect

spawn telnet moore16.cs.purdue.edu 2029
expect "'^]'."
send "CREATE-ROOM cashburn test The Room\r\n"
expect "OK\r\n"

spawn telnet moore16.cs.purdue.edu 2029
expect "'^]'."
send "ENTER-ROOM cashburn test The Room\r\n"
expect "OK\r\n"

spawn telnet moore16.cs.purdue.edu 2029
expect "'^]'."
send "ENTER-ROOM another one The Room\r\n"
expect "OK\r\n"

spawn telnet moore16.cs.purdue.edu 2029
expect "'^]'."
send "ENTER-ROOM peter spider The Room\r\n"
expect "OK\r\n"
for {set i 0} {$i < 105} {incr i 1} {
	spawn telnet moore16.cs.purdue.edu 2029
	expect "'^]'."
	send "SEND-MESSAGE cashburn test The Room Number $i.\r\n"
	expect "OK\r\n"
}