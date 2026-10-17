/* UNIT 0x001BF218 0x001BF758 .sdata=0x29D570:0x29D578 */
#include "event_queue.h"
int D_0029D570=15;
int D_0029D574=120;
class Object { public: virtual void a(); virtual void b(); virtual void c(); virtual char *get(); };
class RiderObject : public Object { public: unsigned int **x4; };

struct Obj { int x0,x4,x8,xC,x10; };
struct Rider { char pad[0x7720]; char x7720[0x20C]; char *x792C; char pad2[0x796C-0x7930]; int x796C; };
struct Riders { char pad[0x50130]; Rider *x50130[1]; Rider **Get() {return x50130;} Rider *At(int i) {return x50130[i];} };
struct Game { char pad[0x288]; int x288; };
extern Game D_004B25A0;
extern Riders D_002AF2C0;
struct Manager { char pad[0x5350]; char x5350[4]; char x5354[4]; char *Get() {return x5350;} };
extern Manager D_00398C40;
extern void func_001DB6B0(void *,int,int,int *,int *,int);
extern float func_00222330(void *,void *);
extern int func_001BC6A0(void *,int,int);
extern void func_001BC868(void *,int,int,int,int,int,int,void *,int,int,int);
extern int func_001BC480(void *,int);
extern int func_001C0180(void *,unsigned int);
extern int func_001BFFF0(void *,int);
extern int func_001C0958(void *);
extern void func_001BBF30(void *,int,void *);
struct Params {int a,b,c; void Send(void *p,int i) {func_00233BA8((EventQueue *)(p), i, 19, (int *)(this), 12, -1);} };
extern void func_001C78D0(void *);
void func_001BF218(void *unused,Object *object) {
    int found=0;
    int other;
    int best=0;
    float distance=32767.0f;
    int i;
    func_001DB6B0(object->get()+16,D_0029D570,D_0029D574,&found,&other,0);
    if (found) {
        for (i=0;i<D_004B25A0.x288;i++) {
            void *pos=D_002AF2C0.x50130[i]->x792C+16;
            float value=func_00222330(pos,object->get()+16);
            if (value<distance) { distance=value; best=i; }
        }
        func_001C78D0(D_002AF2C0.x50130[best]->x7720);
    }
}

int func_001BF368(Obj *pair, Object *object)
{
    int params[3];
    int found = 0;
    int other;
    int index;
    Manager *state;
    char *base;

    func_001DB6B0(object->get() + 16, 60, 120, &found, &other, 0);
    if (found != 0x400) { pair->x10 = 1; return -1; }
    if (!pair->x10) return -1;
    state = &D_00398C40;
    base = state->x5350;
    index = func_001BC6A0(base, pair->x4, pair->x8);
    if (index != 5) {
        func_001BC868(base, index, 60, 120, pair->x4, 0, 1,
                      object->get() + 16, 0, 0, 2);
        pair->x8 = func_001BC480(base, index);
        params[0] = func_001C0958(base);
        params[1] = 0;
        params[2] = 1;
        func_00233BA8((EventQueue *)(state->x5354), index, 18, (int *)(params), 12, -1);
        func_001BBF30(state->x5354, index, base + (index * 132 + 0x614));
        pair->x10 = 0;
    }
    return index;
}

int func_001BF4E8(Obj *obj,RiderObject *object) {
 int found=0,other; int best=0; float distance=32767.0f; int i; int result,kind,rider; Params data; char *manager; Manager *state;
 func_001DB6B0(object->get()+16,D_0029D570,D_0029D574,&found,&other,0);
 if(!found) return -1;
 manager=D_00398C40.Get();
 result=func_001BC6A0(manager,obj->x4,obj->xC);
 if(result!=5) {
 func_001BC868(manager,result,D_0029D570,D_0029D574,obj->x4,0,1,object->get()+16,0,0,2);
 obj->xC=func_001BC480(manager,result);
 kind=func_001C0180(manager,**object->x4>>18);
 for(i=0;i<D_004B25A0.x288;i++) {
 void *pos=D_002AF2C0.Get()[i]->x792C+16;
 float value=func_00222330(pos,object->get()+16);
 if(value<distance) {distance=value;best=i;}
 }
 state=&D_00398C40; char *base=state->x5350;
 rider=func_001BFFF0(base,D_002AF2C0.At(best)->x796C);
 data.a=func_001C0958(base);data.b=kind;data.c=rider;
 data.Send(state->x5354,result);
 func_001BBF30(state->x5354,result,base+0x614+result*0x84);
 }
 return result;
}
