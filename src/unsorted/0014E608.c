/* UNIT 0x0014E608 0x0014E6A0 */
typedef struct { char pad[0x12114]; unsigned int flags; } Flags;
typedef struct {
    char pad[0xA0C];
    char enabled[23];
    char gap;
    int xA24, xA28, xA2C, xA30, xA34, xA38;
} State;
typedef struct { char pad[12]; int xC; } Object;
extern Flags D_004225A0;
extern State D_00380D00;
extern int D_004B2840[];
extern void func_0014E6A0(Object *);

void func_0014E608(Object *obj)
{
    int i;
    int index;

    index = D_004B2840[0];
    D_004225A0.flags = D_004225A0.flags | (1 << index);
    if (obj->xC) {
        /* gcc reverses this count-up loop into the retail count-down form */
        for (i = 0; i < 23; i++) {
            D_00380D00.enabled[i] = 0;
        }
        D_00380D00.xA24 = 0;
        D_00380D00.xA28 = 0;
        D_00380D00.xA2C = 0;
        D_00380D00.xA34 = 0;
        D_00380D00.xA38 = 0;
        func_0014E6A0(obj);
    }
}
