from Crypto.Cipher import AES
from chacha20poly1305 import ChaCha20Poly1305
from chacha20poly1305.poly1305 import Poly1305
import time
import struct
import socket
import random
from threading import Thread

dev_id = '\xee\xad\xbe\xbb'

def get_randstr(n):
    rs = []
    for _ in xrange(n):
        rs.append(random.randrange(0,256))
    return ''.join(map(chr, rs))

def get_timetag():
    localtime = list(time.localtime(time.time())[:6])
    print localtime
    localtime[0] = localtime[0] - 2000
    localtime[3] = localtime[3] - 8
    return ''.join(map(chr, localtime))

def encrypt(dev_id, data):
    secret_key = ''.join(map(chr, xrange(0,16))) * 2
    pt = "%016u" % struct.unpack('>I', dev_id)[0]
    master_key = AES.new(secret_key).encrypt(pt).encode('hex')
    timetag = get_timetag()
    session_key = str(Poly1305(bytearray(master_key)).create_tag(bytearray(timetag))).encode('hex')
    cip = ChaCha20Poly1305(bytearray(session_key))
    nonce = get_randstr(12)
    ct = cip.encrypt(bytearray(nonce), bytearray(data), associated_data=bytearray(dev_id))

    # packet
    p  = ''
    p += dev_id
    p += '\xff\xff'
    p += timetag
    p += nonce
    p += str(ct)[-16:]
    p += struct.pack('>H', len((str(data))))
    p += str(ct)[:-16]
    return p

def decrypt(d):
    dev_id = d[0:4]
    cmd =    d[4:6]
    timetag = d[6:12]
    nonce   = d[12:24]
    tag     = d[24:40]
    alen    = d[40:42]
    data    = d[42:]
    secret_key = ''.join(map(chr, xrange(0,16))) * 2
    pt = "%016u" % struct.unpack('>I', dev_id)[0]
    master_key = AES.new(secret_key).encrypt(pt).encode('hex')
    session_key = str(Poly1305(bytearray(master_key)).create_tag(bytearray(timetag))).encode('hex')
    cip = ChaCha20Poly1305(bytearray(session_key))
    ct = cip.decrypt(bytearray(nonce), bytearray(data+tag), associated_data=bytearray(dev_id))
    return str(ct)

def pack(dev_id, cmd, d):
    p  = ''
    p += dev_id
    p += struct.pack('>H', cmd)
    p += struct.pack('>H', len((d)))
    p += d
    return p

def unpack(p):
    dev_id = p[:4]
    cmd = struct.unpack('>H', p[4:6])
    #print cmd
    alen = struct.unpack('>H', p[6:8])
    #print alen
    data = p[8:]
    return data

########################################################
def cmd_0000():
    d = 'ping'
    print d
    data = encrypt(dev_id, pack(dev_id, 0x0000, d) )
    return data

def cmd_0001():
    data = encrypt(dev_id, pack(dev_id, 0x0100, ''))
    return data

def cmd_0002():
    data = encrypt(dev_id, pack(dev_id, 0x0200, '12345678'))
    return data

def cmd_0003():
    data = encrypt(dev_id, pack(dev_id, 0x0300, '12345678'))
    return data

def cmd_0eee():
    data = encrypt(dev_id, pack(dev_id, 0xee0e, '0C7f2@1X'))
    #data = encrypt(dev_id, pack(dev_id, 0xee0e, '12345678'))
    return data

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
target = ("192.168.201.19", 8081)

data_0000 = cmd_0000()
sock.sendto(data_0000, target)
d = sock.recv(1024)
p = unpack(decrypt(d))
print p

data_0001 = cmd_0001()
sock.sendto(data_0001, target)
d = sock.recv(1024)
p = unpack(decrypt(d))
print 'time', p.encode('hex')

data_0002 = cmd_0002()
sock.sendto(data_0002, target)

#time.sleep(0.5)

data_0003 = cmd_0003()
sock.sendto(data_0003, target)

data_0eee = cmd_0eee()
sock.sendto(data_0eee, target)
d = sock.recv(1024)
p = unpack(decrypt(d))
print 'flag:', p
