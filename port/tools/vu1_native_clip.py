"""Generate a directly threaded native clipping function from local VU1 code.

The decoded operations retain the exact arithmetic/flag pipeline helpers.
Generated instruction words stay under build/, never in tracked source.
The native kernels run only where micro memory holds the words they were written from
(native_code_compare, port/runtime/vu0_interp.c); like the AOT blocks' (port/tools/vu1_aot.py),
those words are copied from the player's disc at start, not kept in dhd.exe.
"""
import os
from pathlib import Path
import struct
from vudis import walk
from vu1_aot import WORDS_MACRO, fill_function, fills, program
ROOT = Path(__file__).resolve().parents[2]

def generate(destination, early_flags=True):
    destination=Path(destination).resolve()
    if ROOT/'build' not in destination.parents:
        raise ValueError('generated clipping code must stay under build/')
    data=Path(os.environ.get('DHD_VUTEXT',ROOT/'assets/vutext.bin')).read_bytes()
    pairs,where=program(data,0x269c50)
    selected=[p for p in sorted(pairs) if 0x1948<=p*8<=0x1ad8 or 0x1cc0<=p*8<=0x2730]
    lines=['/* Generated locally; do not commit. */']+WORDS_MACRO
    entries=[]
    for name,start,end in [
        ('loop',0x170,0x260),
        ('zero',0x270,0x280),
        ('zero1638',0x17c0,0x17e0),
        ('zero17e0',0x1938,0x1948),
        ('loop20c',0x1110,0x11d8),
        ('loop260',0x13a0,0x1420),
        ('loop1638',0x1708,0x17b0),
        ('loop17e0',0x18b0,0x1928),
        ('loop800',0x8b0,0x938),
        ('loop468',0x468,0x578),
        ('loop1430',0x1510,0x1638),
        ('loop2dc8',0x2f00,0x2fc8),
        ('tail2dc8',0x2fc8,0x2fd8),
        ('giftail',0xff8,0x1040),
        ('clip1',0x1948,0x1ae0),
        ('clip2',0x1cc0,0x2738),
    ]:
        words=[w for p in range(start//8,end//8) for w in pairs[p]]
        lines+=[f'AOT_WORDS(native_{name}_words, {len(words)}, '+','.join(f'0x{w:08x}u' for w in words)+')']
        entries+=fills(f'native_{name}_words',data,[where[p] for p in range(start//8,end//8)],words)
    # Particle overlay A uses its own word provenance; keep overlay B's pairs
    # below for the directly threaded clipping emitter.
    particle_pairs,particle_where=program(data,0x265c00)
    for name,start,end in [('loop0e10',0xee0,0xfd0),('tail0e10',0xfd0,0x1010),
                           ('loop2e48',0x2f90,0x30c0),('tail2e48',0x30c0,0x30f8)]:
        words=[w for p in range(start//8,end//8) for w in particle_pairs[p]]
        lines+=[f'AOT_WORDS(native_{name}_words, {len(words)}, '+','.join(f'0x{w:08x}u' for w in words)+')']
        entries+=fills(f'native_{name}_words',data,[particle_where[p] for p in range(start//8,end//8)],words)
    lines+=fill_function('native_words',entries)
    # This closed clip program only reads MAC at 1970 (SUBA at 1950, four
    # issue slots earlier), and CLIP at 1A58/68/78/88/98 and 2600. The newest
    # CLIP before 1A58 is at 1A38, four slots earlier. 2600 is reached through
    # 25F8 after the caller's branch/delay slot and the 25F8 pair; every caller
    # issued its CLIP at least four slots earlier. No status flag reads occur.
    reads={pc*8 for pc in selected if pairs[pc][1]>>31==0 and pairs[pc][0]>>25 in range(0x10,0x1d)}
    assert reads == {0x1970,0x1a58,0x1a68,0x1a78,0x1a88,0x1a98,0x2600}, reads
    lines+=['static inline __attribute__((always_inline)) void native_clip_commit(Vu *u, Upper *up) {',
            'if(up->flags==1){unsigned m=up->mac; unsigned st=(m&15?1:0)|(m&240?2:0)|(m&3840?4:0)|(m&61440?8:0);',
            'u->mac=m;u->status=(u->status&~15u)|st|(st<<6);}',
            'else if(up->flags==2){u->clip=u->clip_next=((u->clip<<6)|up->clip_bits)&0xffffff;}',
            'up->flags=0;commit_upper(u,up,0);}',
            'static unsigned native_clip_run(Vu *u, unsigned resume) {','unsigned count=0; int target=-1,bad=0; Upper up;','goto L1948;']
    def emit(pc):
        lo,hi=pairs[pc]
        out=[f'VU1(u); pair_stalls(u,0x{hi:08x}u,0x{lo:08x}u); apply_flags(u); update_q(u);',
             f'if(upper(u,0x{hi:08x}u,&up)<0) return 0;']
        if hi>>31:
            out+=[f'{{uint32_t word=0x{lo:08x}u;memcpy(&u->c->i,&word,4);}}','target=-1;']
        else:
            out+=[f'target=lower(u,0x{lo:08x}u,{pc},&bad); if(bad)return 0;']
        out += [('native_clip_commit(u,&up);' if early_flags else f'commit_upper(u,&up,{lo>>16&31});') + 'u->cycle++;count++;']
        return out
    branches=(0x20,0x21,0x24,0x25,0x28,0x29,0x2c,0x2d,0x2e,0x2f)
    for pc in selected:
        lo,hi=pairs[pc]
        lines+=[f'L{pc*8:04X}:', 'if(count>=MAX_STEPS)return 0;']+emit(pc)
        if not hi>>31 and lo>>25 in branches:
            lines+=['{ int saved_target=target;']+emit(pc+1)
            if lo>>25 in (0x24,0x25):
                lines+=['target=saved_target;goto indirect; }']
            else:
                imm=lo&0x7ff
                if imm&0x400:imm-=0x800
                lines += [f'if(saved_target>=0)goto L{(pc+1+imm)*8:04X}; goto L{(pc+2)*8:04X}; }}']
        else:
            # Delay slots emitted above can also be jumped to independently.
            lines += [f'goto L{(pc+1)*8:04X};'] if pc+1 in selected else ['return 0;']
    lines+=['indirect:','if(target==(int)resume){drain_flags(u);update_q(u);return count;}','switch(target){']
    # Only computed returns used by this closed subprogram; default rejects corruption.
    for pc in selected:
        lo,hi=pairs[pc]
        if not hi>>31 and lo>>25 in (0x21,0x25):
            lines+=[f'case {(pc+2)}:goto L{(pc+2)*8:04X};']
    lines+=['default:return 0;','}','}']
    destination.write_text('\n'.join(lines)+'\n')
