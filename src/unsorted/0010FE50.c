/* UNIT 0x0010FE50 0x0010FF38 */
typedef struct Sub74 { char pad0[0x10]; unsigned char x10; } Sub74;
typedef struct Sub0 { char pad0[0x4564]; short x4564; } Sub0;
typedef struct Obj {
    Sub0 *x0;
    char pad4[0x70];
    Sub74 *x74;
    char pad78[0x1C8];
    float x240;
    char pad244[0xC];
    float x250;
    char pad254[0x24];
    float x278;
    float x27C;
    char pad280[8];
    float x288;
    char pad28C[0x1C];
    float x2A8;
    char pad2AC[0x114];
    int x3C0;
} Obj;

extern int D_0043EEC8[];
extern float fabsf(float);
extern void func_0010FF38(Obj *);
extern void func_00110330(Obj *);

void func_0010FE50(Obj *obj)
{
    int active = obj->x3C0 != 0;

    if (D_0043EEC8[0] < 600) {
        active = 0;
    } else {
        float speed = obj->x278;
        float aspeed = fabsf(speed);
        __asm__ volatile("Lfe50_speed:" : : "f"(speed));
        do {
            if (aspeed < obj->x74->x10 * 0.5f) {
                active = 0;
            }
        } while (0);
    }
    if (active) {
        short level = obj->x0->x4564;
        obj->x288 = 0.3f;
        if (level >= 11) {
            active = 0;
        }
    } else {
        obj->x288 = 0.4f;
    }
    if (active) {
        func_0010FF38(obj);
    } else {
        func_00110330(obj);
        obj->x27C = obj->x250;
        obj->x2A8 = obj->x240;
    }
}
