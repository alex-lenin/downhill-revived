import struct, collections, rabbitizer as rz
d=open(__import__('sys').argv[1],'rb').read()
# .text: find via section headers
shoff,=struct.unpack_from('<I',d,32); shnum,shstr=struct.unpack_from('<HH',d,48)
secs=[struct.unpack_from('<IIIIIIIIII',d,shoff+i*40) for i in range(shnum)]
so=secs[shstr][4]
def nm(s): return d[so+s[0]:d.index(b'\0',so+s[0])].decode()
t=[s for s in secs if nm(s)=='.text'][0]
base,off,size=t[3],t[4],t[5]
words=struct.unpack_from('<%dI'%(size//4),d,off)
rz.config.misc_omit0XOnSmallImm=False
stats=collections.Counter(); funcs=0
ra_save=collections.Counter()
for i,w in enumerate(words):
    ins=rz.Instruction(w,base+i*4)
    s=ins.disassemble()
    # prologue: addiu sp,sp,-N
    if s.startswith('addiu') and '$sp, $sp, -' in s:
        funcs+=1
        for j in range(1,12):
            if i+j>=len(words): break
            s2=rz.Instruction(words[i+j],base+(i+j)*4).disassemble()
            if '$ra,' in s2 and '($sp)' in s2:
                ra_save[s2.split()[0]]+=1; break
print('prologues',funcs); print('ra save op:',ra_save.most_common())
sreg=collections.Counter(); shown=0
for i,w in enumerate(words):
    s=rz.Instruction(w,base+i*4).disassemble()
    if s.startswith('addiu') and '$sp, $sp, -' in s:
        block=[rz.Instruction(words[i+j],base+(i+j)*4).disassemble() for j in range(0,8) if i+j<len(words)]
        for b in block[1:]:
            if '($sp)' in b and b.split()[0] in('sq','sd','sw') and '$s' in b: sreg[b.split()[0]]+=1
        if shown<4 and sum('$s' in b and '($sp)' in b for b in block)>=2:
            shown+=1; print(hex(base+i*4)); print('   '+'\n   '.join(block))
print('s-reg save ops:',sreg.most_common())
