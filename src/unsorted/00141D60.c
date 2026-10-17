/* UNIT 0x00141D60 0x00141E20 */
struct Obj {char pad[0xF0]; int xF0,xF4,xF8; char padFC[0x70]; int x16C;};
extern int func_00208FB8();
extern void func_00218278(int,unsigned int *);
int func_00141D60(struct Obj *p,unsigned int *data) {
 p->xF0=func_00208FB8();
 if(p->xF0) {
 switch(*data>>18) {case 0x4D8:p->x16C=1;break;case 0x4DD:p->x16C=2;break;case 0x4E2:p->x16C=0;break;}
 func_00218278(p->xF0,data);p->xF4=*data>>18;p->xF8=(*data>>7)&0x7FF;return 1;
 } return 0;
}
