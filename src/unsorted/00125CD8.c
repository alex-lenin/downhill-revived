/* UNIT 0x00125CD8 0x00125D58 */
typedef struct Vec { float x,y,z,w; } Vec;
typedef struct Obj { char pad[8]; int x8; char padc[0x34]; int x40[60]; Vec *x130; } Obj;
extern void func_001257B8(Obj *,float);
extern void func_00203E10(Vec *,float);
void func_00125CD8(Obj *obj) {
 Vec value; Vec *source;
 func_001257B8(obj,29.3333339691162109375f);
 source=&obj->x130[obj->x40[obj->x8]+1];
 value=*source;
 func_00203E10(&value,1.0f);
}
