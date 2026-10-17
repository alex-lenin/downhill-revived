/* UNIT 0x001169F8 0x00116C48 */
typedef struct Global { char pad[0x5BB4]; int x5BB4; char pad2[0x24]; int x5BDC; } Global;
typedef struct Other { char pad[0x796C]; int x796C; } Other;
typedef struct Object { char pad[0x58]; Other *x58; char pad2[0x70]; union { unsigned int flags; struct { unsigned int low:4; unsigned int flag:1; unsigned int high:27; } bits; } xCC; } Object;
extern Global D_00398C40;
extern int func_00222BD8(int, int);
void func_001169F8(Object *obj) {
 int value;
 if(D_00398C40.x5BB4 == 0) value=D_00398C40.x5BDC/2;
 else if(D_00398C40.x5BB4 == 2) value=D_00398C40.x5BDC/2+50;
 else value=D_00398C40.x5BDC/2+25;
 switch(obj->x58->x796C) {
 case 0x20: obj->xCC.bits.flag = value>30; break;
 case 0x40: obj->xCC.bits.flag = value>10; break;
 case 0x60: obj->xCC.bits.flag = value>60; break;
 case 0x80: obj->xCC.bits.flag = value>30; break;
 case 0xA0: obj->xCC.bits.flag = value>20; break;
 case 0xC0: obj->xCC.bits.flag = value>40; break;
 case 0xE0: obj->xCC.bits.flag = value>80; break;
 case 0x100: obj->xCC.bits.flag = value>70; break;
 case 0x120: obj->xCC.bits.flag = value>60; break;
 case 0x140: obj->xCC.bits.flag = value>90; break;
 case 0x160: obj->xCC.bits.flag = value>20; break;
 case 0x180: obj->xCC.bits.flag = value>35; break;
 case 0x1A0: obj->xCC.bits.flag = value>70; break;
 case 0x1C0: obj->xCC.bits.flag=0; break;
 case 0x1E0:
 default: obj->xCC.bits.flag=0; break;
 }
 if(obj->xCC.flags & 16) obj->xCC.bits.flag=func_00222BD8(0,3);
}
