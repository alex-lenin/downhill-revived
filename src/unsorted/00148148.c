/* UNIT 0x00148148 0x001481C8 */
struct Obj {void *x0;char pad4[0xAC];};
extern char D_00284300[];
extern struct Obj D_00373A60[];
void func_00148148(int a,int b) {
 int i,j;struct Obj *p;
 if(b==0xFFFF && a) {
 i=15;while(--i!=-1) {}
 p=D_00373A60;j=255;
 do {p->x0=D_00284300;j--;p++;}while(j!=-1);
 }
}
