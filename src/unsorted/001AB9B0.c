/* UNIT 0x001AB9B0 0x001ABA48 */
typedef struct Obj { char pad[0x16B0]; int x16B0; int x16B4; float x16B8; float x16BC; float x16C0; float x16C4; float x16C8; int x16CC; int x16D0; int x16D4; } Obj;
extern int func_0023CF58(int);
extern int func_0023CF78(int);
extern void func_001ABB48(Obj *);
void func_001AB9B0(Obj *obj,int value) {
 obj->x16D0 = value;
 obj->x16CC = 0;
 obj->x16B0 = func_0023CF58(value);
 obj->x16B4 = func_0023CF78(value);
 obj->x16D4 = 0;
 obj->x16B8 = 1000000.0f;
 obj->x16BC = 1.0f;
 obj->x16C0 = 0.5f;
 obj->x16C4 = 0.5f;
 obj->x16C8 = 0.0008f;
 func_001ABB48(obj);
}
