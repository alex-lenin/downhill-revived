/* UNIT 0x00129230 0x001292E0 */
struct Obj { char pad[0x3138]; int x3138[68]; int x3248; };
struct Event { int x0; char pad[12]; int *x10; };
extern int func_00208FB8(void);
extern void func_002182A8(int, int);
extern void func_00218278(int, int *);
void func_00129230(struct Obj *obj, struct Event *event) {
    int *data;
    if ((event->x0 & 63) == 6) {
        data = event->x10;
        if ((*data & 63) == 1) {
            obj->x3138[obj->x3248] = func_00208FB8();
            func_002182A8(obj->x3138[obj->x3248], 0);
            func_00218278(obj->x3138[obj->x3248], data);
            obj->x3248++;
        }
    }
}
