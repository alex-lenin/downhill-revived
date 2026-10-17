"""Exercise rider input/camera channel selection and float forwarding."""
from pathlib import Path
import subprocess
import pctarget
import transform

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / "build/port/runtime/rider-input"

def main():
    OUT.mkdir(parents=True, exist_ok=True)
    include = OUT / "include"
    transform.port_headers(ROOT / "include", include)
    objects = []
    for name in ("001B2E50.c", "001B3028.c", "001B3068.c"):
        source = OUT / name
        source.write_text('#include "port.h"\n' + transform.transform(
            (ROOT / "src/unsorted" / name).read_text()))
        obj = source.with_suffix(".o")
        subprocess.run(["clang", "--target=" + pctarget.TARGET, "-c", "-O1",
                        "-Wno-gnu-folding-constant", "-msse2", "-mfpmath=sse", "-ffp-contract=off", "-malign-double",
                        "-I" + str(ROOT / "port/include"), "-I" + str(include),
                        str(source), "-o", str(obj)], check=True)
        objects.append(str(obj))
    driver = OUT / "driver.c"
    driver.write_text(r'''#include "port.h"
#include "camera.h"
#include <stdio.h>
extern void func_001B3068(RiderCamera *);
static RiderCamera camera;
static int resets;
void func_001B3108(RiderCamera *p) { resets += p == &camera; }
int main(void) {
 int failures = 0, i;
 func_001B3068(&camera); failures += resets != 1;
 for (i = 0; i < 13; ++i) {
  failures += camera.parameters[i].enabled != 1;
  func_001B3028(&camera, i, (i + 1) * 0.25f);
 }
 for (i = 0; i < 13; ++i) failures += camera.parameters[i].value != (i + 1) * 0.25f;
 camera.parameters[7].enabled = 0;
 func_001B3028(&camera, 7, 17.25f);
 failures += camera.parameters[7].value != 2.0f;
 func_001B3028(&camera, 3, 0.0f);
 failures += camera.parameters[3].value != 1.0f;
 func_001B3028(&camera, 2, -0.75f);
 failures += camera.parameters[2].value != -0.75f;
 printf("Rider input channel index/value forwarding: %d failures\n", failures);
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
