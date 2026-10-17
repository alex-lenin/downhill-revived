"""Execute the real host fiber wrappers with distinct live x87 and MXCSR state."""
from pathlib import Path
import subprocess
import sys
from pctarget import HOST_TARGET

ROOT = Path(__file__).resolve().parents[2]
DRIVER = r'''
#include <windows.h>
#include <assert.h>
#include <xmmintrin.h>
#include <stdio.h>
static void *parent, *child;
static void worker(void *arg) {
    unsigned short status; float value;
    (void)arg;
    __asm__ volatile("fninit\n\tfld1\n\tfld1\n\tfaddp");
    _mm_setcsr(0x9f80);
    host_fiber_switch(parent);
    assert(_mm_getcsr()==0x9f80);
    __asm__ volatile("fstps %0" : "=m"(value)); assert(value==2.0f);
    __asm__ volatile("fstp %%st(0)\n\tfnstsw %0" : "=m"(status));
    assert((status&0x41)==0x41);
    host_fiber_switch(parent);
    __asm__ volatile("fninit");
    host_fiber_switch(parent);
}
int main(void) {
    float value; unsigned short status;
    __asm__ volatile("fninit");
    parent=host_fiber_main(); child=host_fiber_create(worker,0); assert(parent&&child);
    _mm_setcsr(0x1f80);
    __asm__ volatile("fld1");
    host_fiber_switch(child);
    assert(_mm_getcsr()==0x1f80);
    __asm__ volatile("fstps %0" : "=m"(value)); assert(value==1.0f);
    host_fiber_switch(child);
    __asm__ volatile("fnstsw %0" : "=m"(status)); assert(!(status&0x41));
    host_fiber_switch(child);
    host_fiber_delete(child); ConvertFiberToThread();
    puts("Host fibers preserve live x87 values, sticky faults and SSE control independently");
    return 0;
}
'''


def main():
    text = (ROOT / 'port/hle/host.c').read_text()
    text = text[text.index('void *host_fiber_main'):text.index('double host_wall_seconds')]
    if '--old' in sys.argv:
        text = text.replace('ConvertThreadToFiberEx(NULL, FIBER_FLAG_FLOAT_SWITCH)', 'ConvertThreadToFiber(NULL)')
        text = text.replace('CreateFiberEx(0, 0x40000, FIBER_FLAG_FLOAT_SWITCH,', 'CreateFiber(0x40000,')
        start = text.index('void host_fiber_switch')
        end = text.index('void host_fiber_delete', start)
        text = text[:start] + 'void host_fiber_switch(void *fiber) { SwitchToFiber(fiber); }\n' + text[end:]
    out = ROOT / 'build/port/runtime/fiber-float'; out.mkdir(parents=True, exist_ok=True)
    source = out / 'fixture.c'; source.write_text('#include <windows.h>\n' + text + DRIVER)
    exe = out / 'fixture.exe'
    subprocess.run(['clang', '--target=' + HOST_TARGET, '-O1', '-w', '-msse2', '-mfpmath=sse',
                    str(source), '-o', str(exe)], check=True)
    result = subprocess.run([str(exe)], capture_output=True, text=True)
    print(result.stdout, end='')
    if '--old' in sys.argv:
        assert result.returncode != 0, 'unprotected fiber unexpectedly preserves FP state'
        print('Old unprotected host fibers fail the FP context fixture')
    else: assert result.returncode == 0, result.stderr


if __name__ == '__main__': main()
