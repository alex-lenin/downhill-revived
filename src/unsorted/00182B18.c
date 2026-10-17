/* UNIT 0x00182B18 0x00182CF8 */
typedef struct { float x0; float x4; float x8; float xC; float x10; float x14; float x18; float x1C; float x20; float x24; float x28; float x2C; float x30; float x34; float x38; float x3C; float x40; float x44; float x48; float x4C; float x50; float x54; float x58; float x5C; float x60; float x64; float x68; float x6C; float x70; float x74; float x78; float x7C; float x80; float x84; float x88; } Obj;
extern struct {char pad[0x288];int x288;char pad28c[0xcc];int x358;} D_004B25A0;
float func_00182B18(Obj *obj,int index) {
 Obj *selected;
 obj->x24=7.7f;
 obj->x48=8.8f;
 obj->xC=11.8f;
 obj->x6C=11.8f;
 obj->x50=11.8f;
 obj->x38=12.0f;
 obj->x54=12.0f;
 obj->x3C=12.6f;
 obj->x10=12.6f;
 obj->x4C=10.3f;
 obj->x5C=12.15f;
 obj->x58=12.1f;
 obj->x70=9.6f;
 obj->x20=12.7f;
 obj->x64=10.4f;
 obj->x14=12.05f;
 obj->x60=8.4f;
 obj->x44=7.3f;
 obj->x2C=7.3f;
 obj->x34=8.4f;
 obj->x28=7.55f;
 obj->x40=11.95f;
 obj->x68=2.7f;
 obj->x78=3.5f;
 obj->x7C=3.5f;
 obj->x18=12.3f;
 obj->x30=7.8f;
 obj->x1C=10.9f;
 obj->x80=3.3f;
 obj->x84=3.3f;
 obj->x88=3.3f;
 selected=(Obj *)((char *)obj+(index<<2));
 return selected->xC*60.0f+(float)(D_004B25A0.x288+D_004B25A0.x358)*11.25f;
}
