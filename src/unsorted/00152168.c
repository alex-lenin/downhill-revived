/* UNIT 0x00152168 0x001521F0 */
typedef struct Entry { void *vtable; char pad[44]; } Entry;
extern char D_00284668[],D_002846A0[];
extern Entry D_00381DF0[],D_003830B0[];
void func_00152168(int a,int b) {
 int i; Entry *p;
 if(b==65535 && a!=0) {
 for(i=99;i!=-1;i--) D_00381DF0[99-i].vtable=D_002846A0;
 p=D_003830B0; i=49; do { p->vtable=D_00284668; i--; p++; } while(i!=-1);
 }
}
