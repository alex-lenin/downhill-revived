/* UNIT 0x001E7C78 0x001E7DA0 */
/* The retail function is entered two words before its compiled body:
   an `addu $sp,$sp,0x20' / `nop' frame-header entry emitted by the original
   SN build (its code pointer is taken as the +8 entry; see the pointer at
   0x002883F4 -> 0x001E7C80).  cc1 will not emit that entry from C, so it is
   provided here; the C body is emitted immediately after it.  */
__asm__("\t.text\n\t.globl\tfunc_001E7C78\n\t.ent\tfunc_001E7C78\nfunc_001E7C78:\n\taddu\t$sp,$sp,32\n\tnop\n\t.end\tfunc_001E7C78");

typedef struct Sub {
    char pad0[0x10E4];
    unsigned char x10E4;
} Sub;

typedef struct Game {
    char pad0[0x40C0];
    char x40C0[0x4ECC - 0x40C0];
    void *x4ECC;
    char pad4ED0[0x6610 - 0x4ED0];
    Sub x6610;
    char pad[0x78D4 - 0x6610 - sizeof(Sub)];
    int x78D4;
} Game;

typedef struct Obj {
    Game *game;
    char pad4[0x10 - 4];
    unsigned char x10;
    char pad11[0x28 - 0x11];
    int x28;
    int x2C;
} Obj;

extern int func_001E5CC8(Sub *, int);
extern int func_001E5D90(Sub *, int);
extern int func_001E5900(Sub *);
extern int func_001E68C0(Sub *);
extern int func_001E6E28(Sub *);
extern int func_00138560(void *);
extern void func_001E5990(Sub *, int);

int func_001E7C78_body(Obj *obj)
{
    Game *game = obj->game;
    Sub *sub = &game->x6610;
    int yes = 1;
    int result;

    if (game->x78D4) {
        if (func_001E5D90(sub, obj->x28)) {
            obj->x10 = yes;
        }
    } else {
        if (!func_001E5CC8(sub, obj->x28) || func_001E5D90(sub, obj->x28)) {
            obj->x10 = yes;
        }
        if (!obj->x10) {
            return 0;
        }
    }
    if (func_001E5900(sub)) {
        sub->x10E4 = 1;
        if (!func_001E6E28(sub) || !(unsigned char)func_00138560(obj->game->x40C0)) {
            func_001E5990(sub, 0);
            return 0;
        }
        if (game->x4ECC == (char *)game + 0x5FF0) {
            return 0;
        }
        result = func_001E68C0(sub);
        obj->x2C = result;
        return result ? yes : 0;
    }
    return 0;
}
