/* UNIT 0x001E6F58 0x001E7160 .sdata=0x29D7D0:0x29D7E0 */
__asm__(".extern D_0029D750, 4");
extern int D_0029D750;
int D_0029D7D0 = 2;
int D_0029D7D4 = 20;
int D_0029D7D8 = 120;
int D_0029D7DC __attribute__((section(".sdata"))) = 0;
typedef struct Root { char pad0[0x7978]; int x7978; char pad797C[0xE0]; int x7A5C; } Root;
typedef struct Obj { Root *x0; int x4; int x8; int buttons[300]; int analog[300]; char pad96C[0xA0]; int xA0C; char padA10[8]; unsigned char xA18, xA19; char padA1A[2]; int xA1C; } Obj;
extern int func_002205A0(int);
extern int func_00220230(int);
extern void func_001E7160(Obj *, int);
extern void func_001E72A8(Obj *, int, int);
extern int func_001E72F0(Obj *);
extern int func_001E73B0(Obj *);
extern void func_001E7440(Obj *);
void func_001E6F58(Obj *obj)
{
    int buttons, oldIndex, axis, changed, axisChanged; int x=0,right; unsigned char on;
    obj->xA1C = 0;
    if (obj->x0->x7A5C == 1) {
        buttons = func_00220230(func_002205A0(obj->x0->x7978)) & 0xffff;
        oldIndex = obj->xA0C;
        obj->xA0C++;
        if (obj->xA0C >= 300) obj->xA0C = 0;
        obj->buttons[obj->xA0C] = buttons;
        obj->analog[obj->xA0C] = 0;
        right = x > D_0029D7D8;
        axis = 0;
        if (!right) {
            int lower = -D_0029D750;
            if (x < lower) axis = x;
            else if (lower < x) axis = x;
        }
        axis |= right & 2;
        if (buttons || axis) {
            on=1; obj->xA18 = on;
            changed = obj->buttons[oldIndex] ^ buttons;
            axisChanged = obj->analog[oldIndex] ^ axis;
            if (changed || axisChanged) {
                if ((changed & buttons) || axisChanged) {
                    if (buttons || obj) axisChanged=9;
                    obj->x4 = 0;
                    if (!obj->xA19) {
                        obj->xA19 = on;
                        obj->x8 = 0;
                    }
                } else {
                    obj->x4 = 0;
                    if (obj->xA19) func_001E7160(obj, 0);
                }
                func_001E72A8(obj, changed, buttons);
            } else {
                if (obj->xA19) obj->x8++;
                obj->x4++;
            }
        } else {
            if (obj->xA19) obj->x8++;
            obj->x4++;
        }
        if (obj->x8 > D_0029D7D0) func_001E7160(obj, 1);
        if (obj->xA18) {
            if (obj->x4 > D_0029D7D4) obj->xA1C = func_001E72F0(obj);
            if (obj->xA18 && !obj->xA1C) obj->xA1C = func_001E73B0(obj);
        }
        if (obj->xA1C) func_001E7440(obj);
        else if (obj->x4 > D_0029D7D4) func_001E7440(obj);
    }
}
