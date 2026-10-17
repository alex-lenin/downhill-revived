/* UNIT 0x0018D028 0x0018D088 */
typedef struct Obj { char p0[4]; unsigned char x4; } Obj;
typedef struct Entry { int x0; Obj *x4; } Entry;
extern Entry D_003EC540[10][4];
void func_0018D028(void) {
 int i,j;
 for(i=0;i<10;i++) {
 for(j=0;j<4;j++) {
 Obj *p=D_003EC540[i][j].x4;
 if(p) p->x4=0;
 }
 }
}
