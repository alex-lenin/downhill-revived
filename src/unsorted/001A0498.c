/* UNIT 0x001A0498 0x001A0590 */
typedef struct Mtx {
    float m[4][4];
} __attribute__((aligned(16))) Mtx;

typedef struct Path {
    char pad0[0xC];
    int index;
    int active;
    char pad14[0x70 - 0x14];
    Mtx mtx;
    char padB0[0xB0 - 0xB0];
    float points[1][4];
} Path;

extern int D_004B2828[];
extern void func_0023CFE0(float *, int);
extern void func_0023CFA0(Mtx *, int);

int func_001A0498(Path *path, float *pos, Mtx *out) {
    float v[4];

    if (path->active) {
        pos[0] = path->points[path->index][0];
        pos[1] = path->points[path->index][1];
        pos[2] = path->points[path->index][2];
        if (path->index == 0) {
            if (D_004B2828[0] == 3) {
            v[0] = pos[0];
            v[1] = pos[1];
            v[2] = pos[2] + 10.0f;
            func_0023CFE0(v, 3);
            func_0023CFA0(&path->mtx, 3);
            }
        }
        path->index++;
    }
    /* sceVu0CopyMatrix */
    __asm__ volatile (
        "lq $8, 0x0(%1)
	"
        "lq $9, 0x10(%1)
	"
        "lq $10, 0x20(%1)
	"
        "lq $11, 0x30(%1)
	"
        "sq $8, 0x0(%0)
	"
        "sq $9, 0x10(%0)
	"
        "sq $10, 0x20(%0)
	"
        "sq $11, 0x30(%0)"
        : : "r"(out), "r"(&path->mtx) : "$8", "$9", "$10", "$11", "memory");
    return path->active;
}
