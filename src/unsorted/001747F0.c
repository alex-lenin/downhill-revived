/* UNIT 0x001747F0 0x00174840 */
typedef struct Obj { char pad0[0x11A8]; int x11A8; char pad1[0xC4]; int x1270; char pad2[0x169]; unsigned char x13DD; } Obj;
extern int func_00138560(void *);
int func_001747F0(Obj **obj)
{
    Obj *p;
    int value;
    if (func_00138560((char *)*obj + 0x40C0)) return 0;
    p = *obj;
    value = p->x13DD ? p->x1270 : p->x11A8;
    return value != 0;
}
