/* UNIT 0x001A87E8 0x001A8830 */
struct A { int count; short x4[4]; float xC, x10; };
void func_001A87E8(struct A *arg0, short arg1, float max, float min) {
 int count = arg0->count;
 arg0->x4[count] = arg1;
 arg0->count = count + 1;
 if (arg0->xC < max) arg0->xC = max;
 if (min < arg0->x10) arg0->x10 = min;
}
