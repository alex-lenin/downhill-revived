/* UNIT 0x00198680 0x00198780 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct State { char pad0[0x2A0]; int x2A0; } State;
extern char D_004348F0[];
extern State D_004B25A0;
extern char D_004225A0[];
extern const char D_0028DA68[];
extern void func_001962F0(void *);
extern void func_00195B08(void *, int, Vec *, unsigned char *);
extern int func_001961D8(void *, int, Vec *);
extern void func_00254D08(const char *);

static inline char *get_player(int p) {
    return D_004225A0 + p * 0x420;
}

static inline int get_offset(int p) {
    return p * 0x420 + 0x9240;
}

void func_00198680(void) {
    int i = 0;
    int count;
    int offset;
    Vec pos;
    unsigned char type;

    func_001962F0(D_004348F0);
    offset = 0x9240;
    count = *(int *)(get_player(D_004B25A0.x2A0) + offset);
    for (; i < count; i++) {
        func_00195B08(D_004225A0 + get_offset(D_004B25A0.x2A0), i, &pos, &type);
        if (func_001961D8(D_004348F0, type, &pos) < 0) {
            func_00254D08(D_0028DA68);
        }
    }
}
