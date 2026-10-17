/* UNIT 0x0013EFF8 0x0013F098 .sdata=0x29C4E8:0x29C4F0 */
#include "ui_style.h"
typedef struct Pair { int x[2]; } Pair;
Pair D_0029C4E8={0,0};
typedef struct Obj { int x0; char pad0[0x14]; char x18[0x400]; int x418; char pad1[0x60]; int x47c; } Obj;
typedef struct State { char pad[0x138]; int x138; } State;
typedef struct World { char pad[0x20]; int x20; } World;
extern Obj D_0036B0E0;
extern State *D_0029DD30;
extern World D_003F52D0;
extern void func_0018D8A8(void *,Pair *);
extern void func_0020F9F0(int,int,int,int,int);
void func_0013EFF8(Obj *obj) {
    int count;
    int total;
    World *world;
    char *entry;
    obj->x0=-1;
    total=obj->x418;
    for(count=0; count<total; count++) {
        func_0018D8A8(obj->x18+count*16,&D_0029C4E8);
    }
    obj->x47c=0;
    func_0018D800((const UiStyle *)(void *)(&D_0036B0E0));
    world=&D_003F52D0;
    func_0020F9F0(0,world->x20,110,25,D_0029DD30->x138);
}
