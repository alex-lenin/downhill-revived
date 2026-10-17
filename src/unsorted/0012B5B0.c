/* UNIT 0x0012B5B0 0x0012B600 */
typedef struct Entry { void *x0; char pad4[0x30]; int x34; char pad38[0xc]; int x44; char pad48[0xc]; int x54; char pad58[0xc]; int x64; char pad68[0x138]; } Entry;
extern char D_00283A48[];
extern Entry D_0032CA00[];
void func_0012B5B0(int enabled,int id) {
 Entry *entry; int count; void *base;
 if(id==65535 && enabled) {
 base=D_00283A48; entry=D_0032CA00; count=29;
 do { entry->x0=base; --count; entry->x34=0; entry->x44=0; entry->x54=0; entry->x64=0; ++entry; } while(count!=-1);
 }
}
