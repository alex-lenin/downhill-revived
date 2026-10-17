/* UNIT 0x0019BA40 0x0019BAF8 */
typedef struct State State;
typedef struct Obj Obj;

extern struct Global {
    char pad[0x24];
    signed char x24[40];
} D_0043DCC0;

extern State D_0043CD50;
extern Obj D_0043D2A0;
extern char D_003F52D0[];

extern int func_0019B0B8(State *state, int value);
extern void func_0019B6A0(Obj *obj, int value);
extern void func_00190688(char *name);

void func_0019BA40(void *unused, int target)
{
    struct Global *g = &D_0043DCC0;
    signed char *flags;
    int i;
    int count;
    int found;

    i = 0;
    count = 0;
    flags = g->x24;
    found = 0;
    for (; i < 40; i++, flags++) {
        if (!func_0019B0B8(&D_0043CD50, i) && *flags) {
            if (count == target) {
                found = i;
            }
            count++;
        }
    }
    func_0019B6A0(&D_0043D2A0, found);
    func_00190688(D_003F52D0);
}
