"""Verify discarded NPC float returns do not leak the native x87 stack."""
from pathlib import Path
import subprocess
import sys
import pctarget
import transform

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / "build/port/runtime/npc-float-return"

def main():
    OUT.mkdir(parents=True, exist_ok=True)
    include = OUT / "include"
    transform.port_headers(ROOT / "include", include)
    objects = []
    for name in ("0015A968.c", "0015AB08.c"):
        source = OUT / name
        text = (ROOT / "src/unsorted" / name).read_text()
        if "--wrong-prototype" in sys.argv and name == "0015AB08.c":
            text = text.replace("func_0015A968(", "DiscardNpcProbe(")
            text = text.replace('#include "npc_motion.h"', '#include "npc_motion.h"\nextern void DiscardNpcProbe(NpcMotionActor *) asm("func_0015A968");')
        source.write_text('#include "port.h"\n' + transform.transform(text, label_prefix="_"))
        obj = source.with_suffix(".o")
        subprocess.run(["clang", "--target=" + pctarget.TARGET, "-c", "-O1",
                        "-Wno-gnu-folding-constant", "-msse2", "-mfpmath=sse", "-ffp-contract=off", "-malign-double",
                        "-I" + str(ROOT / "port/include"), "-I" + str(include),
                        str(source), "-o", str(obj)], check=True)
        objects.append(str(obj))
    driver = OUT / "driver.c"
    driver.write_text(r'''#include "port.h"
#include "npc_motion.h"
#include <stdio.h>
extern void func_0015AB08(void *);
NpcMotionWorld D_002AF2C0;
float D_0029C6E4[1] = {123.5f};
static SceneNode node;
static unsigned char actor[0x200] __attribute__((aligned(16)));
int func_0011C0D0(NpcMotionWorld *w, float *p, int mode, float value) { return 0; }
char *func_0011B180(NpcMotionWorld *w, float *p, float *d, int mode, float value) { return 0; }
float func_001385A8(void *p) { return 0; }
void func_00159CE0(NpcMotionActor *p, int state) {}
void func_0015A890(NpcMotionActor *p) {}
int func_00222BD8(int a, int b) { return 0; }
int main(void) {
 unsigned short status; int failures = 0, i;
 NpcMotionActor *p = (NpcMotionActor *)actor;
 p->state = 4; p->node = &node;
 failures += func_0015A968(p) != 123.5f;
 __asm__ volatile("fninit");
 for (i = 0; i < 16; ++i) func_0015AB08(actor);
 __asm__ volatile("fnstsw %%ax" : "=a"(status));
 failures += (status & 0x3841) != 0;
 printf("NPC discarded float return/x87 balance: %d failures (status 0x%04x)\n", failures, status);
 return failures != 0;
}
''')
    exe = driver.with_suffix(".exe")
    subprocess.run(["clang", "--target=" + pctarget.HOST_TARGET, "-fuse-ld=lld", "-O1",
                    "-Wno-gnu-folding-constant", "-msse2", "-mfpmath=sse", "-ffp-contract=off", "-malign-double",
                    "-I" + str(ROOT / "port/include"), "-I" + str(include),
                    str(driver), *objects, "-o", str(exe)], check=True)
    subprocess.run([str(exe)], check=True)

if __name__ == "__main__":
    main()
