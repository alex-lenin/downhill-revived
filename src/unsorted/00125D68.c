/* UNIT 0x00125D68 0x00125E38 */
typedef struct Pair { int x[2]; } Pair;
typedef struct Link { unsigned int *x0; } Link;
typedef struct Actor { char pad[4]; Link *x4; } Actor;
typedef struct Obj { char pad0[8]; int x8; char pad1[0x34]; int x40,x44,x48,x4c,x50,x54; char pad2[0x74]; float xcc; char pad3[0x60]; Actor *x130; int x134; char pad4[0x14]; Pair x14c,x154; } Obj;
extern Pair D_0029C1F8[], D_0029C200[];
extern float func_00222B68(float,float);
void func_00125D68(Obj *obj) {
    obj->x14c=D_0029C1F8[0];
    obj->x154=D_0029C200[0];
    obj->x8=1;
    obj->x40=1;
    obj->x44=1;
    obj->x48=3;
    obj->x4c=2;
    obj->x50=3;
    obj->x54=1;
    if((*obj->x130->x4->x0 >>18)==0x1018) {
        obj->x48=1;
        obj->x4c=1;
        obj->x50=1;
    }
    obj->xcc=func_00222B68(-0.34906584f,0.34906584f);
    obj->x134=0;
}
