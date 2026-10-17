/* UNIT 0x0018E408 0x0018E468 */
typedef struct { char pad[0x418]; int x418; } Obj;
extern void func_0018D8A8(void *, void *);
void func_0018E408(Obj *obj, void *arg1, int value) {
    func_0018D8A8((char *)obj + (obj->x418 * 16 + 0x18), arg1);
    *((char *)obj + obj->x418 * 16 + 0x1e) = value;
    obj->x418++;
}
