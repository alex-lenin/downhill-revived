/* UNIT 0x001A1168 0x001A1258 */
typedef struct Vec4 {
    float v[4];
} Vec4 __attribute__((aligned(16)));

typedef struct Mtx {
    Vec4 row[4];
} Mtx;

typedef struct Obj {
    char pad0[0x4];
    char x4;
    char pad5;
    char x6;
    char pad7[0x10 - 0x7];
    Vec4 pos;
    char pad20[0x40 - 0x20];
    Mtx mtx;
} Obj;

typedef struct Global {
    char pad0[0x20];
    int x20;
} Global;

extern Global D_003F52D0;
extern Mtx *func_0023CF58(int);
extern Vec4 *func_0023CF78(int);

void func_001A1168(void *unused, Obj *obj, Vec4 *offset)
{
    Vec4 t;
    Mtx *mtx;
    int bit;

    if (obj != 0) {
        mtx = &obj->mtx;
        /* sceVu0CopyMatrix */
        __asm__ volatile ("lq $8, 0x0(%4)
	"
                          "lq $9, 0x10(%4)
	"
                          "lq $10, 0x20(%4)
	"
                          "lq $11, 0x30(%4)
	"
                          "sq $8, %0
	"
                          "sq $9, %1
	"
                          "sq $10, %2
	"
                          "sq $11, %3"
                          : "=m"(mtx->row[0]), "=m"(mtx->row[1]), "=m"(mtx->row[2]), "=m"(mtx->row[3])
                          : "r"(func_0023CF58(D_003F52D0.x20)) : "$8", "$9", "$10", "$11");
        __asm__ volatile ("lq $2, 0(%1)
	sq $2, %0"
                          : "=m"(obj->pos) : "r"(func_0023CF78(D_003F52D0.x20)) : "$2");
        /* t = mtx (3x3) * offset */
        __asm__ volatile ("lqc2 $vf11, 0(%1)\n\t"
                          "lqc2 $vf12, 0(%2)\n\t"
                          "lqc2 $vf13, 0x10(%2)\n\t"
                          "lqc2 $vf14, 0x20(%2)\n\t"
                          "vmulax.xyz ACC, $vf12, $vf11x\n\t"
                          "vmadday.xyz ACC, $vf13, $vf11y\n\t"
                          "vmaddz.xyz $vf11, $vf14, $vf11z\n\t"
                          "sqc2 $vf11, %0"
                          : "=m"(t) : "r"(offset), "r"(mtx));
        obj->pos.v[0] += t.v[0];
        obj->pos.v[1] += t.v[1];
        obj->pos.v[2] += t.v[2];
        bit = 1;
        obj->x6 = bit << D_003F52D0.x20;
        obj->x4 = bit;
    }
}
