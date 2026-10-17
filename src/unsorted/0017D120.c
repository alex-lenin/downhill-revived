/* UNIT 0x0017D120 0x0017D170 */
typedef struct Entry { void *x0; char pad[40]; } Entry;
extern char D_00285BE8[];
extern Entry D_003B71D0[];
void func_0017D120(int enabled,int mode) {
 Entry *p; int count;
 if(mode==65535 && enabled) { p=D_003B71D0; count=31; do { p->x0=D_00285BE8; count--; p++; } while(count!=-1); }
}
