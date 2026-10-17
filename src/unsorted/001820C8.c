/* UNIT 0x001820C8 0x00182148 */
typedef struct Packed { char pad[0x12]; unsigned short x12; } Packed;
void func_001820C8(void *unused, Packed *p, float *x, float *y, float *z) {
 *x=(float)(p->x12&31)*0.06451613f-1.0f;
 *y=(float)((p->x12>>5)&31)*0.06451613f-1.0f;
 *z=(float)((p->x12>>10)&31)*0.06451613f-1.0f;
}
