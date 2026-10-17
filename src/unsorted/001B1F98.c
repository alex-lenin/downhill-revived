/* UNIT 0x001B1F98 0x001B1FE0 */
extern void func_001B13A0(int);
void func_001B1F98(int object) {
    int end = object + 0x440;
    do {
        func_001B13A0(object);
        object += 0x110;
    } while (object < end);
}
