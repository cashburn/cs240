#!/usr/bin/expect

spawn telnet localhost 2030
expect "'^]'."
send "CREATE-ROOM cashburn test The Room\r\n"
expect "OK\r\n"

spawn telnet localhost 2030
expect "'^]'."
send "ENTER-ROOM cashburn test The Room\r\n"
expect "OK\r\n"

spawn telnet localhost 2030
expect "'^]'."
send "SEND-MESSAGE cashburn test The Room Watson, come here, I want you.\r\n"
expect "OK\r\n"