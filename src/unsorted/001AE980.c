/* UNIT 0x001AE980 0x001AE998 */
typedef struct { char pad[0xA0]; int xA0; int xA4; int xA8; } Object;
void func_001AE980(Object *object) {
    object->xA0 = 0;
    __asm__ volatile ("sqc2 $vf0, 0x10(%0)" : : "r"(object) : "memory");
    object->xA4 = 0;
    object->xA8 = 0;
}
