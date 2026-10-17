/* UNIT 0x00174920 0x00174968 */
struct Obj { char pad[0xc]; int xC; char pad10[0x14]; int x24; };
float func_00174920(struct Obj *obj) {
    float limited;
    if (obj->xC != 2) {
        return 0.0f;
    }
    __asm__ volatile ("min.s %0, %1, %2" : "=&f"(limited) : "f"((float)obj->x24), "f"(30.0f));
    return limited * 0.033333335f;
}
