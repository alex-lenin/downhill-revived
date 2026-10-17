/* UNIT 0x00176E40 0x00176ED8 */
typedef struct { int x0; char x4; } Item;
typedef struct { char pad[0x1168]; Item *x1168,*x116C,*x1170,*x1174,*x1178,*x117C,*x1180,*x1184,*x1188,*x118C; char pad2[0xEC]; int x127C; } Obj;
extern int D_004B2828[];
void func_00176E40(Obj *obj,int flag) {
 Item *last;
 obj->x127C=flag;
 if(!flag) {
 if(obj->x1168) obj->x1168->x4=0;
 if(obj->x116C) obj->x116C->x4=0;
 if(obj->x1178) obj->x1178->x4=0;
 if(obj->x1180) obj->x1180->x4=0;
 if(obj->x117C) obj->x117C->x4=0;
 if(obj->x1170) obj->x1170->x4=0;
 if(obj->x1188) obj->x1188->x4=0;
 if(obj->x1184) obj->x1184->x4=0;
 last=obj->x118C;
 } else { if(D_004B2828[0]<2) return; last=obj->x1180; }
 if(last) last->x4=0;
}
