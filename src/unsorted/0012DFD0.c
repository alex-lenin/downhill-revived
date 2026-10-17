/* UNIT 0x0012DFD0 0x0012E028 */
typedef struct Obj { char pad[0x1198]; int x1198,x119C; char pad11A0[0x1260-0x11A0]; int x1260,x1264; char pad1268[0x13D9-0x1268]; unsigned char x13D9,x13DA; } Obj;
float func_0012DFD0(Obj *obj) {
 float result=0.0f;
 int a,b;
 if(obj->x13D9) a=obj->x1260; else a=obj->x1198;
 if(a) result += 1.0f;
 if(obj->x13DA) b=obj->x1264; else b=obj->x119C;
 if(b) result -= 1.0f;
 return result;
}
