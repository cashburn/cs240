#!/usr/bin/expect

spawn telnet localhost 2030
expect "'^]'."
send "CREATE-ROOM cashburn test Tess\r\n"
expect "OK\r\n"

spawn telnet localhost 2030
expect "'^]'."
send "ENTER-ROOM cashburn test Tess\r\n"
expect "OK\r\n"

spawn telnet localhost 2030
expect "'^]'."
send "ENTER-ROOM twisher password Tess\r\n"
expect "OK\r\n"

spawn telnet localhost 2030
expect "'^]'."
send "SEND-MESSAGE cashburn test Tess I love you so much, my darling!\r\n"
expect "OK\r\n"