/* UNIT 0x001722A0 0x001722E0 */
typedef struct Fields { char pad[0x44]; float x44; } Fields;
typedef struct Obj { char pad[0xA0]; Fields fields; } Obj;
void func_001722A0(Obj *obj, float value)
{
    float result;
    Fields *fields = &obj->fields;
    value = value * 40.90909194946289f;
    value = value * 1.4666666984558105f;
    value = value * 1.5f;
    __asm__ volatile ("max.s %0, %1, %2" : "=&f"(result) : "f"(0.0f), "f"(value), "r"(fields));
    fields->x44 = result;
}
