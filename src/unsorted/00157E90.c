/* UNIT 0x00157E90 0x00157F60 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

typedef struct Entry {
    char pad0[0x8];
    float x8;
    char padC[0x10];
    float x1C;
    Vec x20;
} Entry;

typedef struct List {
    char pad0[0x20];
    Entry *x20;
    int x24;
} List;

extern void func_00231A48(void *, Vec *);

void func_00157E90(void *self, List *list) {
    Entry *e;
    int count;
    Vec best;
    Vec temp;
    float tx, ty, tz, bx, by, bz, vx, vy, vz, rx, ry, rz;

    __asm__ volatile("sqc2 $vf0, %0" : "=m"(best));
    count = list->x24;
    e = list->x20;
    while (count != 0) {
        if (e->x1C < 0.0f) {
            __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                             : "=m"(temp) : "r"(&e->x20), "f"(e->x8) : "$2");
            vx = temp.x;
            tx = __builtin_fabsf(vx);
            __asm__ volatile("" : : "f"(tx));
            rx = best.x;
            bx = __builtin_fabsf(rx);
            __asm__ volatile("" : : "f"(rx));
            if (bx < tx) best.x = vx;
            vy = temp.y;
            ty = __builtin_fabsf(vy);
            __asm__ volatile("" : : "f"(ty));
            ry = best.y;
            by = __builtin_fabsf(ry);
            __asm__ volatile("" : : "f"(ry));
            if (by < ty) best.y = vy;
            vz = temp.z;
            tz = __builtin_fabsf(vz);
            __asm__ volatile("" : : "f"(tz));
            rz = best.z;
            bz = __builtin_fabsf(rz);
            __asm__ volatile("" : : "f"(rz));
            if (bz < tz) best.z = vz;
        }
        count--;
        e++;
    }
    func_00231A48((char *)self + 0x10, &best);
}
