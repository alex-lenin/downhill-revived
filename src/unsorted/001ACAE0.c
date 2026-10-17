/* UNIT 0x001ACAE0 0x001ACB70 */
typedef struct { int x0; float x4, x8; } Range;
typedef struct { char pad[0x10]; Range *x10[1]; } Table;
typedef struct { char pad[0x20]; Table *x20; } Root;
typedef struct { char pad[0x14]; int x14; float x18; } Target;
typedef struct { Root *x0; Target *x4; char pad8[4]; short xC; } Link;
typedef struct { Link *x0; char pad4[0x30]; int x34; float x38; } Obj;
extern int func_00202DA8(void);
void func_001ACAE0(Obj *obj, float fraction) {
    Link *link = obj->x0;
    if (link != 0) {
        Root *root = link->x0;
        if (root != 0) {
            Range *range = root->x20->x10[link->xC];
            float base = range->x4;
            float value = (range->x8 - base) * fraction + base;
            int result = func_00202DA8();
            obj->x0->x4->x14 = obj->x34 = result;
            obj->x0->x4->x18 = obj->x38 = value;
        }
    }
}
