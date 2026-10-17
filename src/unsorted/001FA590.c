/* UNIT 0x001FA590 0x001FA618 */
typedef struct { char p[0x4A4]; int x4A4; } Global;
typedef struct { Global *x0; char p[0x9C]; } Obj;
extern Global D_00434208; extern void func_001F9808(Obj*,int);
void func_001FA590(Obj *o) { Obj *p=o; do { p->x0=&D_00434208; p++; D_00434208.x4A4=0; } while((int)p<(int)((char*)o+0x3C0)); func_001F9808(o,0); func_001F9808(o+1,1); func_001F9808(o+2,2); func_001F9808(o+3,3); func_001F9808(o+4,4); }
