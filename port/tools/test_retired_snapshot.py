"""Deterministically retire a queue between its predicate and its wait.

Compile the production wait functions with a scripted publication counter; waiting
on the already-completed value is a failure. No wall-clock threshold or scheduler
luck is involved. --source-rev runs the same check on an older implementation.
"""
from pathlib import Path
import argparse
import os
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]


def function(source, name):
    import re
    match = re.search(r'(?:static )?void ' + name + r'\([^)]*\)\s*\{', source)
    if not match:
        raise ValueError(name)
    depth, end = 1, match.end()
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[match.start():end]


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source-rev')
    a = ap.parse_args()
    def read(path):
        if a.source_rev:
            return subprocess.check_output(['git', 'show', a.source_rev + ':' + path], cwd=ROOT, text=True)
        return (ROOT / path).read_text()
    queue = read('port/hle/graphics_queue.c')
    gs = read('port/hle/gs_queue.inc')
    out = ROOT / 'build/port/retired-snapshot-test'
    out.mkdir(parents=True, exist_ok=True)
    harness = r'''
#include <stdint.h>
#define GRAPHICS_QUEUE_TEST 1
#define NULL ((void *)0)
typedef int32_t LONG;
typedef int64_t LONG64;
static LONG consumed, written, waiting, gs_consumed, gs_written, gs_waiting;
static int worker=1, gs_worker=1, retired=1, gs_retired=1;
static unsigned reads, retire_read, sleeping_on_completed, calls, done;
static uint32_t position(volatile LONG *p) {
    if (p == &written || p == &gs_written) return (uint32_t)*p;
    if (done || ++reads >= retire_read) return (uint32_t)written;
    return (uint32_t)written - 16;
}
#define gs_position position
static LONG64 tl_begin(void) {return 0;}
static LONG64 tl_now(void) {return 0;}
static void tl_add(int a, LONG64 b) {}
static void tl_site(void *a, int b, LONG64 c) {}
#define TL_EE_VU 0
#define TL_EE_GS 1
static void idle_wait(volatile LONG *p, uint32_t at, volatile LONG *flag,
                      int event, unsigned timeout, unsigned *budget) {
    /* The producer publishes its final retirement now. There is no next packet. */
    done=1; calls++;
    if (at == (uint32_t)written) sleeping_on_completed++;
}
'''
    harness += '\n' + function(queue, 'wait_retired')
    harness += '\n' + function(queue, 'graphics_wait')
    harness += '\n' + function(gs, 'graphics_gs_wait')
    harness += r'''
int main(void) {
    for (unsigned wrap=0; wrap<2; wrap++) {
        written=gs_written=wrap ? 8 : 4096;
        reads=done=0; retire_read=3;
        graphics_wait();
        reads=done=0; retire_read=2;
        graphics_gs_wait();
    }
    return sleeping_on_completed || calls != 4;
}
'''
    source = out / 'test.c'; source.write_text(harness)
    llvm = (Path.home() / 'scoop/apps/llvm/current/bin')
    msvc = Path('C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.51.36231')
    sdk = Path('C:/Program Files (x86)/Windows Kits/10')
    version = '10.0.26100.0'
    exe = out / 'test.exe'
    cmd = [str(llvm/'clang-cl.exe'), '/nologo', '/O2', '/MT', '/imsvc'+str(msvc/'include'),
           '/imsvc'+str(sdk/'Include'/version/'ucrt'), str(source), '/Fe:'+str(exe), '/link',
           '/libpath:'+str(msvc/'lib/x64'), '/libpath:'+str(sdk/'Lib'/version/'ucrt/x64'),
           '/libpath:'+str(sdk/'Lib'/version/'um/x64')]
    env = dict(os.environ, PATH=str(llvm)+';'+os.environ['PATH'])
    subprocess.run(cmd, cwd=out, env=env, check=True, timeout=60)
    result = subprocess.run([str(exe)], timeout=10)
    print(('PASS' if result.returncode == 0 else 'FAIL') + ': retirement between predicate and wait (EE, GS, counter wrap)')
    return result.returncode


if __name__ == '__main__':
    sys.exit(main())
