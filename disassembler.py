# python2 disassembler.py

def print_instruction(n, ins, desc):
    print('%02x %-5s %s'%(n, ins.encode('hex'), desc))

def exit(n=0):
    ret = '\x00\x00'
    print_instruction(n, ret, 'exit')
    return ret

def _mov(op1, op2, n=0, reg=True):
    
    src = ''
    if(reg):
        ret = chr(0x10 & op1) + chr(op2)
        src = 'r%d'%op2
    else:
        ret = chr(0x20 & op1) + chr(op2)
        src = 'mem[%02x]'%op2
    
    print_instruction(n, ret, 'mov r%d, %s'%(op1, src))
    return ret

def _add(r1, r2, n=0):
    ret = chr(0x30 & r1) + chr(r2)
    print_instruction(n, ret, 'add r%d, r%d'%(r1, r2))
    return ret

def _sub(r1, r2, n=0):
    ret = chr(0x40 & r1) + chr(r2)
    print_instruction(n, ret, 'sub r%d, r%d'%(r1, r2))
    return ret

def _xor(r1, r2, n=0):
    ret = chr(0x50 & r1) + chr(r2)
    print_instruction(n, ret, 'xor r%d, r%d'%(r1, r2))
    return ret

def _and(r1, r2, n=0):
    ret = chr(0x60 & r1) + chr(r2)
    print_instruction(n, ret, 'and r%d, r%d'%(r1, r2))
    return ret

def _or(r1, r2, n=0):
    ret = chr(0x70 & r1) + chr(r2)
    print_instruction(n, ret, 'or r%d, r%d'%(r1, r2))
    return ret

def _shl(r1, r2, n=0):
    ret = chr(0x80 & r1) + chr(r2)
    print_instruction(n, ret, 'shl r%d, r%d'%(r1, r2))
    return ret

def _shr(r1, r2, n=0):
    ret = chr(0x90 & r1) + chr(r2)
    print_instruction(n, ret, 'shr r%d, r%d'%(r1, r2))
    return ret

def _not(reg, n=0):
    ret = chr(0xa0 & reg) + '\x00'
    print_instruction(n, ret, 'not r%d'%reg)
    return ret

def _enc(reg, n=0):
    ret = chr(0xb0 & reg) + '\x00'
    print_instruction(n, ret, 'enc r%d'%reg)
    return ret

def _dec(reg, n=0):
    ret = chr(0xc0 & reg) + '\x00'
    print_instruction(n, ret, 'dec r%d'%reg)
    return ret

def _jz(offset, n=0):
    if offset >= 0x80:
        print('-0x7F <= offset <= +0x7F // check it again!')
        return
    if offset < 0:
        offset += 0x100
    ret = chr(0xd0) + chr(offset)
    print_instruction(n, ret, 'jz %02X'%offset)
    return ret

def _jnz(offset, n=0):
    if offset >= 0x80:
        print('-0x7F <= offset <= +0x7F // check it again!')
        return
    if offset < 0:
        offset += 0x100
    ret = chr(0xe0) + chr(offset)
    print_instruction(n, ret, 'jnz %02X'%offset)
    return ret

def _show_register(n=0):
    ret = '\xFF'
    print_instruction(n, ret, 'show_register')
    return ret

if __name__ == '__main__':
    print('compile the encryption program . . .\n')
    program = ''
    program += _show_register(n=0)
    program += _mov(0, 0x80, n=1, reg=False)
    program += _mov(1, 0x81, n=3, reg=False)
    program += _mov(2, 0x82, n=5, reg=False)
    program += _mov(3, 0x83, n=7, reg=False)
    program += _show_register(n=9)

    f = open('enc.program', 'w')
    f.write(program)
    f.close()

    print('\ncompile finished')
