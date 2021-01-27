# python2 disassembler.py

ins_num = 0

def print_instruction(ins, desc):
    global ins_num
    print('%02x %-5s %s'%(ins_num, ins.encode('hex'), desc))
    ins_num += len(ins.encode('hex')) / 2

def _exit(n=0):
    ret = '\x00'
    print_instruction(ret, 'exit')
    return ret

def _mov(op1, op2, reg=True, write=False):
    
    src = ''
    if(write):
        ret = chr(0x30 | op2) + chr(op1)
        src = 'mem[%02x], r%d'%(op1, op2)
    elif(reg):
        ret = chr(0x10 | op1) + chr(op2)
        src = 'r%d, r%d'%(op1, op2)
    else:
        ret = chr(0x20 | op1) + chr(op2)
        src = 'r%d, mem[%02x]'%(op1, op2)
    
    print_instruction(ret, 'mov %s'%src)
    return ret

def _add(r1, r2):
    ret = chr(0x40 | r1) + chr(r2)
    print_instruction(ret, 'add r%d, r%d'%(r1, r2))
    return ret

def _sub(r1, r2):
    ret = chr(0x50 | r1) + chr(r2)
    print_instruction(ret, 'sub r%d, r%d'%(r1, r2))
    return ret

def _xor(r1, r2):
    ret = chr(0x60 | r1) + chr(r2)
    print_instruction(ret, 'xor r%d, r%d'%(r1, r2))
    return ret

def _and(r1, r2):
    ret = chr(0x70 | r1) + chr(r2)
    print_instruction(ret, 'and r%d, r%d'%(r1, r2))
    return ret

def _or(r1, r2):
    ret = chr(0x80 | r1) + chr(r2)
    print_instruction(ret, 'or r%d, r%d'%(r1, r2))
    return ret

def _clflush():
    ret = chr(0x99)
    print_instruction(ret, 'clflush')
    return ret

def _not(reg):
    ret = chr(0xa0 | reg)
    print_instruction(ret, 'not r%d'%reg)
    return ret

def _enc(reg):
    ret = chr(0xb0 | reg)
    print_instruction(ret, 'enc r%d'%reg)
    return ret

def _dec(reg):
    ret = chr(0xc0 | reg)
    print_instruction(ret, 'dec r%d'%reg)
    return ret

def _jz(offset):
    if offset >= 0x80:
        print('-0x7F <= offset <= +0x7F // check it again!')
        return
    if offset < 0:
        offset += 0x100
    ret = chr(0xd0) + chr(offset)
    print_instruction(ret, 'jz %02X'%offset)
    return ret

def _jnz(offset):
    if offset >= 0x80:
        print('-0x7F <= offset <= +0x7F // check it again!')
        return
    if offset < 0:
        offset += 0x100
    ret = chr(0xe0) + chr(offset)
    print_instruction(ret, 'jnz %02X'%offset)
    return ret

def _show_register(n=0):
    ret = '\xFF'
    print_instruction(ret, 'show_register')
    return ret

if __name__ == '__main__':
    print('compile the encryption program . . .\n')
    program = ''
    program += _show_register()
    program += _mov(0, 0x80, reg=False)
    program += _mov(1, 0x81, reg=False)
    program += _sub(0, 1)
    program += _show_register()
    program += _dec(0)
    program += _jz(-4)
    program += _mov(1, 0x81, reg=False)
    program += _show_register()
    program += _exit()

    f = open('enc.program', 'w')
    f.write(program)
    f.close()

    print('\ncompile finished')
