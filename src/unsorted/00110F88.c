/* UNIT 0x00110F88 0x00111080 */
typedef struct Part { char pad0[0xC4]; int xC4; char padC8[0x4]; int xCC; } Part;
typedef struct Obj {
    char *x0;
    char pad4[0x2D0];
    Part x2D4;
    char pad3A4[0x88];
    int x42C;
    char pad430[0x10];
    unsigned long x440;
} Obj;

extern void func_001E6E68(char *, int);
extern float func_001E7C38(char *, int);
extern int func_001E7BA0(char *, int);
extern int func_00222BD8(int min, int max);

void func_00110F88(Obj *obj, int arg)
{
    Part *part = &obj->x2D4;
    float time;
    int chance;

    func_001E6E68(obj->x0 + 0x6610, 1);
    time = func_001E7C38(obj->x0 + 0x5FF0, arg);
    obj->x440 &= ~((unsigned long)1 << 62);
    obj->x42C = (int)(time * 60.0f) - 7;
    if ((part->xCC >> 5) & 1) {
        if (func_001E7BA0(obj->x0 + 0x5FF0, arg)) {
            chance = part->xC4;
            if (func_00222BD8(0, 100) < chance) {
                obj->x440 |= (unsigned long)0x8000 << 47;
            }
        }
    }
}
