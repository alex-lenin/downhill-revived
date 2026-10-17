/* UNIT 0x001132B0 0x00113330 */
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

struct Body { char pad[0x10]; Vec4 pos; };
struct Rider { char pad[0x792C]; struct Body *x792C; };
struct Global { char pad[0x50140]; struct Rider *riders[26]; int count; };
struct Obj { struct Rider *x0; char pad4[0x14C]; Vec4 offsets[1]; };

extern struct Global D_002AF2C0;
extern int D_004B2828[];

void func_001132B0(struct Obj *obj)
{
    struct Global *global;
    struct Rider **riders;
    struct Rider *rider;
    Vec4 *out;
    int count;
    int i;

    global = &D_002AF2C0;
    count = global->count - D_004B2828[0];
    if (count > 0) {
        i = count;
        out = obj->offsets;
        riders = global->riders;
        do {
            rider = *riders;
            if (rider != obj->x0) {
                __asm__ volatile ("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0(%2)"
                                  : : "r"(&rider->x792C->pos), "r"(&obj->x0->x792C->pos), "r"(out) : "memory");
            }
            out++;
            i--;
            riders++;
        } while (i != 0);
    }
}
