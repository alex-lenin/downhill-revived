/* UNIT 0x00133D68 0x00133DE0 */
typedef struct Obj { char pad[0x110]; float x110; char pad114[8]; float x11C,x120; } Obj;
extern void func_00231C10(void *);
extern void func_002318F8(void *,float);
extern void func_00231930(void *,float);
extern void func_00231C20(void *,void *);
void func_00133D68(Obj *obj) {
 float vector[4] __attribute__((aligned(16)));
 void *matrix=(char *)obj+0x10;
 func_00231C10(matrix);
 func_002318F8(matrix,obj->x120);
 func_00231930(matrix,obj->x11C);
 __asm__ volatile("sqc2 $vf0,0(%0)" : : "r"(vector) : "memory");
 vector[2]=obj->x110*-96.600006f;
 func_00231C20(matrix,vector);
}
