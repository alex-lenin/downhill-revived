/* UNIT 0x001119F8 0x00111A88 */
typedef struct Owner { char pad[0x792c]; char *x792c; } Owner;
typedef struct Obj { Owner *x0; char pad4[0x39c]; int x3a0; char pad3a4[0x64]; int x408; } Obj;
extern char D_002AF2C0[];
extern int func_0011C488(void *, void *);
int func_001119F8(Obj *obj,int mode) {
 int result=1, value;
 if(((obj->x3a0>>11)&1) || func_0011C488(D_002AF2C0,obj->x0->x792c+16)==0) {
 value=obj->x408;
 if(value>0) { if(mode) { if(value>=61) result=0; } else result=0; }
 } else result=0;
 return result;
}
