"""Execute NPC animation state changes against the shared pointer APIs."""
from pathlib import Path
import subprocess
import pctarget
import transform
ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / "build/port/runtime/npc-animation"

def main():
    OUT.mkdir(parents=True, exist_ok=True)
    include = OUT / "include"
    transform.port_headers(ROOT / "include", include)
    source = OUT / "00126B20.cpp"
    source.write_text('#include "port.h"\n' + transform.transform(
        (ROOT / "src/unsorted/00126B20.cpp").read_text()))
    obj = source.with_suffix(".o")
    subprocess.run(["clang", "--target=" + pctarget.TARGET, "-c", "-O1",
                    "-msse2", "-mfpmath=sse", "-ffp-contract=off", "-malign-double",
                    "-I" + str(ROOT / "port/include"), "-I" + str(include),
                    str(source), "-o", str(obj)], check=True)
    driver = OUT / "driver.c"
    driver.write_text(r'''#include "port.h"
#include "animation.h"
#include <stdio.h>
#include <string.h>
extern void func_00126B20(void *, unsigned int);
static unsigned char object[0x134] __attribute__((aligned(16)));
static struct { void *vt; unsigned int **kind; char pad8[8]; AnimationHandle handles[1]; } group;
static AnimationOwner owner;
static AnimationTrack track;
static unsigned int kindWord, *kindPointer = &kindWord;
static int failures, seeks, starts, progress, speeds, currentKind;
static void handle(const AnimationHandle *h) {
 failures += h->x0 != &owner || h->x4 != &track || h->x8 != 7 || h->xC != 9 || h->xE != -17;
}
void func_00222460(void *a, void *b) { ++failures; }
float func_00222B68(float a, float b) { failures += a != 0 || b != 1; return 0.25f; }
void func_00203DC8(const AnimationHandle *h, int mode) {
 handle(h); ++seeks; failures += mode != (currentKind != 0x1018);
}
void func_00202E38(AnimationHandle *h, int mode, int flag, float duration) {
 handle(h); ++starts; failures += mode != (currentKind == 0x1018 || currentKind == 0x101D);
 failures += flag != 1 || duration != (currentKind == 0x1018 ? 100 : 250);
}
void func_00203B68(const AnimationHandle *h, int apply, float value) {
 handle(h); ++progress; failures += apply != 1 || value != 0.25f;
}
void func_00203E10(const AnimationHandle *h, float rate) {
 handle(h); ++speeds; failures += rate != 0.75f;
}
static void run(int kind) {
 memset(object, 0, sizeof object); currentKind = kind; kindWord = (unsigned int)kind << 18;
 *(unsigned int *)(object + 8) = 3; *(float *)(object + 0xDC) = 0.75f;
 *(void **)(object + 0x130) = &group;
 func_00126B20(object, 2);
 failures += *(unsigned int *)(object + 8) != 2;
}
int main(void) {
 group.kind = &kindPointer; group.handles[0].x0 = &owner;
 group.handles[0].x4 = &track; group.handles[0].x8 = 7;
 group.handles[0].xC = 9; group.handles[0].xE = -17;
 run(0x1018); run(0x101D); run(0x1023);
 failures += seeks != 3 || starts != 3 || progress != 1 || speeds != 1;
 printf("NPC animation handle pointers, selector slots and rates: %d failures\n", failures);
 return failures != 0;
}
''')
    exe = driver.with_suffix(".exe")
    subprocess.run(["clang", "--target=" + pctarget.HOST_TARGET, "-fuse-ld=lld", "-O1",
                    "-msse2", "-mfpmath=sse", "-ffp-contract=off", "-malign-double",
                    "-I" + str(ROOT / "port/include"), "-I" + str(include),
                    str(driver), str(obj), "-o", str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
if __name__ == "__main__":
    main()
