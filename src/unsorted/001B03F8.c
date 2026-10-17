/* UNIT 0x001B03F8 0x001B0410 */
typedef struct { char pad[0x14]; int x14; int x18; int x1C; int x20; int x24; } Object;
void func_001B03F8(Object *object) {
    object->x18 = 0;
    object->x14 = 0;
    __asm__ volatile ("sqc2 $vf0, 0x0(%0)" : : "r"(object) : "memory");
    object->x1C = 0;
    object->x24 = 0;
}

