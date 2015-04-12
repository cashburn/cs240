#!/usr/bin/expect

spawn telnet moore16.cs.purdue.edu 2029
expect "'^]'."
send "CREATE-ROOM cashburn test Tess\r\n"
expect "OK\r\n"

spawn telnet moore16.cs.purdue.edu 2029
expect "'^]'."
send "ENTER-ROOM cashburn test Tess\r\n"
expect "OK\r\n"

spawn telnet moore16.cs.purdue.edu 2029
expect "'^]'."
send "ENTER-ROOM twisher password Tess\r\n"
expect "OK\r\n"

spawn telnet moore16.cs.purdue.edu 2029
expect "'^]'."
send "SEND-MESSAGE cashburn test Tess I love you so much, my darling!\r\n"
expect "OK\r\n"