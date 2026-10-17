/* UNIT 0x001279D8 0x00127BB8 */
#include "rider_queries.h"
#if defined(__mips__)
#define CallShared0011B268_0 func_0011B268
#else
#define CallShared0011B268_0 Forward0011B268_2
#endif
typedef struct Physics { char pad0[0xD4]; float xD4; char padD8[0x60]; int x138,x13C; } Physics;
typedef struct Object Object;
typedef struct VTable { char pad0[0x20]; short delta; short pad22; char *(*get)(Object *); } VTable;
struct Object { VTable *vt; char pad4[0x22C]; Physics *x230; };
typedef struct World { char pad0[0x21420]; int count; char pad21424[0x10]; unsigned char toggle; char pad21435[3]; int counts[100]; char pad215C8[0x2580]; int order[100]; } World;
extern int func_0011B268(void *, void *, float *, void *, float, float);
extern void func_0011C208(void *, void *, int *, int *, float *);
extern void func_00127440(Physics *, int, void *, float);
extern Object *func_00193498(int);
extern float D_0029C224[];
extern char D_002AF2C0[];
void func_001279D8(World *world) {
    float vector[4] __attribute__((aligned(16)));
    struct { int x,y; float depth,height; } local; float limit;
    int i,n,id,*ids; unsigned int toggle; Object *obj;
    i=0; toggle=world->toggle;
    if(world->count>0) do {
        id=world->order[i];
        n=world->counts[id];
        ids=(int *)((char *)world+0x215C8+id*0x60);
        if(n!=0) {
            limit=D_0029C224[0];
            do {
                obj=func_00193498(*ids);
                local.x=0; local.y=0; local.height=limit;
                __asm__ volatile("sqc2 $vf0, 0x0(%0)" : : "r"(vector) : "memory");
                if(toggle!=0) {
                    func_0011C208(D_002AF2C0,obj->vt->get((Object *)((char *)obj+obj->vt->delta))+0x10,&local.x,&local.y,&local.depth);
                    { Physics *p=obj->x230; int a=local.x, b=local.y; float d=local.depth; p->xD4=d; p->x13C=b; p->x138=a; }
                } else if(obj->x230->xD4<20000.0f) {
                    int result=CallShared0011B268_0(D_002AF2C0,obj->vt->get((Object *)((char *)obj+obj->vt->delta))+0x10,&local.height,vector,100.0f,-50.0f);
                    func_00127440(obj->x230,result,vector,local.height);
                }
                toggle^=1; --n; ++ids;
            } while(n!=0);
        }
        ++i;
    } while(i<world->count);
    world->toggle^=1;
}
