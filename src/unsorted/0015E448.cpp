/* UNIT 0x0015E448 0x0015E630 */
struct Slot { char pad[0xC]; short type; char tail[2]; bool Is(int t) { return type==t; } };
struct Menu { char pad[0x10]; Slot slots[1]; };
typedef struct Child { char pad[4]; char x4; } Child;
typedef struct Manager { char pad[0x12D]; char x12D; char gap[2]; int x130; Child *x134; } Manager;
typedef struct Screen { char pad[0x500]; int x500; } Screen;
extern Manager D_003F52D0;
extern Screen D_00386680,D_00387F90,D_00388990,D_00389390,D_00389D90,D_0038A2C0,D_003965E0,D_00397180,D_0043C850;
extern char D_004B28FC[];
extern void func_00190660(Manager *,Screen *);
extern void func_0015E6E0(Screen *);
void func_0015E448(Menu *obj,int index) {

 if (!index) { D_004B28FC[0]=1; }
 else {
  if (obj->slots[index].Is(0x1A)) { func_00190660(&D_003F52D0,&D_00387F90); return; }
  if (obj->slots[index].Is(0x48)) { func_0015E6E0(&D_00389D90); func_00190660(&D_003F52D0,&D_00389D90); return; }
  if (obj->slots[index].Is(0x3F)) { func_00190660(&D_003F52D0,&D_00389390); return; }
  if (obj->slots[index].Is(0x47)) { D_0038A2C0.x500=D_003F52D0.x130; func_00190660(&D_003F52D0,&D_0038A2C0); return; }
  if (obj->slots[index].Is(0x6C)) { func_00190660(&D_003F52D0,&D_003965E0); return; }
  if (obj->slots[index].Is(0x22)) { func_00190660(&D_003F52D0,&D_00388990); return; }
  if (obj->slots[index].Is(0x7D)) { D_00397180.x500=0; func_00190660(&D_003F52D0,&D_00397180); return; }
  if (obj->slots[index].Is(0x29)) { func_00190660(&D_003F52D0,&D_00386680); return; }
 }
 if (obj->slots[index].Is(0x85)) { func_00190660(&D_003F52D0,&D_0043C850); return; }
 if (D_003F52D0.x134) D_003F52D0.x134->x4=0;
 D_003F52D0.x12D=1;
}
