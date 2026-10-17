/* UNIT 0x00199118 0x001991B0 */
typedef struct Obj { char pad[0x2030]; int x2030; int x2034; float x2038; float x203C; float x2040; float x2044; float x2048; int x204C; int x2050; int x2054; } Obj;
extern int func_0023CF58(int);
extern int func_0023CF78(int);
extern void func_00199298(Obj *);
void func_00199118(Obj *obj,int value) {
 obj->x2050 = value;
 obj->x204C = 0;
 obj->x2030 = func_0023CF58(value);
 obj->x2034 = func_0023CF78(value);
 obj->x2054 = 0;
 obj->x2038 = 1000000.0f;
 obj->x203C = 1.0f;
 obj->x2040 = 0.5f;
 obj->x2044 = 0.5f;
 obj->x2048 = 0.0008f;
 func_00199298(obj);
}
