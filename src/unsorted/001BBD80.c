/* UNIT 0x001BBD80 0x001BBEE8 */
typedef struct Vec { float v[4]; } Vec;
typedef struct Rider { Vec vectors[63]; Vec x3F0; char pad400[0x1A40]; char x1E40[0x404]; Vec x2244; } Rider;
typedef struct Obj { Rider *x0; int x4; int x8; int xC; float x10,x14; } Obj;
extern void func_001BBD50(Obj*),func_00203E10(Vec*,float),func_00202E38(Vec*,int,int,float);
void func_001BBD80(Obj *obj) {
 Vec v; Vec *source; Rider *rider; float *component;
 rider=obj->x0;func_001BBD50(obj);component=(float*)rider->x1E40;
 if(obj->x8==63) {v=rider->x2244;func_00203E10(&v,obj->x14);} else {source=(Vec*)((long)component+(obj->x8*16+20)); v=*source;func_00203E10(&v,obj->x14);}
 func_00202E38((Vec*)((long)component+(obj->x8*16+20)),1,1,obj->x10);
 if(obj->x8==63) {v=rider->x3F0;func_00203E10(&v,obj->x14);} else {v=rider->vectors[obj->x8];func_00203E10(&v,obj->x14);}
 func_00202E38(&rider->vectors[obj->x8],1,1,obj->x10);
}
