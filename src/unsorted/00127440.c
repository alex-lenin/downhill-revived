/* UNIT 0x00127440 0x00127458 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct Obj { char pad[0xD8]; float xD8; int xDC; volatile u128 xE0; char padF0[0x50]; int x140; } Obj;
void func_00127440(Obj *obj, int value, u128 *vector, float scale) {
 obj->x140=value; obj->xD8=scale; obj->xE0=*vector;
}
