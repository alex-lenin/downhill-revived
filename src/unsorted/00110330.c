/* UNIT 0x00110330 0x001103B0 */
typedef struct Owner { char pad[0x792c]; char *x792c; } Owner;
typedef struct Sub { char pad[0x14]; unsigned short x14; } Sub;
typedef struct Obj { Owner *x0; char pad4[0x70]; Sub *x74; char pad78[0x210]; float x288; } Obj;
extern void func_001122D8(Obj *, void *);
extern float func_00138580(void *);
extern void func_001103B0(Obj *, float);
void func_00110330(Obj *obj) {
 float value;
 func_001122D8(obj,obj->x0->x792c+0x10);
 value=func_00138580((char *)obj->x0+0x40c0)*obj->x288;
 if(value<5.0f) value=5.0f;
 if(!((obj->x74->x14>>5)&1)) func_001103B0(obj,value);
}
