/* UNIT 0x0016C348 0x0016C420 */
typedef struct { char pad[0x5bb4]; int mode; char gap[0x10]; float x5bc8, x5bcc, x5bd0, x5bd4, x5bd8; int x5bdc; } Obj;
void func_0016C348(Obj *obj) {
 float a = 100.0f - (float)obj->x5bdc;
 float b = 100.0f - a;
 switch (obj->mode) {
 case 0: obj->x5bd8 = obj->x5bc8; break;
 case 2: obj->x5bd8 = obj->x5bcc; a -= a * 0.25f; b = 100.0f - a; break;
 case 1:
 default: obj->x5bd8 = (obj->x5bcc + obj->x5bc8) * 0.5f; break;
 }
 obj->x5bd8 = obj->x5bd8 + a * (obj->x5bd0 - 1.0f) * 0.01f + b * (obj->x5bd4 - 1.0f) * 0.01f;
}
