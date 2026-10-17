/* UNIT 0x00170740 0x001707C8 */
typedef struct { char pad[0x48B0]; int x48B0; } Inner;
typedef struct { Inner *x0; char pad[0x20]; float x24; } Obj;
extern void func_001ACFA0(void *,int);
extern void func_00136030(void *,int);
extern void func_001AD580(void *,int,float,float);
void func_00170740(Obj *obj) {
 Inner *inner=obj->x0; void *part=(char *)inner+0x4580;
 func_001ACFA0(part,1); inner->x48B0=1;
 func_00136030((char *)inner+0x40C0,4);
 func_001AD580(part,0,0.25f,0.6f); obj->x24=4.0f;
}
