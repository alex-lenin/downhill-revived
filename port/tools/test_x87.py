"""Execute the x87 observer against a live value and a masked stack overflow."""
from pathlib import Path
import os
import re
import subprocess
from unittest.mock import patch

from pctarget import HOST_TARGET

ROOT = Path(__file__).resolve().parents[2]
FIXTURE = r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "fp_trap.h"
static void snapshot(unsigned env[7]) {
    __asm__ volatile("fnstenv %0\n\tfldenv %0" : "=m"(*(unsigned (*)[7])env) : : "memory");
}
static void unchanged(unsigned a[7], unsigned b[7]) {
    assert((a[0]&65535)==(b[0]&65535));
    assert((a[1]&65535)==(b[1]&65535));
    assert((a[2]&65535)==(b[2]&65535));
}
int main(void) {
    unsigned a[7], b[7]; float value;
    _putenv_s("DHD_X87_CHECK", "1");
    ps2_x87_checkpoint(0,0);
    __asm__ volatile("fninit");
    snapshot(a); ps2_x87_checkpoint(0x0019EA38,0); snapshot(b); unchanged(a,b);
    __asm__ volatile("fld1");
    snapshot(a); assert((a[2]&65535)!=65535);
    ps2_x87_checkpoint(0x001AD1C0,0); snapshot(b); unchanged(a,b);
    __asm__ volatile("fstps %0" : "=m"(value)); assert(value==1.0f);
    for(int i=0;i<9;i++) __asm__ volatile("fld1");
    snapshot(a); assert((a[1]&0x41)==0x41);
    ps2_x87_checkpoint(0x0023AE78,1); snapshot(b); unchanged(a,b);
    __asm__ volatile("fstps %0" : "=m"(value));
    assert(*(uint32_t *)&value==0xffc00000u);
    __asm__ volatile("fninit");
    puts("x87 observer preserves empty, live and overflowing stack state");
    return 0;
}
'''


def main():
    import recomp
    instructions = [(0x001B4128, 'nop', []), (0x001B45B4, 'nop', []),
                    (0x001B45B8, 'jr', ['$ra']), (0x001B45BC, 'nop', [])]
    with patch.object(recomp, 'parse', return_value=(instructions, {0x001B4128, 0x001B45B4})):
        frame = recomp.translate('func_001B40A0', set(), {})
    assert frame.lines.count('ps2_x87_checkpoint(0x001B4128, 0);') == 1
    assert frame.lines.count('ps2_x87_checkpoint(0x001B4128, 1);') == 1
    out = ROOT / 'build/port/runtime/x87'
    out.mkdir(parents=True, exist_ok=True)
    source = out / 'fixture.c'; source.write_text(FIXTURE)
    exe = out / 'fixture.exe'
    subprocess.run(['clang', '--target=' + HOST_TARGET, '-O1', '-w', '-msse2',
                    '-mfpmath=sse', '-fno-strict-aliasing', '-I', str(ROOT / 'port/runtime'),
                    str(source), str(ROOT / 'port/runtime/fp_trap.c'), '-o', str(exe)], check=True)
    env = {k:v for k,v in os.environ.items() if not k.startswith('DHD_')}
    result = subprocess.run([str(exe)], capture_output=True, text=True, env=env)
    print(result.stdout, end='')
    assert result.returncode == 0, result.stderr
    assert 'site=001AD1C0 before' in result.stderr, result.stderr
    fault = re.search(r'site=0023AE78 after SW=([0-9A-F]+)', result.stderr)
    assert fault and int(fault[1], 16) & 0x41 == 0x41, result.stderr
    assert re.search(r'site=0023AE78 after .* FIP=[0-9A-F]{8}', result.stderr), result.stderr


if __name__ == '__main__': main()
