/* UNIT 0x00112D88 0x00112E80 */
typedef struct Game { char pad0[0x798C]; int x798C; } Game;
typedef struct Obj { char pad0[0x8]; Game *x8; } Obj;

extern unsigned char D_0029EA20[];
extern float func_001385A8(void *);
extern int func_00111A98(Obj *);
extern void func_00112E80(Obj *);
extern void func_00113098(Obj *);
extern int abs(int);

void func_00112D88(Obj *obj)
{
    if (obj->x8 != 0 && func_001385A8((char *)obj->x8 + 0x40C0) > 0.01f) {
        if (D_0029EA20[0x1089F] != 0) {
            int value = obj->x8->x798C;
            if (value < func_00111A98(obj) || func_00111A98(obj) < 4) {
                func_00112E80(obj);
            } else {
                func_00113098(obj);
            }
        } else {
            int value = obj->x8->x798C;
            if (abs(value - func_00111A98(obj)) < 4) {
                func_00113098(obj);
            } else {
                func_00112E80(obj);
            }
        }
    } else {
        func_00112E80(obj);
    }
}
