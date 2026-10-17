/* UNIT 0x00211280 0x002112D0 */
typedef struct { int x0; char pad[24]; float x1C; } Info;
typedef struct { Info *x0; char pad[12]; char x10[16]; } Obj;
extern void func_00210F40(Obj *, void *, int, float);
void func_00211280(Obj *obj) {
    Info *info = obj->x0;
    int type = info->x0 & 63;
    float value;
    if (type == 1) goto selected;
    if (type != 2) goto fallback;
selected:
    value = info->x1C;
    goto done;
fallback:
    value = 30.0f;
done:
    func_00210F40(obj, obj->x10, type, value);
}
