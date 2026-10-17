/* UNIT 0x001E9998 0x001E9A48 */
struct Vector { float x,y,z,w; };
struct Obj { char pad[0x1f8]; struct Vector x1f8; };
extern void func_00203E10(struct Vector*,float);
extern void func_00203B68(struct Vector*,int,float);
extern void func_002038E0(struct Vector*,int);
void func_001E9998(struct Obj *obj) {
 struct Vector vector;
 vector=obj->x1f8;
 func_00203E10(&vector,0.0f);
 vector=obj->x1f8;
 func_00203B68(&vector,1,1.0f);
 vector=obj->x1f8;
 func_002038E0(&vector,0);
}
