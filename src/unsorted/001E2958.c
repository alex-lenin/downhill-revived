/* UNIT 0x001E2958 0x001E2968 */
typedef struct {
    char pad0[0x514];
    int x514;
    int x518;
} Obj;

void func_001E2958(Obj *p, int value)
{
    p->x514 = value;
    p->x518 = 180;
}
