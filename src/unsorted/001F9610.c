/* UNIT 0x001F9610 0x001F9620 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct {
    char pad0[0x490];
    u128 x490;
} Obj;

void func_001F9610(Obj *obj)
{
    obj->x490 = 0;
}
