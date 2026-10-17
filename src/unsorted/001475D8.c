/* UNIT 0x001475D8 0x00147688 */
struct Obj { char x0[0x1E0]; int x1E0; };
struct Global { char pad[0x2A0]; int x2A0; };
struct Color { char pad[0x758]; int x758; };
extern struct Global D_004B25A0;
extern struct Color D_00279B00;
extern void func_00147750(void *);
void func_001475D8(struct Obj *obj) {
 int count=29,color; char *p=(char *)obj;
 do { --count; func_00147750(p); p+=16; } while(count>=0);
 obj->x1E0=0;
 if(D_004B25A0.x2A0==22) D_00279B00.x758=0x40404840;
 else if(D_004B25A0.x2A0==18) D_00279B00.x758=0x50608040;
 else D_00279B00.x758=0x50303840;
}
