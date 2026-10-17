/* UNIT 0x001A8A10 0x001A8A48 */
typedef struct Obj { char pad[0x44]; unsigned char x44; } Obj;
extern void func_001A8CE8(Obj *, int);
extern void func_001A8958(Obj *);
void func_001A8A10(Obj *obj) {
    if (obj->x44) {
        func_001A8CE8(obj, 1);
    } else {
        func_001A8958(obj);
    }
}
