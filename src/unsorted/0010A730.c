/* UNIT 0x0010A730 0x0010A7B0 */
typedef struct Obj { int x0; char pad4[0x280]; float x284; char pad288[0x1b8]; unsigned long x440; } Obj;
extern void func_0010A7B0(Obj *);
extern void func_0010AFB0(Obj *, int);
extern void func_00113810(Obj *);
void func_0010A730(Obj *obj, int value) {
 func_0010A7B0(obj);
 obj->x0=value;
 obj->x440 |= ((unsigned long)0x8000 << 19);
 obj->x440 |= ((unsigned long)0x8000 << 20);
 obj->x284=50.0f;
 func_0010AFB0(obj,1);
 if(obj->x0) func_00113810(obj);
}
