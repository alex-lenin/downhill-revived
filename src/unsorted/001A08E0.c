/* UNIT 0x001A08E0 0x001A0958 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct Obj { char pad[0x518]; signed char x518; } Obj;
extern char D_004B2A90[];
extern void func_001C57A8(void *,u128);
void func_001A08E0(Obj *o) {
 if (o->x518==4) o->x518-=2; else o->x518--;
 if (o->x518<=0) o->x518=5;
 func_001C57A8(D_004B2A90, ((u128)0x5355<<64) | (u128)0x4f49564552505f53UL);
}
