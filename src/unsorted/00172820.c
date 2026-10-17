/* UNIT 0x00172820 0x001728A8 */
typedef struct Obj { int x0,x4,x8,xC,x10,x14,x18,x1C; char x20[32]; int x40,x44,x48,x4C; char x50[96]; int xB0,xB4,xB8,xBC,xC0; char pad[0x5C]; char x120; } Obj;
extern void func_00254204(void *, int, int);
void func_00172820(Obj *o) {
o->x4=0;o->x8=0;o->xC=0;o->x10=0;o->x14=0;o->x18=0;
func_00254204(o->x20,0,32);
o->x40=0;o->x44=0;o->x48=0;o->x4C=0;
__asm__ volatile ("sqc2 $vf0, 0x50(%0)" : : "r"(o) : "memory");
__asm__ volatile ("sqc2 $vf0, 0x60(%0)" : : "r"(o) : "memory");
__asm__ volatile ("sqc2 $vf0, 0x70(%0)" : : "r"(o) : "memory");
__asm__ volatile ("sqc2 $vf0, 0x80(%0)" : : "r"(o) : "memory");
__asm__ volatile ("sqc2 $vf0, 0x90(%0)" : : "r"(o) : "memory");
__asm__ volatile ("sqc2 $vf0, 0xa0(%0)" : : "r"(o) : "memory");
o->xB0=0;o->xB4=0;o->xB8=0;o->xBC=0;o->xC0=0;o->x120=0;
}
