/* UNIT 0x00111368 0x00111510 */
struct Other { char pad[0x7a78]; float x7a78; };
struct Part { char pad[0x14]; unsigned short flags; };
struct Obj { char *x0; int x4; struct Other *x8; char padC[0x68]; struct Part *x74,*x78; char pad7C[0x308]; int x384; char pad388[0xa8]; int x430; char pad434[0xc]; union { long flags; int low; } x440; };
extern int D_004B26AC[];
extern float func_001385A8(void *);
extern void func_00112D88(struct Obj *);
void func_00111368(struct Obj *p) {
 int count = p->x430;
 if (count < p->x384 || (count < 300 && p->x8 && p->x8->x7a78 == 0.0f && count < p->x440.low * 6)) {
  p->x440.flags &= ~(1L << 34);
 } else if (((p->x74->flags >> 3) & 1) || ((p->x78->flags >> 3) & 1) || D_004B26AC[0] == 10) {
  p->x440.flags |= 1L << 34;
 } else if (p->x430 > 300 && func_001385A8(p->x0 + 0x40c0) < 0.17f) {
  p->x440.flags = (p->x440.flags & ~(1L << 38)) | (1L << 34);
 } else if (!(p->x440.flags & (1L << 40))) {
  func_00112D88(p);
 }
 if (p->x440.flags & (1L << 34)) p->x440.flags &= ~(1L << 38);
}
