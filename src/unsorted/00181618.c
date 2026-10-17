/* UNIT 0x00181618 0x00181680 */
struct Obj {
    char pad0[0x134];
    float x134;
    char pad138[0x140 - 0x138];
    char x140[1];
};

extern float *func_00261028(void *);
extern float *func_00261038(void *);
extern float *func_00261030(void *);

void func_00181618(struct Obj *obj, float value)
{
    float scaled;

    scaled = value * 60.0f;
    *func_00261028(obj->x140) = scaled;
    *func_00261038(obj->x140) = scaled;
    *func_00261030(obj->x140) = scaled;
    obj->x134 = scaled;
}
