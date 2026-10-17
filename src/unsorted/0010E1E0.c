/* UNIT 0x0010E1E0 0x0010E270 */
typedef struct Obj { char *x0; int x4; char *x8; char padC[0x2A0]; float x2AC; } Obj;
extern int func_00138560(char *);
extern float func_001385A8(char *);
extern void func_0010E270(Obj *);
void func_0010E1E0(Obj *obj) {
    if (!(func_00138560(obj->x0 + 0x40C0) & 0xFF)) {
        if (obj->x8 && func_001385A8(obj->x8+0x40C0) > 0.1f) {
            func_0010E270(obj);
            return;
        }
    }
    obj->x2AC = 1.0f;
}
