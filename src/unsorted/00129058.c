/* UNIT 0x00129058 0x00129110 */
#include "animation_track_search.h"
#if defined(__mips__)
#define CallShared00163C58_0 func_00163C58
#else
#define CallShared00163C58_0 Forward00163C58_0
#endif
typedef struct Obj { char pad[0x31C8]; float x31C8; char pad31CC[0x3250-0x31CC]; int x3250,x3254; } Obj;
typedef struct Globals { char pad[0x50198]; float x50198; } Globals;
typedef struct Result { char pad[8]; float x8; } Result;
extern Globals D_002AF2C0;
extern char D_003041B0[];
extern int func_00163C58(void *,float,int,int);
extern Result *func_00163D40(void *,int);
Result *func_00129058(Obj *obj,int id) {
 Result *result=0;
 float minimum, position; int index;
 float limit=D_002AF2C0.x50198-200.0f;
 __asm__ volatile("min.s %0,%1,%2" : "=&f"(minimum) : "f"(obj->x31C8), "f"(limit));
 obj->x31C8=minimum;
 if(obj->x3254!=id) {
  obj->x31C8=limit; obj->x3250=-1; obj->x3254=id;
 }
 position=obj->x31C8;
 index=CallShared00163C58_0(D_003041B0,position,obj->x3250,id);
 obj->x3250=index;
 if(index>=0) { result=func_00163D40(D_003041B0,index); obj->x31C8=result->x8; }
 return result;
}
