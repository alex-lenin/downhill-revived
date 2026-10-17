/* UNIT 0x0017D6B0 0x0017D700 */
typedef struct Entry { void *x0; char pad[20]; } Entry;
extern char D_00285CB0[];
extern Entry D_003B7D98[];
void func_0017D6B0(int enabled,int mode) {
 Entry *p; int count;
 if(mode==65535 && enabled) { p=D_003B7D98; count=3; do { p->x0=D_00285CB0; count--; p++; } while(count!=-1); }
}
