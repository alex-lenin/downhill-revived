"""Exercise the real rider phase setter and idle caller across their native ABI."""
from pathlib import Path
import subprocess
import sys
import re

import pctarget
import transform

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port'))
import patches


def prepare(directory, patched=True, patched_handle=True):
    out = Path(directory) / 'rider-time'
    out.mkdir(parents=True, exist_ok=True)
    sources = []
    for name in ('001AD1C0.c', '001AD580.c', '001825B0.c', '001F87B0.c', '00177DA0.c'):
        source = (ROOT / 'src/unsorted' / name).read_text()
        if (patched or name != '001AD580.c') and (patched_handle or name != '001825B0.c'):
            for kind, old, new in patches.PATCHES.get('unsorted/' + name, []):
                if kind == 're':
                    pattern, count = old
                    source, found = re.subn(pattern, new, source)
                    assert found == count
                else:
                    assert kind == 'text' and source.count(old) == 1
                    source = source.replace(old, new)
        if not patched_handle and name == '001825B0.c':
            # The size repair now lives in src; retain the old overflow fixture.
            source = source.replace('typedef AnimationHandle Obj;',
                                    'typedef struct { int x0, x4, x8; } Obj;')
            source = source.replace('(&obj,', '((AnimationHandle *)&obj,')
        source = source.replace('#include "animation.h"',
                                (ROOT / 'include/animation.h').read_text())
        path = out / name
        path.write_text('#include "port.h"\n' + transform.transform(source))
        sources.append(path)
    driver = out / 'driver.c'
    driver.write_text(r'''
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
void ps2_x87_checkpoint(unsigned site, unsigned after) {(void)site; (void)after;}
extern void func_001AD1C0(void *);
extern void func_001AD580(void *, int, float, float);
extern void func_001825B0(void);
extern void func_001F87B0(void *);
extern void func_00177DA0(void *);
extern int host_sprintf(char *, const char *, ...);
static unsigned char ctrl[0x144], body[0x7A7C];
static int failures, queries;
char D_002FF590[16];
char D_004B25A0[0x294];
static int loads, lookups, starts, seeks;
static int menu_run, menu_lookups, menu_starts, menu_transitions;
float D_0029DD88 = 1;
int D_0043F348[1];
char D_0029C9D0[] = "%2.0f ";
typedef struct { void *p; unsigned int size; } ps2_op;
void ps2_asm(const char *text, const char *constraints, int n, ps2_op *ops) {
    float x, y, z;
    failures += n != 4;
    x = *(float *)ops[1].p; y = *(float *)ops[2].p; z = *(float *)ops[3].p;
    *(float *)ops[0].p = sqrtf((x*x + y*y) + z*z);
}
static void speed_case(float speed, uint64_t flags, int mode, int ones, int tens) {
    unsigned char object[0x1349] = {0}, race[0x4190] = {0};
    uint64_t node = UINT64_C(0xFEDCBA9876543210);
    uint64_t expected = (node & ~UINT64_C(0x3F)) | (tens ? 0 : 9);
    *(void **)(object + 0xF24) = &node;
    *(void **)(object + 0x1164) = race;
    *(uint64_t *)(race + 0x4098) = flags;
    *(float *)(race + 0x4180) = speed;
    object[0x1347] = 7; object[0x1348] = 8;
    D_0043F348[0] = mode;
    func_00177DA0(object);
    failures += object[0x1344] != 7 || object[0x1345] != 8;
    failures += object[0x1347] != ones || object[0x1348] != tens || node != expected;
}
void func_0011C6F0(void *p) { failures += p != D_002FF590; loads++; }
void func_001DFFF0(void) { loads++; }
void func_00203590(void *p, int resource, int index, int slot) {
    unsigned int handle[4] = {0x11223344, 0x55667788, 0, 0x56780000u + index};
    if (menu_run) {
        int resources[] = {0x2588, 0x23D7, 80, 0x25EE, 0x384};
        int slots[] = {5, 1, 250, 250, 250};
        failures += menu_lookups >= 5 || index != 0;
        if (menu_lookups < 5)
            failures += resource != resources[menu_lookups] || slot != slots[menu_lookups];
        handle[3] = 0x56780000u + menu_lookups++;
        memcpy(p, handle, sizeof handle); return;
    }
    failures += resource != 0x32C8 || index != lookups || slot != 0;
    memcpy(p, handle, sizeof handle); lookups++;
}
static void check_resource_handle(void *p, int index) {
    unsigned int handle[4] = {0x11223344, 0x55667788, 0, 0x56780000u + index};
    failures += memcmp(p, handle, sizeof handle) != 0;
}
void func_002038E0(void *p, int force) {
    check_resource_handle(p, starts++); failures += force != 1;
}
void func_00203B68(void *p, int apply, float fraction) {
    check_resource_handle(p, seeks);
    failures += apply != 1 || fraction != (float)seeks * 0.1666f; seeks++;
}
void func_002039B0(void *p, int force) {
    check_resource_handle(p, menu_starts++); failures += force != 1;
}
void func_00202E38(void *p, int enable, int mode, float duration) {
    check_resource_handle(p, menu_transitions++ + 2);
    failures += enable != 1 || mode != 1 || duration != 300.0f;
}
#define F(off) (*(float *)(ctrl + (off)))
float func_002041D0(void *handle) {
    unsigned int expected[4] = {0x11223344, 0x55667788, 0x99aabbcc, 0x12345678};
    failures += memcmp(handle, expected, 16) != 0;
    queries++;
    return 120.0f;
}
float func_001385A8(void *p) { failures++; return 0; }
float func_001ACE98(void *p) { failures++; return 0; }
void func_001AD700(void *p, float value) { failures++; }
void func_001AD710(void *p) { failures++; }
static void reset(float phase) {
    unsigned int handle[4] = {0x11223344, 0x55667788, 0x99aabbcc, 0x12345678};
    memset(ctrl, 0, sizeof ctrl);
    memset(body, 0, sizeof body);
    *(void **)(ctrl + 0x140) = body;
    memcpy(ctrl + 0x58, handle, sizeof handle);
    F(0x24) = phase;
}
static void check(float target, float rate, float original) {
    failures += !isfinite(F(0x1C)) || fabsf(F(0x1C) - rate) > 0.000001f;
    failures += F(0x20) != F(0x1C) || F(0x2C) != target || F(0x30) != original;
    failures += ctrl[0x18] != 1;
}
int main(void) {
    int modes[] = {0, 1, 0x12345678};
    int i;
    for (i = 0; i < 3; i++) {
        reset(0); func_001AD580(ctrl, modes[i], 0, 1); check(0, 0, 0);
        reset(0); func_001AD580(ctrl, modes[i], 0.25f, 0.5f); check(0.25f, 3, 0);
        reset(0); func_001AD580(ctrl, modes[i], 0.75f, 0.5f); check(0.75f, -3, 0);
        reset(1.75f); func_001AD580(ctrl, modes[i], 1.25f, 2); check(0.25f, -1.5f, 1.75f);
    }
    /* The real idle caller passes mode=1, target at +0x50, and duration=1. */
    reset(0); F(0x50) = 0; ctrl[0x19] = 1;
    func_001AD1C0(ctrl); check(0, 0, 0); failures += ctrl[0x19] != 0;
    reset(0.25f); F(0x50) = 0.75f;
    func_001AD1C0(ctrl); check(0.75f, 3, 0.25f);
    failures += queries != 14;
    func_001825B0();
    failures += loads != 2 || lookups != 6 || starts != 6 || seeks != 6;
    {
        unsigned char menu[0x514] = {0};
        *(int *)(D_004B25A0 + 0x290) = 64;
        *(int *)(menu + 0x510) = 3;
        menu_run = 1; func_001F87B0(menu);
        failures += *(int *)(menu + 0x500) != 50 || *(int *)(menu + 0x504) != 50;
        failures += *(int *)(menu + 0x508) != -1 || *(int *)(menu + 0x50C) != 60;
        failures += *(int *)(menu + 0x510) != 0;
        failures += menu_lookups != 5 || menu_starts != 2 || menu_transitions != 3;
    }
    speed_case(0, 0, 0, 0, 0); speed_case(10, 0, 0, 7, 0);
    speed_case(100, 0, 0, 8, 6); speed_case(200, 0, 0, 9, 9);
    speed_case(100, 4, 0, 0, 0); speed_case(100, 0, 300, 0, 0);
    {
        char formatted[32];
        failures += host_sprintf(formatted, "%s %d %.2f", "x", 7, 1.25) != 8;
        failures += strcmp(formatted, "x 7 1.25") != 0;
    }
    printf("Rider phase, handle and speedometer callers: %d failures\n", failures);
    return failures != 0;
}
''')
    return sources, driver


def main():
    patched = '--unpatched' not in sys.argv
    sources, driver = prepare(ROOT / 'build/port/runtime', patched,
                              '--unpatched-handle' not in sys.argv)
    objects = []
    for source in sources:
        obj = source.with_suffix('.o')
        subprocess.run(['clang', '--target=' + pctarget.TARGET, '-c', '-O1',
                        '-fstack-protector-all',
                        '-msse2', '-mfpmath=sse', '-ffp-contract=off', '-malign-double',
                        '-I', str(ROOT / 'port/include'), '-I', str(ROOT / 'include'),
                        str(source), '-o', str(obj)], check=True)
        objects.append(str(obj))
    exe = driver.with_suffix('.exe')
    shim = driver.parent / 'libc_shims.o'
    subprocess.run(['clang', '--target=' + pctarget.HOST_TARGET, '-c', '-O1',
                    '-Wno-deprecated-declarations', str(ROOT / 'port/runtime/libc_shims.c'),
                    '-o', str(shim)], check=True)
    subprocess.run(['clang', '--target=' + pctarget.HOST_TARGET, '-fuse-ld=lld', '-O1',
                    '-msse2', '-mfpmath=sse', '-ffp-contract=off', str(driver), *objects,
                    str(shim), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)


if __name__ == '__main__':
    main()
