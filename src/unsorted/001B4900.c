/* UNIT 0x001B4900 0x001B4A60 */
struct Global { char pad[0xE00]; int xE00; };
struct State { char pad[0x130]; int x130; };
struct Timer { char pad[0x508]; int x508; };
struct Game { char pad[0x10C]; int x10C; char pad110[0x374-0x110]; char x374[1]; };
extern int D_00270738[];
extern struct Global D_0037EA60;
extern struct State D_003F52D0;
extern struct Timer D_00437340;
extern struct Game D_004B25A0;
extern char D_004B1F40[];
extern int func_002205A0(int);
extern int func_00220618(int,int);
extern void func_001C5B80(void *);
extern void func_001C6628(void *);
extern void func_001B45F8(void *,int,int);
void func_001B4900(void) {
    int i=0;
    int changed=0;
    for (;i<D_0037EA60.xE00;i++) {
        if (func_00220618(8,func_002205A0(i))) {
        if (D_00270738[i] == 0) {
            changed=1;
            D_003F52D0.x130=i;
            D_00270738[i]=1;
        }
        } else D_00270738[i]=0;
    }
    if (changed) {
        if (D_004B25A0.x10C == 10) {
            func_001C5B80(D_004B25A0.x374);
            func_001B45F8(D_004B1F40,0,1);
            func_001C6628(D_004B25A0.x374);
        } else func_001B45F8(D_004B1F40,2,0);
    }
    if (D_004B25A0.x10C == 14) {
        if (D_00437340.x508 > 0) D_00437340.x508--;
        if (D_00437340.x508 == 0) func_001B45F8(D_004B1F40,5,0);
    }
}
