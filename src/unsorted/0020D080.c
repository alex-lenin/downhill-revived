/* UNIT 0x0020D080 0x0020D0A0 */
typedef struct {
    char pad0[0x44];
    char x44;
    char pad45[3];
} Entry; /* size 0x48 */

typedef struct {
    Entry entries[80];      /* 0x0000 */
    char pad1680[0xC];
    int x168C;
    int x1690;
} Obj;

void func_0020D080(Obj *obj, int index)
{
    obj->x168C = 0;
    obj->x1690 = 0;
    obj->entries[index].x44 = 0;
}
