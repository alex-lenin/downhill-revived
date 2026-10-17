/* UNIT 0x001564A8 0x00156568 */
struct Obj { char pad[0x10]; float x10,x14,x18; char pad1c[0x54]; float x70,x74,x78,x7c,x80,x84,x88,x8c,x90,x94,x98,x9c,xa0; };
void func_001564A8(struct Obj *obj) {
 float scale, half, small;
 scale=obj->xa0; small=scale*0.6f; half=scale*0.5f;
 
 obj->x74=0.2f;obj->x70=0.65f;obj->x18=1.6f;
 obj->x8c=obj->x88=0.94f;obj->x90=0.08726646f;obj->x94=0.7853982f;obj->x98=-1.0471976f;
 obj->x78=small;obj->x7c=scale*1.2f;
 obj->x10=0.6f;obj->x14=0.5f;obj->x84=obj->x80=half;
}
