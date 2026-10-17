/* UNIT 0x0016EE40 0x0016EF78 .rodata=0x28B040:0x28B080 */
struct Child { char pad[0x16]; short x16; };
struct Obj { int state; char pad4[0x94]; struct Child *x98; char pad9C[0x620]; unsigned char x6BC,x6BD,x6BE,x6BF; };
extern void func_0016FBB0(void *);
extern void func_0016F9F8(void *);
extern void func_0016EB40(struct Obj *);
extern void func_0016ED38(struct Obj *);
void func_0016EE40(struct Obj *obj) {
 obj->x6BC=0; obj->x6BD=0; obj->x6BE=0; obj->x6BF=0;
 switch(obj->state) {
 case 1: obj->state=2; break;
 case 2: obj->state=3; func_0016EB40(obj); break;
 case 7: obj->state=3; func_0016EB40(obj); break;
 case 3: func_0016FBB0((char *)obj+0x10); obj->state=4; break;
 case 4: obj->state=5; func_0016EB40(obj); break;
 case 5: obj->state=6; func_0016ED38(obj); break;
 case 6: obj->state=8; func_0016EB40(obj); break;
 case 8: func_0016FBB0((char *)obj+0x10); obj->state=9; break;
 case 9: obj->state=10; func_0016EB40(obj); break;
 case 10: obj->state=11; break;
 case 11: obj->state=12; func_0016EB40(obj); break;
 case 12: func_0016FBB0((char *)obj+0x10); obj->state=13; break;
 case 13: if(obj->x98->x16<=0) obj->state=14; else { obj->state=1; func_0016EB40(obj); } func_0016F9F8((char *)obj+0x10); break;
 case 0: case 14: default: break;
 }
}
