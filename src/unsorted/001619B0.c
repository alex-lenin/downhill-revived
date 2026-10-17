/* UNIT 0x001619B0 0x00161B10 */
#include "particle.h"
typedef struct Effect { char pad0[0x2720]; int x2720; short x2724; unsigned short x2726; char pad2728[0x44]; unsigned short x276C; char pad276E[0xDA]; struct Effect **x2848; } Effect;
typedef struct Obj { char pad0[0x792C]; char *x792C; char pad7930[0x11C]; Effect *x7A4C; Effect *x7A50; } Obj;
extern char D_0026E970[], D_0026E980[], D_0027BF00[], D_0027BEC0[];
extern unsigned short D_0027D246[], D_0027D286[];
extern Effect *func_0022DD88(int);
extern void func_0022DDD8(Effect *);
void func_001619B0(Obj *obj) {
 Effect *a = obj->x7A4C; Effect *b;
 if (a && a->x276C != 0x4A) { func_0022DDD8(a); a=0; obj->x7A4C=0; }
 b=obj->x7A50;
 if (b && b->x276C != 0x4B) { func_0022DDD8(b); b=0; obj->x7A50=0; }
 if (!a) { a=func_0022DD88(func_0022CFB8((Vector4 *)(obj->x792C+0x10), obj->x792C+0x40, 0x4A, (Vector4 *)(D_0026E980), 1.0f, (Matrix4 *)(D_0027BF00), (int)(1), (int)(0))); if(a) {obj->x7A4C=a; a->x2848=&obj->x7A4C;} }
 else {a->x2726=D_0027D246[0]; a->x2720 |= 8;}
 if (!b) { b=func_0022DD88(func_0022CFB8((Vector4 *)(obj->x792C+0x10), obj->x792C+0x40, 0x4B, (Vector4 *)(D_0026E970), 1.0f, (Matrix4 *)(D_0027BEC0), (int)(1), (int)(0))); if(b) {obj->x7A50=b; b->x2848=&obj->x7A50;} }
 else {b->x2726=D_0027D286[0]; b->x2720 |= 8;}
}
