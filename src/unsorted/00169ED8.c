/* UNIT 0x00169ED8 0x00169F08 */
struct Obj { int x0,x4; float x8; int xC; char vectors[0x40]; };
void func_00169ED8(struct Obj *p) {
 p->x0=0; p->x8=1.0f; p->xC=0;
 __asm__ volatile("sqc2 $vf0, 0x10(%0)\n\tsqc2 $vf0, 0x20(%0)\n\tsqc2 $vf0, 0x30(%0)\n\tsqc2 $vf0, 0x40(%0)" : : "r"(p) : "memory");
}
