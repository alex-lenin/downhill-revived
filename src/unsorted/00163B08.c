/* UNIT 0x00163B08 0x00163B68 */
struct Vector { float x,y,z; };
struct Inner { struct Vector x0; char padC[10]; short x16,x18; char pad1A[6]; struct Vector x20; char pad2C[10]; short x36,x38; };
struct Obj { struct Inner *x0; char pad4[0x58]; int x5C; char pad60[8]; int x68; };
void func_00163B08(struct Obj *p) {
 struct Inner *inner=p->x0;
 struct Vector *v;
 p->x5C=2; p->x68=1;
 inner->x0.x=0.0f; inner->x0.y=0.0f; inner->x0.z=100.0f;
 p->x0->x16=1; p->x0->x18=1;
 v=&p->x0->x20;
 v->x=100.0f; v->y=100.0f; v->z=100.0f;
 p->x0->x36=1; p->x0->x38=0;
}
