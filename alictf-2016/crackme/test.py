import sys
import struct
from hashlib import md5

def encrypt(v, k):
	y=v[0];z=v[1]
	_sum=0
	delta=0x9e3779b9
	a=k[0];b=k[1];c=k[2];d=k[3]
	for i in xrange(128):
		_sum += delta
		_sum &= 0xffffffff
		y += ((z<<4) + a) ^ (z + _sum) ^ ((z>>5) + b)
		y = y & 0xffffffff
		z += ((y<<4) + c) ^ (y + _sum) ^ ((y>>5) + d)
		z = z & 0xffffffff
	v[0] = y
	v[1] = z
	return v

def encode(flag):
	k = [0x00112233,0x44556677,0x8899aabb,0xccddeeff]
	flag = flag.decode('hex')
	v  = struct.unpack(">IIII",flag)
	v0 = [v[0],v[1]]
	v1 = [v[2],v[3]]
	v0 = encrypt(v0,k)
	v1 = encrypt(v1,k)
	v  = struct.pack("IIII",v0[0],v0[1],v1[0],v1[1])
	s  = ''
	for i in xrange(16):
		s += chr(ord(v[i])^0x31)
	print "[+] code:",s.encode('hex')
	return s

def generateFile(checkcode):
	fi = open("Crackme.exe","rb+")
	fo = open("test.exe","wb+")
	data = fi.read(0x7030)
	fo.write(data)
	data = fi.read(16)
	fo.write(checkcode)
	data = fi.read()
	fo.write(data)
	fi.close()
	fo.close()

def main():	
	salt     = "@xdsec"
	username = "bigtang"
	flag     = md5(username+salt).hexdigest()
	print "[+] flag:",flag
	assert len(flag) == 32
	checkcode = encode(flag)
	generateFile(checkcode)

if __name__ == '__main__':
	main()
