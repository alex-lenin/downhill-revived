/* UNIT 0x001E65D0 0x001E65E8 */
typedef struct {
    char pad0[0x20];
    float x20;
} Obj;

int func_001E65D0(Obj *obj)
{
    return obj->x20;
}
