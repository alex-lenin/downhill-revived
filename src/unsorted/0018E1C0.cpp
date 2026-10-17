/* UNIT 0x0018E1C0 0x0018E398 */
typedef unsigned int u128 __attribute__((mode(TI)));

class Base {
public:
    int cursor;
    char pad4[0x45C - 0x4];
    int x45C;
    char pad460[0x490 - 0x460];
    u128 sound;
    char pad4A0[0x4A8 - 0x4A0];
    int x4A8;
    char pad4AC[0x4B0 - 0x4AC];
    int x4B0;
    char pad4B4[0x4F8 - 0x4B4];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6(int index);
    virtual void v7();
    virtual void v8(int index);
    virtual void v9(int index);
};

class A : public Base {
public:
    void func_0018E1C0();
};

typedef struct Global {
    char pad0[0x64];
    int x64;
} Global;

extern Global D_003F52D0;
extern char D_004B2A90[];

extern "C" {
int func_00190D60(void *p, int button, int mask);
void func_001C57A8(void *p, u128 name);
void func_001904B8(Global *g, int value);
}

static inline void PlaySound(Base *obj)
{
    int *words = (int *)&obj->sound;
    int empty = 1;
    int i;

    for (i = 0; i < 4; i++) {
        if (words[i] != 0) {
            empty = 0;
        }
    }
    if (!empty) {
        func_001C57A8(D_004B2A90, obj->sound);
    }
}

void A::func_0018E1C0()
{
    Global *g = &D_003F52D0;
    int *pad = &g->x64;

    if (func_00190D60(pad, 4, x45C)) {
        sound = (u128)0x5443454C45535F53UL;
        v6(cursor);
        PlaySound(this);
        if (x4B0 != 0) {
            func_001904B8(&D_003F52D0, x4B0);
        }
    }
    if (x4A8 >= 0 && func_00190D60(pad, x4A8, x45C)) {
        sound = (u128)0x4B4341425F53UL;
        v7();
        PlaySound(this);
    }
    if (func_00190D60(pad, 2, x45C)) {
        v8(cursor);
    }
    if (func_00190D60(pad, 3, x45C)) {
        v9(cursor);
    }
}
