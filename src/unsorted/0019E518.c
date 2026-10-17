/* UNIT 0x0019E518 0x0019E530 */
typedef struct Obj {
    char pad0[0x610];
    char *x610;
    int x614;
} Obj;

char *func_0019E518(Obj *obj)
{
    return obj->x610 + obj->x614 * 3;
}
