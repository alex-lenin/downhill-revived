/* UNIT 0x00192D50 0x00192DB0 */
typedef struct { int x0; char *x4; char pad8[0x228]; void *x230; char x234[1]; } Obj;
extern void func_00126F38(void *, void *, char *);
extern void func_001C77D0(void *, void *);
extern void func_001BDA58(void *, void *, int, int);
/* A breakable object hit by a rider: direction, rider. The direction and the rider go on to
   func_00126F38 (the impulse that knocks the object away) in the registers they came in. */
void func_00192D50(Obj *obj, void *direction, char *rider) {
    if (obj->x230) {
        func_00126F38(obj->x230, direction, rider);
        func_001C77D0(rider + 0x7720, obj->x4 + 0x10);
        func_001BDA58(obj->x234, obj, 0x14, 0x8c);
    }
}
