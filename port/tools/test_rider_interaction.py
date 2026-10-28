"""Exercise the actual rider/NPC distance gate across both native C calls."""
from pathlib import Path
import testprog
import transform

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / "build/port/runtime/rider-interaction"

def main():
    OUT.mkdir(parents=True, exist_ok=True)
    include = OUT / "include"
    transform.port_headers(ROOT / "include", include)
    objects = []
    for name in ("00113398.c", "00113588.c"):
        source = OUT / name
        source.write_text('#include "port.h"\n' + transform.transform(
            (ROOT / "src/unsorted" / name).read_text()))
        objects.append(source)
    driver = OUT / "driver.c"
    driver.write_text(r'''#include "port.h"
#include "rider.h"
#include <stdio.h>
#include <string.h>
int D_004B26AC[1] = {15};
static Rider owner, target;
static SceneNode node;
static RiderPhysics physics;
static int actions;
static float outputY;
void func_00221888(const Matrix4 *matrix, const Vector4 *input, Vector4 *output) {
    output->x = 1; output->y = outputY; output->z = 0; output->w = 1;
}
void func_00112808(RiderPhysics *p, int rider, int mode) { ++actions; }
void func_00112878(RiderPhysics *p, int rider, int mode) { ++actions; }
static void reset(void) {
    memset(&owner, 0, sizeof owner); memset(&target, 0, sizeof target);
    memset(&physics, 0, sizeof physics);
    owner.bodyNode = &node; physics.owner = &owner;
    physics.flags = ((SceneNodeFlags)1 << 50) | ((SceneNodeFlags)1 << 33);
    target.x7A5C = 1; actions = 0; outputY = 0;
}
int main(void) {
    Vector4 position = {1, 0, 0, 1}; int failures = 0;
    reset();
    failures += func_00113588(&physics, &target, 11.0f) != 0;
    failures += func_00113588(&physics, &target, 5.0f) != 1;
    func_00113398(&physics, &target, &position, 11.0f);
    failures += owner.x11A4 != 0 || actions != 0;
    func_00113398(&physics, &target, &position, 5.0f);
    failures += owner.x11A4 != 1 || actions != 1;
    reset(); owner.x6390 = 4; outputY = 20;
    /* The alternate NPC type expands the distance gate to forty. */
    failures += func_00113588(&physics, &target, 41.0f) != 0;
    failures += func_00113588(&physics, &target, 25.0f) != 1;
    func_00113398(&physics, &target, &position, 41.0f);
    failures += owner.x11A4 != 0 || actions != 0;
    func_00113398(&physics, &target, &position, 25.0f);
    failures += owner.x11A4 != 1 || actions != 1;
    owner.x7A68 = 1;
    failures += func_00113588(&physics, &target, 5.0f) != 0;
    printf("Rider/NPC interaction distance forwarding: %d failures\n", failures);
    return failures != 0;
}
''')
    testprog.run(testprog.guest("rider-interaction", [driver, *objects], out=OUT, includes=[include]))

if __name__ == "__main__":
    main()
