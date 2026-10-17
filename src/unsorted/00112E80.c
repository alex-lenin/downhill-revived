/* UNIT 0x00112E80 0x00112EF8 */
typedef struct Obj { char pad0[0x408]; int x408; char pad40C[0x30]; int x43C; long x440; } Obj;
extern int D_0043EEC8[];
extern void func_00112EF8(Obj *, int, int);
void func_00112E80(Obj *obj) {
 int count = obj->x43C;
 int threshold = 300;
 if (count) threshold = 300 - D_0043EEC8[0] * 300 / count;
 if (obj->x408 < threshold) obj->x440 |= (long)0x8000 << 19;
 else if (obj->x408 >= 121) func_00112EF8(obj, count, threshold);
}
