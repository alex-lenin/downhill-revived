/* UNIT 0x001755F0 0x00175658 */
typedef struct { int x0; char x4; } Entry;
typedef struct { char pad[0x1168]; Entry *x1168,*x116C,*x1170,*x1174,*x1178,*x117C,*x1180,*x1184,*x1188,*x118C; } Obj;
void func_001755F0(Obj *obj) {
 if(obj->x1168) obj->x1168->x4=0;
 if(obj->x116C) obj->x116C->x4=0;
 if(obj->x1178) obj->x1178->x4=0;
 if(obj->x1180) obj->x1180->x4=0;
 if(obj->x117C) obj->x117C->x4=0;
 if(obj->x1170) obj->x1170->x4=0;
 if(obj->x1184) obj->x1184->x4=0;
 if(obj->x118C) obj->x118C->x4=0;
}
