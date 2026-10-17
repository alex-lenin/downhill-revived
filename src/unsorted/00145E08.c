/* UNIT 0x00145E08 0x00145E48 */
struct Part { char pad[0x44]; float x44; };
struct Obj { char pad[0x40]; struct Part x40; };
void func_00145E08(struct Obj *obj, float value) {
    struct Part *part = &obj->x40;
    float result;
    value = value * 40.909091949462890625f * 1.466666698455810546875f * 1.5f;
    __asm__ ("max.s %0, %1, %2" : "=&f"(result) : "f"(0.0f), "f"(value), "r"(part));
    part->x44 = result;
}

