/* UNIT 0x001B0AC0 0x001B0AF0 */
typedef struct { char pad[0x70]; float x70; char x74; } Obj;
void func_001B0AC0(Obj *obj, float value) {
    float temp;
    float result;
    __asm__ volatile("min.s %0, %1, %2" : "=&f"(temp) : "f"(1.0f), "f"(value) : "memory");
    __asm__ volatile("max.s %0, %1, %2" : "=&f"(result) : "f"(-1.0f), "f"(temp) : "memory");
    obj->x74 = 1;
    obj->x70 = result;
}

