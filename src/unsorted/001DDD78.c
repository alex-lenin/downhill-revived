/* UNIT 0x001DDD78 0x001DDDB0 */
struct Obj { char pad0[0x174]; unsigned int x174; };
extern void func_001DF600(unsigned int *);
void func_001DDD78(struct Obj *obj) {
    unsigned int value;
    value = obj->x174;
    if (value != 0xFFFFFFFFU && value != 0) {
        func_001DF600(&obj->x174);
    }
}
