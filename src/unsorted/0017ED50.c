/* UNIT 0x0017ED50 0x0017EDE8 */
#include "scalar_api.h"
typedef struct { char pad[0x7A5C]; int x7A5C; } Inner;
typedef struct Scalar0017ED50Obj { Inner *x0; int entries[1][4]; } Obj;
#if defined(__mips__)
#define RiderPairBoolResult long
#else
#define RiderPairBoolResult int
#endif
RiderPairBoolResult func_0017ED50(Obj *obj,int a,int b) {
 long result;
 int same;
 if((obj->x0->x7A5C^3)==0) return 0;
 result=0;
 if(obj->entries[a][0]==obj->entries[b][0]) {
  if(obj->entries[a][1]==obj->entries[b][1]) {
   if(obj->entries[a][2]==obj->entries[b][2]) {
    same=obj->entries[a][3]==obj->entries[b][3];
    result=same;
   }
  }
 }
 return result;
}
