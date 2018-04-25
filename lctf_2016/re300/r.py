

from Crypto.Cipher import AES
from struct import unpack
from hashlib import md5

with open('./re300') as fp:
	fp.seek(0xe81ff)
	key = fp.read(16)

with open('./dump.pcap') as fp:
	data = fp.read()
	idex = data.find('\x06\x00\x00\x00')
	data = data[idex+4:idex+0x1a4]

aes  = AES.new(key , AES.MODE_ECB)
text = aes.decrypt(data)
ip   = text[:text.find('\x00')]
port = unpack('<H',text[0x1d4-0x54:][:2])[0]

flag = md5(ip+':'+str(port)).hexdigest()
print 'flag is: {0}'.format(flag)
