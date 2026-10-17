/* UNIT 0x001E5460 0x001E5680 */
typedef struct Triple { int x,y,z; } Triple;
typedef struct Obj {
 int x0,x4,x8,xC,x10,x14,x18,x1C,x20,x24,x28,x2C,x30,x34,x38,x3C,x40;
 Triple x44[43];
 int x248[43],x2F4[43];
 int x3A0[8],x3C0[30];
 Triple x438[30],x5A0[30];
 int x708,x70C,x710,x714,x718,x71C,x720,x724;
 int x728[300],xBD8[300],x1088[20];
 int x10D8,x10DC;
 unsigned char x10E0,x10E1,x10E2,x10E3,x10E4,x10E5,x10E6,x10E7,x10E8,x10E9;
 char pad10EA[2];
 int x10EC,x10F0,x10F4,x10F8;
} Obj;
void func_001E5460(Obj *obj,int value) {
 obj->x40=value;
 obj->x0=0; obj->x4=0; obj->x8=0; obj->xC=0;
 obj->x10=0; obj->x14=0; obj->x18=0; obj->x1C=0;
 obj->x20=0; obj->x24=0; obj->x28=0; obj->x2C=0;
 obj->x30=0; obj->x34=0; obj->x10F8=0; obj->x38=0;
 { int i; for(i=0;i<43;i++) { obj->x44[i].x=0; obj->x44[i].y=0; obj->x44[i].z=0; } }
 { int i; for(i=0;i<43;i++) { obj->x248[i]=0; obj->x2F4[i]=0; } }
 { int i; for(i=7;i>=0;i--) obj->x3A0[i]=0; }
 { int i; for(i=29;i>=0;i--) obj->x3C0[i]=0; }
 { int i; for(i=0;i<30;i++) {
  obj->x438[i].x=0; obj->x438[i].y=0; obj->x438[i].z=0;
  obj->x5A0[i].x=0; obj->x5A0[i].y=0; obj->x5A0[i].z=0;
 } }
 obj->x708=0; obj->x70C=0; obj->x710=0;
 obj->x10E8=0; obj->x10E9=0;
 obj->x714=0; obj->x718=0; obj->x71C=0; obj->x720=0; obj->x724=0;
 { int i; for(i=0;i<300;i++) { obj->x728[i]=0; obj->xBD8[i]=0; } }
 { int i; for(i=19;i>=0;i--) obj->x1088[i]=0; }
 obj->x10E5=1; obj->x10D8=0; obj->x10DC=0;
 obj->x10E0=0; obj->x10E1=0; obj->x10E2=0; obj->x10E3=0; obj->x10E4=0;
 obj->x10E6=0; obj->x10E7=0; obj->x10EC=0; obj->x10F0=0; obj->x10F4=0;
}
