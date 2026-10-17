/* UNIT 0x001711B8 0x00171310 */
struct Obj { char pad[0x2940]; int x2940[20]; char pad2[0]; float x2990, x2994, x2998, x299C, x29A0, x29A4, x29A8; int x29AC, x29B0, x29B4, x29B8, x29BC, x29C0, x29C4, x29C8; unsigned char x29CC; };
struct Global { char pad[0x2A0]; int x2A0; };
extern struct Global D_004B25A0;
extern float D_0029C8C0[];
extern void func_00171538(void *);
void func_001711B8(struct Obj *obj) {
    int *flags = obj->x2940;
    char *p = (char *)obj;
    int count = 19;
    float value;
    do {
        --count;
        func_00171538(p);
        p += 0x210;
        *flags++ = 0;
    } while (count >= 0);
    if (D_004B25A0.x2A0 == 2) obj->x29CC = 1;
    else obj->x29CC = 0;
    if (obj->x29CC) {
        obj->x2990 = 0.9777778f;
        obj->x2994 = 0.9777778f;
        obj->x2998 = 10.0f;
        obj->x299C = 20.0f;
    } else {
        obj->x2990 = 0.1711111f;
        obj->x2994 = 0.2688889f;
        obj->x2998 = 1.75f;
        obj->x299C = 30.0f;
    }
    value = D_0029C8C0[0];
    obj->x29A0 = 2.2f;
    obj->x29A4 = 0.15f;
    obj->x29AC = 6;
    obj->x29B0 = 186;
    obj->x29B4 = 30;
    obj->x29B8 = 180;
    obj->x29A8 = value;
    obj->x29C4 = -1;
    obj->x29BC = 0;
    obj->x29C0 = 0;
    obj->x29C8 = 1;
}
