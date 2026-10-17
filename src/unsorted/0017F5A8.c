/* UNIT 0x0017F5A8 0x0017F6A0 */
struct Obj {
    char pad0[0x5530];
    int handles[40];
    char pad55D0[0xCC];
    int ids[40];
    int count;
};

extern char D_00383A10[];
extern int func_001591D8(unsigned int *);
extern void func_002182A8(int, int);
extern void func_00158120(void *, int, float);

/* Every branch plays the same sound; the copies are merged by cross-jumping, which leaves
   the dead type comparisons in retail. count++ must sit inside each copy: a branch ending
   in the call gets a gcse (use 0) marker that stops the merge. */
#define PLAY() \
    func_00158120(D_00383A10, obj->handles[obj->count], 8.0f); \
    obj->count++

void func_0017F5A8(struct Obj *obj, unsigned int *arg) {
    unsigned int t;

    if (obj->count < 40) {
        obj->handles[obj->count] = func_001591D8(arg);
        func_002182A8(obj->handles[obj->count], 0);
        if (arg != 0) {
            obj->ids[obj->count] = (*arg >> 7) & 0x7FF;
            goto Check;
        } else {
            PLAY();
            return;
        }
    Check:
        if (arg == 0) {
            PLAY();
        } else {
            t = *arg >> 18;
            if (t == 0x516) {
                PLAY();
            } else if (t < 0x517) {
                PLAY();
            } else if (t == 0x57A) {
                PLAY();
            } else if (t < 0x57B) {
                PLAY();
            } else {
                PLAY();
            }
        }
    }
}
