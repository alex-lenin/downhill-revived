/* UNIT 0x0016A9E0 0x0016AA58 */
typedef struct Obj {
    int state;
} Obj;

extern int func_0016A788(Obj *obj);
extern int func_0016A860(Obj *obj);
extern int func_0016A990(Obj *obj);

int func_0016A9E0(Obj *obj)
{
    switch (obj->state) {
    case 0:
        break;
    case 1:
        return func_0016A788(obj);
    case 2:
        return func_0016A860(obj);
    case 3:
        return func_0016A990(obj);
    }
    return 0;
}
