/* UNIT 0x001AFC90 0x001AFDE8 */
struct Obj { int x0,x4,x8; char padC[0xE4]; float xF0; unsigned char xF4; };
extern int func_00238F48();
extern void func_001ACAE0(void*,float);
extern void func_001ACB70(void*,float);
extern void func_001AC810(void*,float);
extern float func_001ACBA0(void*);
void func_001AFC90(Obj *obj) {
 if(obj->x8 && obj->xF4 && obj->x4>=0) {
 int elapsed=func_00238F48()-obj->x4;
 obj->x4=-1; obj->x0=0;
 if(!((float)elapsed<15.0f)) {
 char *a=(char*)obj+12,*b=(char*)obj+128;
 obj->xF0=(((float)elapsed-15.0f)*0.011111111f >? 0.4f) <? 0.9f;
 func_001ACAE0(a,0.0f); func_001ACB70(a,2.0f);
 *(float*)(a+68)=0.0f; func_001AC810(a,0.0f);
 if(func_001ACBA0(b)>=1.0f) func_001ACAE0(b,0.0f);
 func_001ACB70(b,2.0f); *(float*)(b+68)=obj->xF0;
 }
 }
}
